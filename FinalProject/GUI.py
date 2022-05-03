import pygame
import sys
import math
import os
import socket
from threading import Thread

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


SCREEN_WIDTH = 1150
SCREEN_HEIGHT = 1150

UNIT_MULTIPLIER = 1.3

CYBOT_DIAMETER = 33 * UNIT_MULTIPLIER

black = (0, 0, 0)
red = (255, 0, 0)
green = (0, 255, 0)
purple = (255, 0, 255)
cyan = (0, 255, 255)
blue = (0, 0, 255)
white = (255, 255, 255)
ALPHA = (71, 112, 76)

pygame.init()

clock = pygame.time.Clock()

screen = pygame.display.set_mode([SCREEN_WIDTH, SCREEN_HEIGHT])
DEFAULT_IMAGE_SIZE = (CYBOT_DIAMETER, CYBOT_DIAMETER)


roomba = pygame.image.load(image).convert()

rRotateSPD = 3.95
lRotateSPD = 3.95
spdForward = 0.28
spdBackward = -0.28


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
        self.startx = x
        self.starty = y

    def update(self, action):
        # self.angle = angle
        # nX = (self.startx + (dist * math.cos((self.angle + 90) * math.pi / 180)))
        # nY = (self.starty - (dist * math.sin((self.angle + 90) * math.pi / 180)))
        # # self.rect = self.image.get_rect()
        # # self.rect.center = (nX, nY)
        # self. image = pygame.transform.rotate(self.original_image, self.angle)
        # self.rect.center = (nX, nY)
        if action == 1 or action == 2:
            if action == 1:
                movement = 1
            if action == 2:
                movement = -1
            movement_v = self.direction * movement
            if movement_v.length() > 0:
                movement_v.normalize_ip()
                self.pos += movement_v * spdForward
            self.rect.center = (self.pos)
            # val = val / 10
            # x, y = self.rect.center
            # if val < 0:
            #     nX = math.ceil(x + (val * math.cos((self.angle + 90) * math.pi / 180)))
            #     nY = math.ceil(y - (val * math.sin((self.angle + 90) * math.pi / 180)))
            #     print(nX)
            # else:
            #     nX = math.floor(x + (val * math.cos((self.angle + 90) * math.pi / 180)))
            #     nY = math.floor(y - (val * math.sin((self.angle + 90) * math.pi / 180)))
            #     print(nX)
            # self.rect.center = (nX,nY)
        # # elif action == 2:
        #     self.angle = val
        #     x, y = self.rect.center
        #     self.rect = self.image.get_rect()
        #     self.rect.center = (x, y)
        #     self.image = pygame.transform.rotate(self.original_image, self.angle)
        elif action == 3:
            self.image = pygame.transform.rotate(
                self.original_image, self.angle)
            # Value will reapeat after 359. This prevents angle to overflow.
            self.angle = (self.angle + lRotateSPD) % 360
            x, y = self.rect.center  # Save its current center.
            self.direction.rotate_ip(-lRotateSPD)
            # Replace old rect with new rect.
            self.rect = self.image.get_rect()
            # Put the new rect's center at old center.
            self.rect.center = (x, y)
        elif action == 4:
            self.image = pygame.transform.rotate(
                self.original_image, self.angle)
            # Value will reapeat after 359. This prevents angle to overflow.
            self.angle = (self.angle - rRotateSPD) % 360
            x, y = self.rect.center  # Save its current center.
            # Replace old rect with new rect.
            self.direction.rotate_ip(rRotateSPD)
            self.rect = self.image.get_rect()
            # Put the new rect's center at old center.
            self.rect.center = (x, y)


class Obj(pygame.sprite.Sprite):
    def __init__(self, pos, radius, color):
        super(Obj, self).__init__()
        self.obj_surface = screen
        self.obj_color = color
        self.obj_radius = radius
        self.obj_pos = pos
        self.obj_width = 0

    def draw(self):
        pygame.draw.circle(self.obj_surface, self.obj_color, self.obj_pos, self.obj_radius, self.obj_width)



class rect(pygame.sprite.Sprite):
    def __init__(self, x, y, size, color):
        super(rect, self).__init__()
        self.surf = screen
        self.color = color
        self.size = size
        self.x = x
        self.y = y

    def draw(self):
        pygame.draw.rect(self.surf, self.color, pygame.Rect(self.x, self.y, self.size, self.size))

cybot = Player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)

all_sprites = pygame.sprite.Group()

x = SCREEN_WIDTH/2
y = SCREEN_HEIGHT/2

res = ""
            
startTheta = 0
distSum = 0
isObj = False
def recieve():
    global isObj
    global startTheta
    global distSum
    while True:
        res = ""
        data = s.recv(1024)
        if not data:
            break
        res += data.decode()
        x, y = cybot.rect.center
        print(res)
        if 'v1' in res:
            px = x + ((CYBOT_DIAMETER / 2) * math.cos((cybot.angle + (180 - 30)) * math.pi / 180))
            py = y - ((CYBOT_DIAMETER / 2) * math.sin((cybot.angle + (180 - 30)) * math.pi / 180))
            all_sprites.add(rect(px, py, 5, white))
        if 'v2' in res:
            px = x + ((CYBOT_DIAMETER / 2) * math.cos((cybot.angle + (180 - 75)) * math.pi / 180))
            py = y - ((CYBOT_DIAMETER / 2) * math.sin((cybot.angle + (180 - 75)) * math.pi / 180))
            all_sprites.add(rect(px, py, 5, white))
        if 'v3' in res:
            px = x + ((CYBOT_DIAMETER / 2) * math.cos((cybot.angle + -(105 - 180)) * math.pi / 180))
            py = y - ((CYBOT_DIAMETER / 2) * math.sin((cybot.angle + -(105 - 180)) * math.pi / 180))
            all_sprites.add(rect(px, py, 5, white))
        if 'v4' in res:
            px = x + ((CYBOT_DIAMETER / 2) * math.cos((cybot.angle + -(150 - 180)) * math.pi / 180))
            py = y - ((CYBOT_DIAMETER / 2) * math.sin((cybot.angle + -(150 - 180)) * math.pi / 180))
            all_sprites.add(rect(px, py, 5, white))

        if 'c1' in res:
            px = x + ((CYBOT_DIAMETER / 2) * math.cos((cybot.angle + (180 - 30)) * math.pi / 180))
            py = y - ((CYBOT_DIAMETER / 2) * math.sin((cybot.angle + (180 - 30)) * math.pi / 180))
            all_sprites.add(rect(px, py, 5, cyan))
        if 'c2' in res:
            px = x + ((CYBOT_DIAMETER / 2) * math.cos((cybot.angle + (180 - 75)) * math.pi / 180))
            py = y - ((CYBOT_DIAMETER / 2) * math.sin((cybot.angle + (180 - 75)) * math.pi / 180))
            all_sprites.add(rect(px, py, 5, cyan))
        if 'c3' in res:
            px = x + ((CYBOT_DIAMETER / 2) * math.cos((cybot.angle + -(105 - 180)) * math.pi / 180))
            py = y - ((CYBOT_DIAMETER / 2) * math.sin((cybot.angle + -(105 - 180)) * math.pi / 180))
            all_sprites.add(rect(px, py, 5, cyan))
        if 'c4' in res:
            px = x + ((CYBOT_DIAMETER / 2) * math.cos((cybot.angle + -(150 - 180)) * math.pi / 180))
            py = y - ((CYBOT_DIAMETER / 2) * math.sin((cybot.angle + -(150 - 180)) * math.pi / 180))
            all_sprites.add(rect(px, py, 5, cyan))

        if res[0] == 'm' and (len(res) < 50):
            res = res.strip('m')
            angle, dist = (int(float(s)) for s in res.split())

            if angle <= 90:
                agl = (180 - angle)
            elif angle > 90:
                agl = -(angle - 180)
            if dist < 50:
                pos = (x + ((dist + 8) * math.cos((cybot.angle + agl) * math.pi / 180)), y - ((dist + 8) * math.sin((cybot.angle + agl) * math.pi / 180)))
                obj = Obj(pos, 1, purple)
                obj.draw()
                all_sprites.add(obj)
                if(isObj == False):
                    startTheta = angle
                    isObj = True
                distSum += dist

            elif isObj == True and ((angle - 1) - startTheta) > 1:
                obj_angleSum = (angle-1) - startTheta
                obj_avg_dist = ((distSum / obj_angleSum) + 8) * UNIT_MULTIPLIER
                width = 2 * math.pi * obj_avg_dist * (obj_angleSum/360)
                obj_angle = (startTheta + (angle-1)) / 2
                if obj_angle <= 90:
                    obj_angle = (180 - obj_angle)
                elif angle > 90:
                    obj_angle = -(obj_angle - 180)

                pos = (x + (obj_avg_dist * math.cos((cybot.angle + obj_angle) * math.pi / 180)), y - (obj_avg_dist * math.sin((cybot.angle + obj_angle) * math.pi / 180)))

                if (width < 17):
                    obj = Obj(pos, (width / 2) * UNIT_MULTIPLIER, green)
                else:
                    obj = Obj(pos, (width / 2) * UNIT_MULTIPLIER, red)
                obj.draw()
                all_sprites.add(obj) 
                distSum = 0
                count = 0
                isObj = False
            else:
                distSum = 0
                count = 0
                isObj = False   
        elif 'w' in res:
            cybot.update(1)
        elif 's' in res:
            cybot.update(2)
        elif 'a' in res:
            cybot.update(3)
        elif 'd' in res:
            cybot.update(4)
action = 0
threads = list()
thread = Thread(target=recieve, args=(), daemon=True)
thread.start()
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
        s.send(bytes(' ', 'utf-8'))
    elif keys[K_w]:
        s.send(bytes('w', 'utf-8'))
    elif keys[K_s]:
        s.send(bytes('s', 'utf-8'))
    elif keys[K_a]:
        s.send(bytes('a', 'utf-8'))
    elif keys[K_d]:
        s.send(bytes('d', 'utf-8'))
    else:
        s.send(bytes(' ', 'utf-8'))
    screen.blit(cybot.image, cybot.rect)

    for x in all_sprites:
        x.draw()
    pygame.display.flip()
    pygame.display.update()
    clock.tick(30)


s.close()
pygame.quit()
