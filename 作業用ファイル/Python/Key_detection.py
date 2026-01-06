import pygame
pygame.init()

KEYS = {pygame.K_a:False,pygame.K_b:False,pygame.K_c:False,pygame.K_d:False,pygame.K_e:False,pygame.K_f:False,pygame.K_g:False,pygame.K_h:False,
        pygame.K_i:False,pygame.K_j:False,pygame.K_k:False,pygame.K_l:False,pygame.K_m:False,pygame.K_m:False,pygame.K_n:False,pygame.K_o:False,
        pygame.K_p:False,pygame.K_q:False,pygame.K_r:False,pygame.K_s:False,pygame.K_t:False,pygame.K_u:False,pygame.K_v:False,pygame.K_w:False,
        pygame.K_x:False,pygame.K_y:False,pygame.K_z:False,
        pygame.K_1:False,pygame.K_2:False,pygame.K_3:False,pygame.K_4:False,pygame.K_5:False,pygame.K_6:False,pygame.K_7:False,pygame.K_8:False,
        pygame.K_9:False,pygame.K_0:False,
        pygame.K_UP:False,pygame.K_RIGHT:False,pygame.K_LEFT:False,pygame.K_DOWN:False,
        pygame.K_SPACE:False,pygame.K_RSHIFT:False,pygame.K_LSHIFT:False,pygame.K_RCTRL:False,pygame.K_LCTRL:False,
        pygame.K_RALT:False,pygame.K_LALT:False,pygame.K_ESCAPE:False}

def key_detection(event):
    if event.type == pygame.KEYDOWN:
        KEYS[event.key] =  True
    elif event.type == pygame.KEYUP:
        KEYS[event.key] = False

running = True
screen = pygame.display.set_mode((800,600))
while (running):
    for events in pygame.event.get():
        if events.type == pygame.QUIT:
            running = False
        key_detection(events)
    print(KEYS[pygame.K_RIGHT])
    print(KEYS[pygame.K_LEFT])
    print(KEYS[pygame.K_UP])
    print(KEYS[pygame.K_DOWN])