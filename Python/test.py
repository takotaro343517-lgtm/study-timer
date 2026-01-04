import pygame
import numpy as np

array = np.array([1,2,3])
print(array)

pygame.init()

# 画面の設定
screen_width = 800
screen_height = 600
screen = pygame.display.set_mode((screen_width, screen_height))

# プレイヤーの初期位置
player_x = screen_width // 2
player_y = screen_height // 2

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                player_x -= 10
            elif event.key == pygame.K_RIGHT:
                player_x += 10
            elif event.key == pygame.K_UP:
                player_y -= 10
            elif event.key == pygame.K_DOWN:
                player_y += 10

    # 画面のクリア
    screen.fill((0, 0, 0))

    # プレイヤーの描画
    pygame.draw.rect(screen, (255, 0, 0), (player_x, player_y, 50, 50))

    pygame.display.flip()

pygame.quit()
