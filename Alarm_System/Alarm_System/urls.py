"""Alarm_System URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from Alarmpp import views
from rest_framework import routers

urlpatterns = [
    path('admin/', admin.site.urls),
    #account
    path('api/account/register/', views.register.as_view()),
    path('api/account/login/', views.login.as_view()),
    path('api/account/logout/', views.logout.as_view()),
    path('api/account/index/',views.index.as_view()),
    path('api/account/img_mod', views.img_mod.as_view()),
    path('api/account/info_mod/', views.info_mod.as_view()),
    #control
    path('api/control/onoff/', views.onoff.as_view()),
    path('api/control/settime', views.settime.as_view()),
    path('api/control/feedback/', views.feedback.as_view()),
    path('api/control/record/', views.record.as_view())
]
