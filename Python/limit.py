import asyncio
import json
import websockets
import time
import datetime
import os
import requests

# ===== 設定 =====
EDGE_REMOTE_DEBUGGING_URL = "http://localhost:9222/json"
TARGET_SITE_KEYWORD = "youtube.com"
DAILY_LIMIT_SECONDS = 2 * 60 * 60  # 2時間
SAVE_FILE = "youtube_limit_time.json"

# ===== カウント保存・読み込み =====
def load_state():
    if not os.path.exists(SAVE_FILE):
        return {"date": today_str(), "seconds": 0}

    try:
        with open(SAVE_FILE, "r") as f:
            return json.load(f)
    except:
        return {"date": today_str(), "seconds": 0}

def save_state(state):
    with open(SAVE_FILE, "w") as f:
        json.dump(state, f)

def today_str():
    return datetime.date.today().isoformat()

# ===== Edge のタブ一覧取得 =====
def get_edge_tabs():
    try:
        res = requests.get(EDGE_REMOTE_DEBUGGING_URL)
        return res.json()
    except:
        return []

# ===== タブを閉じる =====
async def close_tab(websocket_url):
    try:
        async with websockets.connect(websocket_url) as ws:
            cmd = {
                "id": 1,
                "method": "Page.close"
            }
            await ws.send(json.dumps(cmd))
    except Exception as e:
        print("タブを閉じる際のエラー:", e)

# ===== メインループ =====
async def main_loop():
    state = load_state()
    last_date = state["date"]

    print("YouTube時間制限システム 起動完了")
    print("バックグラウンドで実行中です…")

    while True:
        now_date = today_str()

        # 日付変わったらリセット
        if now_date != last_date:
            print("日付が変わったのでカウントをリセットします")
            state = {"date": now_date, "seconds": 0}
            save_state(state)
            last_date = now_date

        tabs = get_edge_tabs()

        youtube_open_tabs = [
            tab for tab in tabs
            if TARGET_SITE_KEYWORD in (tab.get("url") or "")
        ]

        # YouTube が開かれているなら 1秒加算
        if youtube_open_tabs:
            state["seconds"] += 1
            save_state(state)

            remaining = DAILY_LIMIT_SECONDS - state["seconds"]
            print(f"YouTube閲覧中: 経過 {state['seconds']} 秒 / 残り {remaining} 秒")

            if state["seconds"] >= DAILY_LIMIT_SECONDS:
                print("=== 時間制限を超えたので YouTube タブを閉じます ===")

                # 開いているYouTubeタブ全部閉じる
                for tab in youtube_open_tabs:
                    ws_url = tab.get("webSocketDebuggerUrl")
                    if ws_url:
                        await close_tab(ws_url)

                print("タブを閉じました。")

        await asyncio.sleep(1)  # 1秒ごとにチェック

# ===== 実行 =====
if __name__ == "__main__":
    try:
        asyncio.run(main_loop())
    except KeyboardInterrupt:
        print("終了します。")
