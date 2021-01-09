import socket
import time

HOST = '192.168.0.177'  # The server's hostname or IP address
PORT = 5001        # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    for x in range(0, 5):
        s.sendall(b'w121\n')
        data = s.recv(5)
        print('Received', repr(data))
        s.sendall(b'r120\n')
        data = s.recv(5)
        print('Received', repr(data))
        time.sleep(1.0)
        s.sendall(b'w120\n')
        data = s.recv(5)
        print('Received', repr(data))
        s.sendall(b'r120\n')
        data = s.recv(5)
        print('Received', repr(data))
        time.sleep(1.0)
    s.close();

