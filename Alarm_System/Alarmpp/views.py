from django.shortcuts import render
from django.shortcuts import HttpResponse
from rest_framework_jwt.views import ObtainJSONWebToken
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework_jwt.settings import api_settings
from rest_framework import permissions
from Alarmpp.models import User, Record
from Alarm_System import settings
from django.contrib.auth import logout as Logout
from Alarmpp.my_socket import my_socket_server
# Create your views here.
jwt_payload_handler = api_settings.JWT_PAYLOAD_HANDLER
jwt_encode_handler = api_settings.JWT_ENCODE_HANDLER
class runserver(APIView):
    run = False
    def get(self, request):
        global board_server
        if not self.run:
            board_server = my_socket_server("127.0.0.1", 9000)
            board_server.start()
        res = {
            "msg" : self.run
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
            status = 1
            msg = '用户名已存在'
        except:
            User.objects.create(username=username,password=passwprd,email=email)
            status = 0
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
                status = 0
                user_id = user.id
                email = user.email
                payload = jwt_payload_handler(user)
                token = jwt_encode_handler(payload)
            else:
                status = 1
                user_id = None
                email = None
                token = None
        except:
            status = '用户不存在'
            user_id = None
            phone = None
            token = None

        res = {
            'status': status,
            'user_id': user_id,
            'phone': phone,
            'token': token,
        }
        return Response(res)

class logout(APIView):
    def get(self, request):
        Logout(request)
        return Response({'name': request.user.username})

class index(APIView):
    def post(self, request):
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
            status = 1
            msg = '修改成功'
        except:
            status = 0
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
            if on_off == 1:
                on_off = "r"
                msg = "开启"
            else:
                on_off = "p"
                msg = "关闭"
            data = str(on_off+sensor_id).encode()
            sender = board_server.getsender(request.user.username)
            sender.send(data)
            status = 0
        except:
            status = 1
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
            sensor_type = int(data.get('sensor'))
            date = data.get('date')
            records = Record.objects.get(date=date)
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