from django.db import models
from django.contrib.auth.models import AbstractUser
# Create your models here.

class User(AbstractUser):
    phone = models.CharField(max_length=20, unique=True)
    name = models.CharField(max_length=20)
    img_path = models.ImageField(upload_to='img')

class PPC(models.Model):
    address = models.CharField(max_length=128, default='Anonymous')
    type    = models.IntegerField(default=0)
    status  = models.IntegerField(default=0)
    switch  = models.BooleanField(default=False)
    manager = models.IntegerField(default=0)

class Manager(models.Model):
    title  = models.CharField(max_length=128, default='初级')
    detail = models.CharField(max_length=128)
    phone  = models.CharField(max_length=20)

class Sensor(models.Model):
    type      = models.IntegerField(default=0)
    status    = models.IntegerField(default=0)
    address   = models.CharField(max_length=32, default='客厅')
    necessity = models.BooleanField(default=False)

class Control(models.Model):
    PPC_id     = models.IntegerField()

class Load(models.Model):
    PPC_id    = models.IntegerField()
    sensor_id = models.IntegerField()
    switch    = models.BooleanField()

class Record(models.Model):
    user_id = models.IntegerField()
    sensor_id = models.IntegerField()
    time    = models.DateTimeField()
    type    = models.IntegerField()
    area    = models.CharField(max_length=32)