import socket

string = "m 34 60"

char = string[0]
if char == 'm':
    string = string.strip('m')
    angle, dist = (int(float(s)) for s in string.split())
    print(angle, dist)
