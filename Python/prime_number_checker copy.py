#import pyautogui as pag
#import math
#
#pag.moveTo(850,25)
#pag.click(850,25)
#pag.press('f11')
#
#center_x, center_y = 960, 540
#r = 520
#
#pag.moveTo(center_x + r, center_y)
#pag.mouseDown()   # ← ループ外に移動
#
#for i in range(63):
#    x = int(center_x + r * math.cos(i/10))
#    y = int(center_y + r * math.sin(i/10))
#    pag.moveTo(x, y, duration=0.24)   # 一瞬で移動
#
#pag.mouseUp()

import pyautogui as pag
import numpy as np
import time

center = [960,540]
fineness = 60
radius = 500
pag.moveTo(850,25)
pag.leftClick(850,25)
pag.press('f11')
pag.moveTo(center[0] + radius * 2 * np.pi,
           center[1])
pag.mouseDown()
time.sleep(3)
for i in range(fineness):
    pag.moveTo(center[0] + radius * np.cos((i / fineness) * 2 * np.pi),
               center[1] + radius * np.sin((i / fineness) * 2 * np.pi))