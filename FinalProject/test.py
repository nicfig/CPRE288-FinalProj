import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.1.1', 288))

s.send(bytes('m', 'utf-8'))

s.close()
