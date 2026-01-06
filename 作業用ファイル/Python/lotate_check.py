import pyautogui
from pynput import mouse

def on_click(x, y, button, pressed):
    # マウスボタンが「押された瞬間」のみ反応
    if pressed:
        pos = pyautogui.position()
        print(f"クリック座標: x={pos.x}, y={pos.y}")

        # 1回で終了したい場合は以下を有効化
        return False

# マウスリスナー開始
with mouse.Listener(on_click=on_click) as listener:
    print("画面をクリックすると座標が表示されます（Ctrl+Cで終了）")
    listener.join()
