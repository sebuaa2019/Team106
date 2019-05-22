#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：client.py

import socket  # 导入 socket 模块
import time
import threading
import datetime
import  os
import socket


def tcplinkrec(conn, addr,s2):
    print("Accept new connection from %s:%s" % addr)
    #conn.send(b"Welcome!\n")
    while True:
        #conn.send(b"What's your name?")
        data = conn.recv(1024).decode()
        print('from PPC:'+data+'\n')
        s2.send(data.encode())
        #if data == "exit":
          #  conn.send(b"Good bye!\n")
        #    break
        #conn.send(b"Hello %s!\n" % data.encode())
    conn.close()
    print("Connection from %s:%s is closed" % addr)


def tcplinksend(conn,addr,s2):
    while True:
        data = s2.recv(1024).decode()
        print('from WEB:'+data+'\n')
        if data=='p0' or data=='p1':
            data = 'pd'
        elif data =='p2' or data =='p3':
            data = 'pi'
        elif data == 'p4' or data =='p5':
            data = 'pw'
        elif data == 'p6' or data =='p7':
            data = 'ps'
        elif data == 'p8' or data == 'p9':
            data = 'pt'
        elif data=='r0' or data=='r1':
            data = 'rd'
        elif data =='r2' or data =='r3':
            data = 'ri'
        elif data == 'r4' or data =='r5':
            data = 'rw'
        elif data == 'r6' or data =='r7':
            data = 'rs'
        elif data == 'r8' or data == 'r9':
            data = 'rt'
        conn.send(data.encode())
    s2.close()


#rdpath = 'C://Users//chenshang//Desktop//serverWithPPC//PPC2WEB.txt'
#wtpath = 'C://Users//chenshang//Desktop//serverWithPPC//WEB2PPC.txt'

#时间戳到时间
def TimeStampToTime(timestamp):
    timeStruct = time.localtime(timestamp)
    return time.strftime('%Y-%m-%d %H:%M:%S',timeStruct)
#获取文件大小,KB
def get_FileSize(filePath):
    #filePath = str(filePath,'utf8')
    fsize = os.path.getsize(filePath)
    #fsize = fsize/float(1024)
    return fsize

#获取文件访问时间 access
def get_FileAccessTime(filePath):
    #filePath = str(filePath,'utf8')
    t = os.path.getatime(filePath)
    return TimeStampToTime(t)

#获取文件创建时间
def get_FileCreateTime(filePath):
    #filePath = str(filePath,'utf8')
    t = os.path.getctime(filePath)
    return TimeStampToTime(t)

#获取文件修改时间
def get_FileModifyTime(filePath):
    #filePath = str(filePath,'utf8')
    t = os.path.getmtime(filePath)
    return int(t)

# s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)# 创建 socket 对象
# host = socket.gethostname() # 获取本地主机名
# port = 12345                # 设置端口号
# print('客户端名称=',host,'客户端端口=',port)
#
# client_send_data='客户端发送字符：client send '.encode('utf-8')
# client_receive_data=''
#
# s.connect((host, port))
# print('客户端开始发送数据...')
# s.send(client_send_data)
# print('客户端接收数据=', s.recv(1024).decode('utf-8'))
# s.close()

def sendClient(s): #send line
    while(1):
        if os.path.getsize(rdpath): #文件非空
            time_mod = get_FileModifyTime(rdpath)
            time_cur = time.time()
            if time_cur != time_mod:
                rdf = open(rdpath, "r")
                line = rdf.readline()
                rdf.close()
                rdf = open(rdpath,"w")
                rdf.truncate()
                rdf.close()
                print(line)
                s.send(line.encode())
        time.sleep(1)
def client():
    #s1 with PPC
    s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = socket.gethostname()
    print(host)
    # addr = ['100.125.1.250', '100.125.21.250', '192.168.0.1', '192.168.15.254', '192.168.8.3', "114.116.48.152"]
    # for i in addr:
    # time.sleep(0.2)
    # host = input('please host:')
    # port = int(input('please port:'))
    print('stat...', s1)
    s1.bind(("192.168.0.14", 8888))
    if s1:
        print('ok')
    else:
        print('ng')
    s1.listen(5)
    print("Waiting for connection...")

    conn, addr = s1.accept()

    #s2 with WEB
    s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s2.connect(("114.115.160.42", 9000))
    data = "lipu"
    data = data.encode()
    s2.send(data)

    t1 = threading.Thread(target=tcplinkrec, args=(conn, addr,s2,))
    t1.start()
    t2 = threading.Thread(target=tcplinksend, args=(conn, addr,s2,))
    t2.start()

    # t = threading.Thread(target=sendClient,args=(s,))
    # t.start()

    # while True:   #receive line
    #     data = s.recv(1024).decode()
    #     print(data)
    #     wtf = open(wtpath, "w")
    #     if data=="pi":
    #         wtf.write("pi" + '\n')
    #     elif data=="ps":
    #         wtf.write("ps" + '\n')
    #     elif data=="ri":
    #         wtf.write("ri" + '\n')
    #     elif data=="rs":
    #         wtf.write("rs" + '\n')
    #     wtf.close()
    #     time.sleep(1)
    s.close()

if __name__=="__main__":
    client()