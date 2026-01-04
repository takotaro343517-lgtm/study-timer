import pyautogui
import time
import keyboard

# ===== クリックしたい座標 =====
X = 330
Y = 510

# 安全装置
pyautogui.FAILSAFE = True

running = False  # 実行中かどうかのフラグ

print("F8：実行 / 一時停止")
print("Esc：終了")

while True:
    # Escキーで終了
    if keyboard.is_pressed("esc"):
        print("終了しました")
        break

    # F8キーで実行・停止を切り替え
    if keyboard.is_pressed("F8"):
        running = not running
        if running:
            print("▶ クリック開始")
        else:
            print("⏸ 一時停止")

        # キー長押し防止（重要）
        time.sleep(0.5)

    # 実行中のみクリック
    if running:
        pyautogui.click(X, Y, button="left")
        print(f"クリック: x={X}, y={Y}")
        time.sleep(0.0000000001)
    else:
        time.sleep(0.1)
