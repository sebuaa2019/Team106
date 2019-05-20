#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：client.py

import socket  # 导入 socket 模块
import time
import threading
import datetime
import  os


rdpath = 'C://Users//chenshang//Desktop//serverWithPPC//PPC2WEB.txt'
wtpath = 'C://Users//chenshang//Desktop//serverWithPPC//WEB2PPC.txt'

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

def sendClient(): #send line
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
                line = line[:-1]
                print(line)
                s.send(line.encode())
        time.sleep(1)
def client():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print(socket.gethostname())
    #print(get_FileModifyTime('C://Users//chenshang//Desktop//testPipes//test.txt'))
    s.connect(("114.115.160.42", 3333))
    print(s.recv(1024).decode())
    data = "PPC106"
    data = data.encode()
    s.send(data)
    t = threading.Thread(target=sendClient,args=(s,))
    t.start()

    #data = "client"
    while True:   #receive line
        data = s.recv(1024).decode()
        print(data)
        wtf = open(wtpath, "w")
        if data=="pi":
            wtf.write("pi" + '\n')
        elif data=="ps":
            wtf.write("ps" + '\n')
        elif data=="ri":
            wtf.write("ri" + '\n')
        elif data=="rs":
            wtf.write("rs" + '\n')
        wtf.close()
        time.sleep(1)
        #if data: print(s.recv(1024).decode())

        #data = input("Please input your name: ")
        #if not data: continue
        #s.send(data.encode())

        #print(s.recv(1024).decode())
        #if data == "exit": break
    s.close()

if __name__=="__main__":
    client()