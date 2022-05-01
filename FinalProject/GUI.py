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
DEFAULT_IMAGE_SIZE = (25, 25)


roomba = pygame.image.load(image).convert()

rRotateSPD = 1.6
lRotateSPD = 1.6
spdForward = 1
spdBackward = -1


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
                movement = spdForward
            if action == 2:
                movement = spdBackward
            movement_v = self.direction * movement
            if movement_v.length() > 0:
                movement_v.normalize_ip()
                self.pos += movement_v
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
    def __init__(self, pos, radius):
        super(Obj, self).__init__()
        self.obj_surface = screen
        self.obj_color = red
        self.obj_radius = radius
        self.obj_pos = pos
        self.obj_width = 0

    def draw(self):
        self.obj_character = pygame.draw.circle(
            self.obj_surface, self.obj_color, self.obj_pos, self.obj_radius, self.obj_width)


cybot = Player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)

all_sprites = pygame.sprite.Group()

x = SCREEN_WIDTH/2
y = SCREEN_HEIGHT/2

res = ""

def processData(dists):
    isObj = False
    objDist = []
    objTheta = []
    objWidth = []
    startTheta = 0
    distSum = 0
    count = 0
    for i, dist in enumerate(dists):
        if dist < 50:
            if(isObj == False):
                startTheta = i
                isObj = True
            distSum += dist
            count += 1

        elif isObj == True and (count > 1):
            obj_angle = (i-1) - startTheta
            obj_avg_dist = distSum / count
            width = 2 * math.pi * obj_avg_dist * (obj_angle/360)

            objDist.append(obj_avg_dist)
            objWidth.append(width)
            objTheta.append((startTheta + (i-1)) / 2)
            distSum = 0
            count = 0
            isObj = False
        else:
            distSum = 0
            count = 0
            isObj = False

    if isObj == True and (count > 1):
        obj_angle = len(dists) - startTheta
        obj_avg_dist = distSum / count
        width = 2 * math.pi * obj_avg_dist * (obj_angle/360)
        objDist.append(obj_avg_dist)
        objWidth.append(width)
        objTheta.append((startTheta + len(dists)) / 2)


    x, y = cybot.rect.center
    for i in range(0, len(objDist)):
        if objTheta[i] <= 90:
            objTheta[i]= (180 - objTheta[i])
        elif objTheta[i] > 90:
            objTheta[i] = -(objTheta[i] - 180)

        pos = (x + (objDist[i] * math.cos((cybot.angle + objTheta[i]) * math.pi / 180)), y - (objDist[i] * math.sin((cybot.angle + objTheta[i]) * math.pi / 180)))
        obj = Obj(pos, objWidth[i])
        all_sprites.add(obj)

        
            



def recieve():
    angles = []
    dists = []
    while True:
        res = ""
        data = s.recv(1024)
        if not data:
            break
        res += data.decode()
        if res[0] == 'm':
            res = res.strip('m')
            angle, dist = (int(float(s)) for s in res.split())
            dists.append(dist)
            if (len(dists) == 180):
                processData(dists)
                dists.clear()
            # x, y = cybot.rect.center
            # if angle <= 90:
            #     angle = (180 - angle)
            # elif angle > 90:
            #     angle = -(angle - 180)
            # pos = (x + (dist * math.cos((cybot.angle + angle) * math.pi / 180)),
            #        y - (dist * math.sin((cybot.angle + angle) * math.pi / 180)))
            # if dist < 50:
            #     obj = Obj(pos)
            #     all_sprites.add(obj)
        elif res[0] == 'd':
            res = res.strip('d')
            dist, angle = (int(float(s)) for s in res.split())
            # cybot.update(dist, angle)
        # elif res[0] == 't':
        #     res = res.strip('t')
        #     cybot.update(2, int(float(res)))
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
    if keys[K_w]:
        s.send(bytes('w', 'utf-8'))
        cybot.update(1)
    elif keys[K_s]:
        s.send(bytes('s', 'utf-8'))
        cybot.update(2)
    elif keys[K_a]:
        s.send(bytes('a', 'utf-8'))
        cybot.update(3)
    elif keys[K_d]:
        s.send(bytes('d', 'utf-8'))
        cybot.update(4)
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
