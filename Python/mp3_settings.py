import subprocess
import os
from pathlib import Path
from mutagen.easyid3 import EasyID3
from mutagen.mp3 import MP3
import datetime

def reencode_mp3(input_path, output_path):
    """ffmpegを使ってMP3を再エンコード"""
    result = subprocess.run([
        "ffmpeg",
        "-y",
        "-i", str(input_path),
        "-codec:a", "libmp3lame",
        "-qscale:a", "2",
        str(output_path)
    ], capture_output=True, text=True)

    return result.returncode == 0

def add_id3_tags(mp3_path):
    """title, artist, album を自動で追加（推測）"""
    try:
        title = mp3_path.stem.replace("_fixed", "")
        audio = MP3(mp3_path, ID3=EasyID3)
        audio["title"] = title
        audio["artist"] = "HIKAKIN & SEIKIN"
        audio["album"] = "YouTube"
        audio.save()
        print(f"  🎵 タグ追加完了: {title}")
    except Exception as e:
        print(f"  ❌ タグ追加失敗: {e}")

def process_all_mp3s(folder_path):
    folder = Path(folder_path)
    if not folder.exists():
        print("❌ 指定フォルダが存在しません")
        return

    mp3_files = list(folder.glob("*.mp3"))

    if not mp3_files:
        print("❗ MP3ファイルが見つかりません")
        return

    for mp3 in mp3_files:
        if mp3.name.endswith("_fixed.mp3"):
            continue  # すでに変換済みならスキップ

        fixed_path = mp3.with_name(mp3.stem + "_fixed.mp3")
        print(f"▶ 処理中: {mp3.name}")

        if reencode_mp3(mp3, fixed_path):
            print(f"  ✅ 再エンコード成功: {fixed_path.name}")
            add_id3_tags(fixed_path)
        else:
            print(f"  ❌ エンコード失敗: {mp3.name}")

# メイン処理
if __name__ == "__main__":
    import sys
    
    if len(sys.argv) != 2:
        print("使い方: python mp3_settings.py <フォルダパス>")
    else:
        process_all_mp3s(sys.argv[1])


#python C:\Users\tetsu\Desktop\孝太郎の使うfile\孝太郎のデータ\VSCode\Python\mp3_settings.py "C:/Users/tetsu/Desktop/孝太郎の使うfile/孝太郎のデータ/MP3/musics"
