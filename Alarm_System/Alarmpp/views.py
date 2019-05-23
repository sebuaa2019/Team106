from django.shortcuts import render
from django.contrib.auth import logout as Logout
from rest_framework_jwt.views import ObtainJSONWebToken
from rest_framework_jwt.settings import api_settings
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import permissions
from Alarm_System import settings
from Alarmpp.models import User, Record, Sensor
from Alarmpp.my_socket import my_socket_server
from Alarmpp.my_enum import Sensortype, Status, Onoff

# Create your views here.
jwt_payload_handler = api_settings.JWT_PAYLOAD_HANDLER
jwt_encode_handler = api_settings.JWT_ENCODE_HANDLER
__runserver = False
if not __runserver:
    board_server = my_socket_server("127.0.0.1", 9000)
    board_server.start()
    __runserver = board_server
class runserver(APIView):
    run = False
    def get(self, request):
        global board_server
        if not self.run:
            board_server = my_socket_server("127.0.0.1", 9000)
            board_server.start()
        res = {
            "msg" : "runserver"
        }
        return Response(res)
#account
class test(APIView):
    permission_classes = []
    authentication_classes = []

    def get(self, reuqest):
        res = {
            'msg' : 'server connect OK',
        }
        return Response(res)

class register(APIView):
    permission_classes = []
    authentication_classes = []

    def post(self, request):
        data = request.data
        username = data.get('username')
        passwprd = data.get('password')
        email    = data.get('email')
        try:
            user = User.objects.get(username=username)
            status = Status.Fail
            msg = '用户名已存在'
        except:
            User.objects.create(username=username,password=passwprd,email=email)
            status = Status.Success
            msg = '注册成功'
        res = {
            'status': status,
            'msg': msg,
        }
        return Response(res)

class login(ObtainJSONWebToken):
    def post(self, request, *args, **kwargs):
        data = request.data
        username = data.get('username')
        password = data.get('password')
        print(username)
        try:
            user = User.objects.get(username=username)
            if user.password == password:
                status = Status.Success
                user_id = user.id
                email = user.email
                payload = jwt_payload_handler(user)
                token = jwt_encode_handler(payload)
            else:
                status = Status.Fail
                user_id = None
                email = None
                token = None
        except:
            status = Status.Fail
            user_id = None
            email = None
            token = None

        res = {
            'status': status,
            'user_id': user_id,
            'email': email,
            'token': token,
        }
        return Response(res)

class logout(APIView):
    def get(self, request):
        Logout(request)
        return Response({'name': request.user.username})

class index(APIView):
    def get(self, request):
        username = request.user.username
        user = User.objects.get(username=username)
        phone = user.phone
        name  = user.name
        res = {
            'name' : name,
            'phone' : phone,
        }
        return Response(res)

class info_mod(APIView):
    def post(self, request):
        try:
            data = request.data
            new_phone = data.get('new_phone')
            new_name = data.get('new_name')
            new_email  = data.get('new_email')
            user = User.objects.get(username=request.user.username)
            user.phone = new_phone
            user.name = new_name
            user.email = new_email
            user.save()
            status = Status.Success
            msg = '修改成功'
        except:
            status = Status.Fail
            msg = '修改失败'
        res = {
            'status' : status,
            'msg' : msg
        }
        return Response(res)

class freshtoken(APIView):
    def post(self, request):
        username = request.user.username
        user = User.objects.get(username=username)
        payload = jwt_payload_handler(user)
        token = jwt_encode_handler(payload)
        return Response({'token', token})
#control
class onoff(APIView):
    def post(self, request):
        msg = ""
        try:
            data = request.data
            on_off = int(data.get('on_off'))
            sensor_id = data.get('sensor_id')
            if on_off == Onoff.On:
                switch = "r"
                msg = "开启"
            else:
                switch = "p"
                msg = "关闭"
            data = str(switch+sensor_id).encode()
            sender = board_server.getsender(request.user.username)
            sender.send(data)
            status = Status.Success
            sensor = Sensor.objects.get(sensor_id=sensor_id)
            sensor.switch = on_off
            sensor.save()
            msg = msg+"成功"
        except:
            status = Status.Fail
            msg = msg+"失败"
        res = {
            'status': status,
            'msg': msg,
        }
        return Response(res)

class record(APIView):
    def post(self, request):
        info_list = []
        try:
            data = request.data
            sensor_type = int(data.get('sensor_type'))
            date = data.get('date')
            records = Record.objects.get(date=date, sensor_type=sensor_type)
            for record in records:
                r = {
                    "record_id" : record.sensor_id,
                    "category" : record.type,
                    "time" : record.time
                }
                info_list.append(r)
            count = len(info_list)
        except:
            count = 0
        res = {
            'info_list' : info_list,
            'count' : count
        }
        return Response(res)