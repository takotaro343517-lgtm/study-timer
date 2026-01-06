import subprocess
from pathlib import Path
import tkinter as tk
from tkinter import filedialog, messagebox

def select_input_file():
    file_path = filedialog.askopenfilename(
        title="入力MP3ファイルを選択",
        filetypes=[("MP3 files", "*.mp3")]
    )
    if file_path:
        input_entry.delete(0, tk.END)
        input_entry.insert(0, file_path)

def select_output_file():
    file_path = filedialog.asksaveasfilename(
        title="保存先を指定",
        defaultextension=".mp3",
        filetypes=[("MP3 files", "*.mp3")]
    )
    if file_path:
        output_entry.delete(0, tk.END)
        output_entry.insert(0, file_path)

def convert_audio():
    input_mp3 = input_entry.get()
    output_mp3 = output_entry.get()

    if not input_mp3 or not output_mp3:
        messagebox.showerror("エラー", "入力ファイルと出力ファイルを指定してください。")
        return

    try:
        subprocess.run([
            "ffmpeg",
            "-i", input_mp3,
            "-af", "volume=6dB",
            output_mp3
        ], check=True)
        messagebox.showinfo("完了", "変換が完了しました！")
    except subprocess.CalledProcessError:
        messagebox.showerror("エラー", "変換中にエラーが発生しました。")

# UI構築
root = tk.Tk()
root.title("MP3音量アップツール")
root.geometry("500x200")

# 入力ファイル
tk.Label(root, text="入力MP3ファイル:").pack(anchor="w")
input_frame = tk.Frame(root)
input_frame.pack(fill="x", padx=5)
input_entry = tk.Entry(input_frame)
input_entry.pack(side="left", fill="x", expand=True)
tk.Button(input_frame, text="参照", command=select_input_file).pack(side="right")

# 出力ファイル
tk.Label(root, text="保存先MP3ファイル:").pack(anchor="w")
output_frame = tk.Frame(root)
output_frame.pack(fill="x", padx=5)
output_entry = tk.Entry(output_frame)
output_entry.pack(side="left", fill="x", expand=True)
tk.Button(output_frame, text="保存先を選択", command=select_output_file).pack(side="right")

# 実行ボタン
tk.Button(root, text="変換開始", command=convert_audio, bg="lightblue").pack(pady=10)

root.mainloop()
