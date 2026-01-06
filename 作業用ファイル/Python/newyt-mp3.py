import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess
from pathlib import Path
import yt_dlp
from mutagen.easyid3 import EasyID3
from mutagen.mp3 import MP3

# ==========
# YouTube → MP3ダウンロード
# ==========
def download_youtube(url, output_folder, filename):
    opts = {
        'format': 'bestaudio/best',
        'extractaudio': True,
        'audio-format': "mp3",
        'audio-quality': "192K",
        'outtmpl': f'{output_folder}/{filename}.mp3'
    }
    with yt_dlp.YoutubeDL(opts) as ydl:
        ydl.download([url])
    return Path(f'{output_folder}/{filename}.mp3')

# ==========
# 再エンコード（iTunes対応）
# ==========
def reencode_mp3(input_path, output_path):
    result = subprocess.run([
        "ffmpeg", "-y",
        "-i", str(input_path),
        "-codec:a", "libmp3lame",
        "-qscale:a", "2",
        str(output_path)
    ], capture_output=True, text=True)
    return result.returncode == 0

# ==========
# ID3タグ付与
# ==========
def add_id3_tags(mp3_path, title, artist, album):
    try:
        audio = MP3(mp3_path, ID3=EasyID3)
        audio["title"] = title
        audio["artist"] = artist
        audio["album"] = album
        audio.save()
    except Exception as e:
        print("タグ追加失敗:", e)

# ==========
# 音量調整
# ==========
def adjust_volume(input_path, output_path, gain_db=6):
    try:
        subprocess.run([
            "ffmpeg", "-y",
            "-i", str(input_path),
            "-af", f"volume={gain_db}dB",
            str(output_path)
        ], check=True)
        return True
    except subprocess.CalledProcessError:
        return False

# ==========
# フォルダ参照
# ==========
def select_output_folder():
    folder = filedialog.askdirectory(title="保存先フォルダを選択")
    if folder:
        output_entry.delete(0, tk.END)
        output_entry.insert(0, folder)

# ==========
# 一括処理
# ==========
def start_download():
    url = url_entry.get().strip()
    name = name_entry.get().strip()
    artist = artist_entry.get().strip()
    album = album_entry.get().strip()
    volume = volume_entry.get().strip()

    if not url or not name:
        messagebox.showerror("エラー", "URLと保存名を入力してください。")
        return

    try:
        volume = int(volume)
    except ValueError:
        messagebox.showerror("エラー", "音量(dB)は整数で入力してください。")
        return

    output_folder = Path(output_entry.get() or ".")
    output_folder.mkdir(parents=True, exist_ok=True)

    try:
        # ステップ1: ダウンロード
        mp3_file = download_youtube(url, output_folder, name)

        # ステップ2: 再エンコード
        fixed_file = mp3_file.with_name(mp3_file.stem + "_fixed.mp3")
        if not reencode_mp3(mp3_file, fixed_file):
            messagebox.showerror("エラー", "再エンコードに失敗しました。")
            return
        mp3_file.unlink(missing_ok=True)  # 元のダウンロード版を削除

        # ステップ3: 音量調整
        final_file = output_folder / f"{name}_loud.mp3"
        if adjust_volume(fixed_file, final_file, volume):
            add_id3_tags(final_file, name, artist, album)
            fixed_file.unlink(missing_ok=True)  # 再エンコード版を削除
            messagebox.showinfo("完了", f"処理完了！\n保存先: {final_file}")
        else:
            messagebox.showerror("エラー", "音量調整に失敗しました。")

    except Exception as e:
        messagebox.showerror("エラー", str(e))


# ==========
# UI構築
# ==========
root = tk.Tk()
root.title("YouTube → MP3 (iTunes対応 + 音量調整)")
root.geometry("1000x480")

tk.Label(root, text="YouTube URL:").pack(anchor="w")
url_entry = tk.Entry(root, width=50)
url_entry.pack(fill="x", padx=5)

tk.Label(root, text="保存名 (拡張子不要):").pack(anchor="w")
name_entry = tk.Entry(root, width=30)
name_entry.pack(fill="x", padx=5)

tk.Label(root, text="保存フォルダ:").pack(anchor="w")
output_frame = tk.Frame(root)
output_frame.pack(fill="x", padx=5)
output_entry = tk.Entry(output_frame, width=50)
output_entry.insert(0, str(Path.cwd()))
output_entry.pack(side="left", fill="x", expand=True)
tk.Button(output_frame, text="参照", command=select_output_folder).pack(side="right")

tk.Label(root, text="アーティスト名:").pack(anchor="w")
artist_entry = tk.Entry(root, width=30)
artist_entry.insert(0, "Unknown Artist")
artist_entry.pack(fill="x", padx=5)

tk.Label(root, text="アルバム名:").pack(anchor="w")
album_entry = tk.Entry(root, width=30)
album_entry.insert(0, "YouTube")
album_entry.pack(fill="x", padx=5)

tk.Label(root, text="音量調整 (dB):").pack(anchor="w")
volume_entry = tk.Entry(root, width=10)
volume_entry.insert(0, "6")
volume_entry.pack(fill="x", padx=5)

tk.Button(root, text="ダウンロード & 変換開始", command=start_download, bg="lightblue").pack(pady=20)

root.mainloop()
