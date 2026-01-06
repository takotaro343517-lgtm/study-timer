import pygame
from pygame.locals import *
import sys
import numpy as np

screen  = pygame.display.set_mode((1300, 720))
function_list =[
    np.sin,
    np.exp,
    lambda x:x**2
]
block_x=140
block_y=207

def draw_block():
    block_color = (255,0,0)
    block_width , block_height = (140,207)
    font = pygame.font.Font(None, 50)
    #card_1 = font.render(function_list[0],True, (0,0,0))
    for i in range(7):
        pygame.draw.rect(screen,block_color,Rect(157 +i*141,513,block_width,block_height),2)
    #screen.blit(card_1, Rect(218,600,block_x,block_y))



def main():
    pygame.init()                                 # Pygameの初期化

    block_x=140
    block_y=207
    x = y = None


    while True:
        screen.fill((255,255,255))                                     # 背景を白
        draw_block()
        pygame.draw.rect(screen, (255,0,0), Rect(10,256,block_x,block_y))    # ■
        pygame.draw.rect(screen, (255,0,0), Rect(1150,256,block_x,block_y))
        pygame.draw.rect(screen, (255,0,0), Rect(161,720-207*2-10,block_x *7,block_y))
        pygame.draw.rect(screen, (255,0,0), Rect(161,720-207*3-10*2,block_x *7,block_y))
        #pygame.draw.line(screen, (0,255,0), (0,200), (100,300), 2)    # 線
        pygame.display.update()                                       # 画面更新

        # イベント処理
        for event in pygame.event.get():  
            if event.type == QUIT:        
                pygame.quit()             
                sys.exit()                
            elif event.type == MOUSEBUTTONDOWN and event.button == 1:
                x , y = event.pos
                print(x,y)

        if x is not None and y is not None:
            if 513 <= y <= 720:
                if 157 <= x <= 297:    
                    print("１のカードを認識しました")
                if 298 <= x <= 438:
                    print("２のカードを認識しました")
                if 299 <= x <= 439:
                    print("３のカードを認識しました")
                if 440 <= x <= 580:
                    print("４のカードを認識しました")
                if 581 <= x <= 721:
                    print("５のカードを認識しました")
                if 722 <= x <= 862:
                    print("６のカードを認識しました")
                if 863 <= x <= 1003:
                    print("７のカードを認識しました")
                
            x = y = None
            

if __name__ == "__main__":
    main()
