import sys
import tkinter as tk
import yt_dlp
from pathlib import Path

source = "C:/Users/tetsu/Desktop/孝太郎の使うfile/孝太郎のデータ/MP3/musics"
directory_path = Path(source)

def button_clicked():
    
    print(f'you typed {url_entry.get()}')
    output_folder = f'{source}'
    print(output_folder)
    
    opts = {
    'format':'bestaudio/best',
    'extractaudio' : True,
    'audio-format' : "mp3",
    "audio-quality" : "192K",
    'outtmpl':f'{output_folder}/{name_entry.get()}.mp3'
    }
    print("download start...")
    with yt_dlp.YoutubeDL(opts) as ydl:
        ydl.download([url_entry.get()])
    print("download finished!")
    

WIDTH,HEIGHT = 800,500
root = tk.Tk()
root.title("Youtube file downloader")
root.geometry(f'{str(WIDTH)}x{str(HEIGHT)}')
root.resizable(False,False)
root.configure(bg="lightblue")


download_menu = tk.Frame(root,width=WIDTH,height=HEIGHT,bg="white")


'''
download menu
'''


Title = tk.Label(download_menu,text="Youtube mp3 downloader")
Title.grid(row=0,column=0,columnspan=2)

d_1 = tk.Label(download_menu,text="download URL")
d_1.grid(row=1,column=0)

url_entry = tk.Entry(download_menu,width=30,font=("Arial",14))
url_entry.grid(row=1,column=1)

d_2 = tk.Label(download_menu,text="Name")
d_2.grid(row=2,column=0)

name_entry = tk.Entry(download_menu,width=30,font=("Arial",14))
name_entry.grid(row=2,column=1)

button = tk.Button(download_menu,text="confirm",command=button_clicked,bg="blue",fg="white")
button.grid(row=4,column=0,columnspan=2)

download_menu.pack(fill=tk.BOTH, expand=False)

root.mainloop()