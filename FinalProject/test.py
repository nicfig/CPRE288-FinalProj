import socket

s = socket.socket()
s.connect(('192.168.1.1', 15200))

s.send('m')

s.close()
