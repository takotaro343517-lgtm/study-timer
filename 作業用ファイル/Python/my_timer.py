import tkinter as tk
from tkinter import ttk
import time
import threading
import pygame
from datetime import datetime


class PomodoroTimer:
    def __init__(self, root):
        self.root = root
        self.root.title("Pomodoro Timer")
        self.root.geometry("1000x380")

        # 状態管理
        self.remaining = 0
        self.running = False
        self.mode = "study"  # "study" or "break"
        self.study_count = 0  # 何回目の勉強が終わったか

        # pygameでmp3再生準備
        pygame.mixer.init()

        # UI
        self.create_widgets()

    def create_widgets(self):
        # ---------------------------
        # 入力フォーム（勉強・休憩）
        # ---------------------------
        input_frame = ttk.Frame(self.root)
        input_frame.pack(pady=10)

        ttk.Label(input_frame, text="勉強時間（秒）:").grid(row=0, column=0)
        self.study_var = tk.StringVar(value="1500")
        ttk.Entry(input_frame, textvariable=self.study_var, width=10).grid(row=0, column=1)

        ttk.Label(input_frame, text="休憩時間（秒）:").grid(row=1, column=0)
        self.break_var = tk.StringVar(value="300")
        ttk.Entry(input_frame, textvariable=self.break_var, width=10).grid(row=1, column=1)

        # ---------------------------
        # 状態表示
        # ---------------------------
        self.mode_label = ttk.Label(self.root, text="勉強中", font=("Helvetica", 20), foreground="blue")
        self.mode_label.pack(pady=10)

        # ---------------------------
        # タイマー表示
        # ---------------------------
        self.time_label = ttk.Label(self.root, text="00:00", font=("Helvetica", 40))
        self.time_label.pack(pady=10)

        # ---------------------------
        # ログ表示
        # ---------------------------
        self.log_label = ttk.Label(self.root, text="勉強回数: 0", font=("Helvetica", 14))
        self.log_label.pack(pady=10)

        # ---------------------------
        # ボタン
        # ---------------------------
        btn_frame = ttk.Frame(self.root)
        btn_frame.pack()

        ttk.Button(btn_frame, text="Start", command=self.start).grid(row=0, column=0, padx=5)
        ttk.Button(btn_frame, text="Stop", command=self.stop).grid(row=0, column=1, padx=5)
        ttk.Button(btn_frame, text="Reset", command=self.reset).grid(row=0, column=2, padx=5)

    # ---------------------------
    # 表示更新
    # ---------------------------
    def update_display(self):
        m, s = divmod(self.remaining, 60)
        self.time_label.config(text=f"{m:02d}:{s:02d}")

    # ---------------------------
    # カウントダウン
    # ---------------------------
    def countdown(self):
        if self.running and self.remaining > 0:
            self.remaining -= 1
            self.update_display()
            self.root.after(1000, self.countdown)
        elif self.remaining == 0 and self.running:
            # 終わったらアラーム
            self.play_alarm()

            # 勉強が終わったらカウント＋ログ保存
            if self.mode == "study":
                self.study_count += 1
                self.log_label.config(text=f"勉強回数: {self.study_count}")
                self.write_log()

            # 次のモードへ
            self.switch_mode()
            self.countdown()

    # ---------------------------
    # モード切替
    # ---------------------------
    def switch_mode(self):
        if self.mode == "study":
            # 休憩へ
            self.mode = "break"
            self.mode_label.config(text="休憩中", foreground="green")
            self.remaining = int(self.break_var.get())
        else:
            # 勉強へ
            self.mode = "study"
            self.mode_label.config(text="勉強中", foreground="blue")
            self.remaining = int(self.study_var.get())

        self.update_display()

    # ---------------------------
    # ボタン：Start
    # ---------------------------
    def start(self):
        if not self.running:
            self.running = True

            if self.remaining == 0:  # 初回
                if self.mode == "study":
                    self.remaining = int(self.study_var.get())
                else:
                    self.remaining = int(self.break_var.get())

            self.update_display()
            self.countdown()

    # ---------------------------
    # ボタン：Stop
    # ---------------------------
    def stop(self):
        self.running = False

    # ---------------------------
    # ボタン：Reset
    # ---------------------------
    def reset(self):
        self.running = False
        self.mode = "study"
        self.mode_label.config(text="勉強中", foreground="blue")
        self.remaining = int(self.study_var.get())
        self.study_count = 0
        self.log_label.config(text="勉強回数: 0")
        self.update_display()

    # ---------------------------
    # アラーム mp3 再生
    # ---------------------------
    def play_alarm(self):
        threading.Thread(target=self._play_mp3, daemon=True).start()

    def _play_mp3(self):
        try:
            pygame.mixer.music.load("alarm.mp3")  # 同じフォルダに置く
            pygame.mixer.music.play()
        except Exception as e:
            print("MP3エラー:", e)

    # ---------------------------
    # ログ書き込み
    # ---------------------------
    def write_log(self):
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        with open("pomodoro_log.txt", "a", encoding="utf-8") as f:
            f.write(f"{now} - 勉強{self.study_count}回目 完了\n")


# ---------------------------
# 実行
# ---------------------------
if __name__ == "__main__":
    root = tk.Tk()
    app = PomodoroTimer(root)
    root.mainloop()
