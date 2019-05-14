from django.shortcuts import render
from django.shortcuts import HttpResponse
from rest_framework_jwt.views import ObtainJSONWebToken
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework_jwt.settings import api_settings
#from rest_framework.request import Request
from Alarmpp.models import User
# Create your views here.
jwt_payload_handler = api_settings.JWT_PAYLOAD_HANDLER
jwt_encode_handler = api_settings.JWT_ENCODE_HANDLER
#account
class register(APIView):
    permission_classes = []
    authentication_classes = []

    def creat_token(self, user):
        payload = jwt_payload_handler(user)
        token = jwt_encode_handler(payload)
        return token
    def post(self, request):
        data = request.data
        username = data.get('username')
        passwprd = data.get('password')
        try:
            user = User.objects.get(username=username)
            status = None
            msg = '用户名已存在'
        except:
            User.objects.create(username=username,password=passwprd,phone='456')
            user = User.objects.get(username=username)
            token = self.creat_token(user)
            print(token)
            #user.token = token
            #user.save()
            status = None
            msg = '注册成功'
        res = {
            'status': status,
            'msg': msg,
        }
        return Response(res)

class login(ObtainJSONWebToken):

    def post(self, request):
        data = request.data
        username = data.get('username')
        password = data.get('password')
        user = User.objects.get(name=username)
        if user is not None:
            if user.PassWord == password:
                status = '登录成功'
                user_id = user.user_id
                phone = user.Phone
                token = user.token
            else:
                status = '密码错误'
                user_id = None
                phone = None
                token = None
        else:
            status = '用户不存在'
            user_id = None
            phone = None
            token = None
        res = {
            'status': status,
            'user_id': user_id,
            'phone': phone,
            'token' : token,
        }
        return Response(res)

class logout(APIView):
    def post(self, request):
        data = request.data
        status = None
        msg = None
        res = {
            'status': status,
            'msg': msg
        }
        pass
        return Response(res)
        pass

class index(APIView):
    def get(self, request):
        data = request.data
        username = data.get('username')
        user = User.objects.get(Name=username)
        phone = user.Phone
        img_path = data.get('img_path')
        user = User.objects.get(Name=username)

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
        img = data.get('img')
        status = None
        msg = None
        res = {
            'status': status,
            'msg': msg
        }
        pass
        return Response(res)
class info_mod(APIView):
    def post(selfself, request):
        data = request.data
        new_phone = data.get('new_phone')
        new_name = data.get('new_name')

        status = None
        msg = None
        res = {
            'status' : status,
            'msg' : msg
        }
        pass
        return Response(res)

#control
class onoff(APIView):
    def post(self, request):
        data = request.data
        on_off = data.get('on_off')
        mode = data.get('mode')
        area = data.get('area')
        status = None
        msg = None
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

class feedback(APIView):
    def post(self, request):
        data = request.data
        type = data.get('tyoe')
        index = data.get('index')
        date = data.get('date')
        pass
        return Response()

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
        pass
        return Response(res)
