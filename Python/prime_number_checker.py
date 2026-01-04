import pyautogui as pag
import sympy as sp

pag.moveTo(850,25)
pag.leftClick(850,25)
pag.press('f11')

#繰り返し処理

pag.moveTo(960,540)
pag.PAUSE = 0.1
pi = sp.pi
num_pi = sp.N(pi)


for i in range(0,314):
    pag.mouseDown()
    sin = sp.sin(i/50)
    cos = sp.cos(i/50)
    pag.moveTo(int(960 + 520*cos),int(540 + 520*sin))

pag.mouseUp()