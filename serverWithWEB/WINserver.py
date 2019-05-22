#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：server.py
# import op
import socket  # 导入 socket 模块

# s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)         # 创建 socket 对象
# host = socket.gethostname() # 获取本地主机名
# port = 12345                # 设置端口
# s.bind((host, port))        # 绑定端口
#
# s.listen(5)                 # 等待客户端连接
# print('服务器主机名=',host,'服务器端口=',port)
#
# server_send_data='服务器发送字符：server send '.encode('utf-8')
# server_receive_data=''
#
# while True:
#     c, addr = s.accept()     # 建立客户端连接。
#     print( '连接地址：', addr)
#     print('服务器开始发送数据...')
#     c.send(server_send_data)
#     server_receive_data=c.recv(1024).decode('utf-8')
#     print('服务器接收数据=',server_receive_data)
#     c.close()                # 关闭连接

import socket
import threading, time


def tcplinkrec(conn, addr,conn2):
    print("Accept new connection from %s:%s" % addr)
    #conn.send(b"Welcome!\n")
    while True:
        #conn.send(b"What's your name?")
        data = conn.recv(1024).decode()
        print(data+'\n')

        #if data == "exit":
          #  conn.send(b"Good bye!\n")
        #    break
        #conn.send(b"Hello %s!\n" % data.encode())
    conn.close()
    print("Connection from %s:%s is closed" % addr)


def tcplinksend(conn,addr,conn2):
    while True:
        data = input()
        conn.send(data.encode())
        time.sleep(1)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = socket.gethostname()
print(host)
#addr = ['100.125.1.250', '100.125.21.250', '192.168.0.1', '192.168.15.254', '192.168.8.3', "114.116.48.152"]
# for i in addr:
# time.sleep(0.2)
#host = input('please host:')
#port = int(input('please port:'))
print('stat...', s)
s.bind(("192.168.0.14", 8888))
if s:
    print('ok')
else:
    print('ng')
s.listen(5)
print("Waiting for connection...")

while True:
    conn, addr = s.accept()
    t1 = threading.Thread(target=tcplinkrec, args=(conn, addr))
    t1.start()
    t2 = threading.Thread(target=tcplinksend, args=(conn, addr))
    t2.start()