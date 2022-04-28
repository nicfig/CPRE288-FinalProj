import pygame
import sys
import math
import os
import socket

from pygame.locals import (
    K_w,
    K_a,
    K_s,
    K_d,
    K_m,
    K_ESCAPE,
    QUIT,
    KEYDOWN,
)
THIS_FOLDER = os.path.dirname(os.path.abspath(__file__))
image = os.path.join(THIS_FOLDER, 'roomba.png')

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.1.1', 288))


SCREEN_WIDTH = 1000
SCREEN_HEIGHT = 800

black = (0, 0, 0)
red = (255, 0, 0)
green = (0, 255, 0)
blue = (0, 0, 255)
white = (255, 255, 255)
ALPHA = (71, 112, 76)

pygame.init()

clock = pygame.time.Clock()

screen = pygame.display.set_mode([SCREEN_WIDTH, SCREEN_HEIGHT])
DEFAULT_IMAGE_SIZE = (80, 80)


roomba = pygame.image.load(image).convert()


class Player(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super(Player, self).__init__()
        self.original_image = pygame.transform.scale(
            roomba, DEFAULT_IMAGE_SIZE)
        self.original_image.convert_alpha()  # optimise alpha
        self.original_image.set_colorkey(ALPHA)  # set alpha
        self.image = self.original_image
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)
        self.angle = 0
        self.pos = pygame.Vector2((x, y))
        self.direction = pygame.Vector2((0, -1))

    def update(self, v):
        if v == 1 or v == 2:
            if v == 1:
                movement = 1
            if v == 2:
                movement = -1
            movement_v = self.direction * movement
            if movement_v.length() > 0:
                movement_v.normalize_ip()
                self.pos += movement_v
            self.rect.center = (self.pos)
        elif v == 3:
            self.image = pygame.transform.rotate(
                self.original_image, self.angle)
            # Value will reapeat after 359. This prevents angle to overflow.
            self.angle = (self.angle + 1) % 360
            print(self.angle)
            x, y = self.rect.center  # Save its current center.
            self.direction.rotate_ip(-1)
            # Replace old rect with new rect.
            self.rect = self.image.get_rect()
            # Put the new rect's center at old center.
            self.rect.center = (x, y)
        elif v == 4:
            self.image = pygame.transform.rotate(
                self.original_image, self.angle)
            # Value will reapeat after 359. This prevents angle to overflow.
            self.angle = (self.angle - 1) % 360
            print(self.angle)
            x, y = self.rect.center  # Save its current center.
            # Replace old rect with new rect.
            self.direction.rotate_ip(1)
            self.rect = self.image.get_rect()
            # Put the new rect's center at old center.
            self.rect.center = (x, y)


class Obj(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super(Obj, self).__init__()
        self.obj_surface = screen
        self.obj_color = red
        self.obj_radius = 25
        self.obj_pos = (x, y)
        self.obj_width = 0

    def draw(self):
        self.obj_character = pygame.draw.circle(
            self.obj_surface, self.obj_color, self.obj_pos, self.obj_radius, self.obj_width)


cybot = Player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)

all_sprites = pygame.sprite.Group()

obj = Obj(100, 100)
all_sprites.add(obj)

x = SCREEN_WIDTH/2
y = SCREEN_HEIGHT/2


# def scan():
#     for x in range(0, 180):
#         data = s.recv(1024)
#         print(data)
#         pygame.display.flip()
#         pygame.display.update()
#         screen.blit(cybot.image, cybot.rect)
#         clock.tick(30)


running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                running = False

    keys = pygame.key.get_pressed()

    screen.fill(black)

    if keys[K_m]:
        s.send(bytes('m', 'utf-8'))
    elif keys[K_w]:
        cybot.update(1)
        s.send(bytes('w', 'utf-8'))
    elif keys[K_s]:
        cybot.update(2)
        s.send(bytes('s', 'utf-8'))
    elif keys[K_a]:
        cybot.update(3)
        s.send(bytes('a', 'utf-8'))
        data = s.recv(1024)
        print(data)
    elif keys[K_d]:
        cybot.update(4)
        s.send(bytes('d', 'utf-8'))
    else:
        s.send(bytes(' ', 'utf-8'))
    screen.blit(cybot.image, cybot.rect)

    data = s.recv(1024)
    print(data)

    pygame.display.flip()
    pygame.display.update()
    clock.tick(30)

s.close()
pygame.quit()
