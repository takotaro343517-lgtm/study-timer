import tkinter as tk
import time

# ウィンドウの土台作成
root = tk.Tk()
# ウィンドウのタイトルを設定
root.title(u"Clock")
# ウィンドウのサイズを設定
root.geometry("1080x720")

clock = tk.Label(root, font = ('MS Gothic', 20), foreground = "#D8B555", background = 'white')
# pack()は自動的に上から配置してくれるので、簡単な配置には最適、place()は座標を指定して配置
clock.place(x = 10, y = 10, width = 230, height = 50)

# 時計を更新する関数
def tick():
    # 現在の日時を取得
    now = time.strftime("%H:%M:%S")
    # ラベルのテキストを更新
    clock.config(text=now)
    # 1000msごとに再度tick関数を呼び出す
    clock.after(1000, tick)

# 時計をスタート
tick()
# メインループの実行
root.mainloop()
