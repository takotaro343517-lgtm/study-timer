import pygame

pygame.init()

screen = pygame.display.set_mode((800,600))
pygame.display.set_caption("Invaders Game")
#screen.fill((150,150,150))

##img = pygame.image.load('')

# Player
Playerimg = pygame.image.load('player_small2.png')
PlayerX, PlayerY = 370, 480
PlayerX_change = 0

def player(x,y):
    screen.blit(Playerimg, (x, y))

running = True
while running:
    screen.fill((0, 0, 0))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:#Aキー
                playerX_change = -20
            if event.key == pygame.K_RIGHT:#Dキー
                playerX_change = 20
#            if event.key == pygame.K_SPACE:
#               if bullet_state is 'ready':
#                    bulletX = playerX
#                    fire_bullet(bulletX, bulletY)

        if event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT:
                playerX_change = 0

    PlayerX += PlayerX_change
    if PlayerX <= 0:
        PlayerX = 0
    elif PlayerX >= 736:
        PlayerX = 736
    player(PlayerX, PlayerY)

    pygame.display.update()


#font = pygame.font.SysFont(None, 50)
#message = font.render('Hello World', False,(255,255,255))
#screen.blit(message, (20, 50))
