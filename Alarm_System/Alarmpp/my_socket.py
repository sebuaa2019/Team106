import threading
import socket
from Alarmpp.models import Sensor, Record, User
import datetime
import time as Time
from django.core.mail import send_mail
from django.conf import settings

class my_sender(threading.Thread):
    def __init__(self, conn, addr):
        threading.Thread.__init__(self)
        self.conn = conn
        self.addr = addr
    def send(self, str):
        self.conn.send(str)

class my_reciver(threading.Thread):
    def __init__(self, conn, addr, username):
        threading.Thread.__init__(self)
        self.conn = conn
        self.addr = addr
        self.username = username
    def sensor_id2text(self, sensor_id):
        sensor = Sensor.objects.get(sensor_id=sensor_id)
        area = sensor.area
        type = str(sensor.type)
        text = "报警"
        text = area+type+text
    def run(self):
        while(True):
            try:
                data = self.conn.recv(1024).decode()
                sensor_id = int(data)
                date = datetime.date.today()
                time = int(Time.time())
                sensor = Sensor.objects.get(sensor_id= sensor_id)
                type   = sensor.type
                Record.objects.create(sensor_id=sensor_id, date=date, time=time, type=type)
                title = "震惊！你不家的时候竟然发生..."
                text = self.sensor_id2text(sensor_id)
                sender = settings.EMAIL_FROM
                reciver = user = User.objects.get(username=self.username)
                send_mail(title, text, sender, reciver)
            except:
                pass

class my_socket_server(threading.Thread):
    __host = "14.115.160.42"
    __port = 3333

    def __init__(self, host, port):
        threading.Thread.__init__(self)
        self.sockets = {}
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((host, port))
        self.server.listen(5)
        print("waiting....")

    def run(self):
        while True:
            conn, addr = self.server.accept()
            username = conn.recv(1024).decode()
            print("connect",addr, username)
            t_recv = my_reciver(conn, addr, username)
            t_send = my_sender(conn, addr)
            t_recv.start()
            t_send.start()
            self.sockets[username] = (t_recv, t_send)

    def getsender(self, username):
        return self.sockets[username][1]

    def getreciver(self, username):
        return self.sockets[username][0]