import threading
import socket
import time
from rest_framework.views import APIView

class my_sender(threading.Thread):
    def __init__(self, conn, addr):
        threading.Thread.__init__(self)
        self.conn = conn
        self.addr = addr
    def send(self, str):
        self.conn.send(str)

class my_reciver(threading.Thread):
    def __init__(self, conn, addr):
        threading.Thread.__init__(self)
        self.conn = conn
        self.addr = addr
    def run(self):
        while(True):
            data = self.conn.recv(1024).decode()


class my_socket_server(threading.Thread):
    __host = "14.115.160.42"
    __port = 3333

    def __init__(self, host, port):
        threading.Thread.__init__(self)
        self.sockets = {}
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((host, port))
        self.server.listen(5)

    def run(self):
        while True:
            conn, addr = self.server.accept()
            t_recv = my_reciver(conn, addr)
            t_send = my_sender(conn, addr)
            t_recv.start()
            t_send.start()
            self.sockets['0'] = (t_recv, t_send)

    def getsender(self, user):
        return self.sockets[user][0]

    def getreciver(self, user):
        return self.sockets[user][1]