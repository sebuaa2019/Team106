from django.shortcuts import render
from django.shortcuts import HttpResponse
from rest_framework_jwt.views import ObtainJSONWebToken
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework_jwt.settings import api_settings
from rest_framework import permissions
from Alarmpp.models import User
from Alarm_System import settings
from django.contrib.auth import logout as Logout
from Alarmpp.my_socket import my_socket_server
# Create your views here.
jwt_payload_handler = api_settings.JWT_PAYLOAD_HANDLER
jwt_encode_handler = api_settings.JWT_ENCODE_HANDLER
board_server = my_socket_server('127.0.0.1',9000)
board_server.start()

#account
class test(APIView):
    permission_classes = []
    authentication_classes = []

    def get(self, reuqest):
        res = {
            'msg' : 'testOK',
        }
        return Response(res)

class register(APIView):
    permission_classes = []
    authentication_classes = []

    def post(self, request):
        data = request.data
        username = data.get('username')
        passwprd = data.get('password')
        try:
            user = User.objects.get(username=username)
            status = None
            msg = '用户名已存在'
            token = None
        except:
            User.objects.create(username=username,password=passwprd,phone='456')

            status = None
            msg = '注册成功'
        res = {
            'status': status,
            'msg': msg,
            #'token' : token,
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
            print(username)
            print(user.username)
            if user.password == password:
                status = '登录成功'
                user_id = user.id
                phone = user.phone
                payload = jwt_payload_handler(user)
                token = jwt_encode_handler(payload)
            else:
                status = '密码错误'
                user_id = None
                phone = None
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
        img_path = user.img_path.url
        res = {
            'usernaem' : username,
            'phone' : phone,
            'img_path' : img_path
        }
        pass
        return Response(res)

class img_mod(APIView):
    def post(self, request):
        data = request.data
        img = request.FILES.get('img')
        username = request.user.username
        user = User.objects.get(username=username)
        user.img_path = img
        user.save()
        fname = settings.MEDIA_ROOT + img.name
        with open(fname, 'wb') as pic:
            20
            for c in img.chunks():
                pic.write(c)
        status = 1
        msg = '上传成功'
        res = {
            'status': status,
            'msg': msg
        }
        pass
        return Response(res)

class info_mod(APIView):
    def post(self, request):
        data = request.data
        new_phone = data.get('new_phone')
        new_name = data.get('new_name')
        user = request.user
        user.phone = new_name
        user.name = new_name
        user.save()
        status = 1
        msg = '修改成功'
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
        data = request.data
        on_off = data.get('on_off')
        mode = data.get('mode')
        sensor = data.get('sensor')
        sender = board_server.getsender(request.user.username)
        sender.send(mode)
        status = "test"
        msg = "OK"
        res = {
            'status': status,
            'msg': msg,
        }
        return Response(res)

class settime(APIView):
    def post(self, request):
        data = request.data
        interval = data.get('interval')
        status = None
        msg = None
        res = {
            'status': status,
            'msg': msg,
        }
        pass
        return Response(res)


class record(APIView):
    def post(self, request):
        data = request.data
        sensor = data.get('sensor')
        begin_time = data.get('begin_time')
        end_time = data.get('end_time')
        area = data.get('area')
        info_list = None
        count = None
        res = {
            'info_list' : info_list,
            'count' : count
        }
        return Response(res)

class cancelalarm(APIView):
    def post(self, request):
        data = request.data
        record_id = data.get("record_id")
        status = None
        msg = None
        res = {
            'status' : status,
            'msg' : msg,
        }
        return Response(res)
