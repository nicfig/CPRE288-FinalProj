import pygame, sys
import math
from pygame.locals import (
    K_w,
    K_a,
    K_s,
    K_d,
    K_ESCAPE,
    QUIT,
    KEYDOWN,
)


SCREEN_WIDTH = 800
SCREEN_HEIGHT = 800

pygame.init()

clock = pygame.time.Clock()

screen = pygame.display.set_mode([SCREEN_WIDTH, SCREEN_HEIGHT])

class Player(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super(Player, self).__init__()
        self.player_surface = screen
        self.player_color = (0, 255, 0)
        self.player_radius = 25
        self.x = x
        self.y = y
        self.player_pos = (x, y)
        self.player_width = 0
    
    def move(self):
        self.x += 1
        self.obj_pos = (self.x + 1, self.y)
        self.player_character = pygame.draw.circle(self.player_surface, self.player_color, self.player_pos, self.player_radius, self.player_width)

    def draw(self):
        self.player_character = pygame.draw.circle(self.player_surface, self.player_color, self.player_pos, self.player_radius, self.player_width)


class Obj(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super(Obj, self).__init__()
        self.obj_surface = screen
        self.obj_color = (255, 0, 0)
        self.obj_radius = 25
        self.obj_pos = (x, y)
        self.obj_width = 0
        
    def draw(self):
        self.obj_character = pygame.draw.circle(self.obj_surface, self.obj_color, self.obj_pos, self.obj_radius, self.obj_width)




cybot = Player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)

all_sprites = pygame.sprite.Group()

obj = Obj(100, 100)
all_sprites.add(obj)

x = SCREEN_WIDTH/2
y = SCREEN_HEIGHT/2
vel = 5

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                running = False

    keys = pygame.key.get_pressed()


    if keys[K_w]:
        cybot.move()

    print(cybot.x)

    for x in all_sprites:
        x.draw()


    pygame.display.flip()
    clock.tick(30)


pygame.quit()