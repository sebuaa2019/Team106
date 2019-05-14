from django.db import models
from django.contrib.auth.models import AbstractUser
# Create your models here.

class User(AbstractUser):
    #user_id  = models.AutoField(primary_key=True)
    #username = models.CharField(default='Anonymous', max_length=32)
    #password = models.CharField(default='111111', max_length=32)
    phone    = models.CharField(max_length=20, unique=True)
    token    = models.CharField(max_length=64)

class PPC(models.Model):
    PPC_id      = models.AutoField(primary_key=True)
    address = models.CharField(max_length=128, default='Anonymous')
    type    = models.IntegerField(default=0)
    status  = models.IntegerField(default=0)
    switch  = models.BooleanField(default=False)
    manager = models.IntegerField(default=0)

class Manager(models.Model):
    manager_id     = models.AutoField(primary_key=True)
    title  = models.CharField(max_length=128, default='初级')
    detail = models.CharField(max_length=128)
    phone  = models.CharField(max_length=20)

class Sensor(models.Model):
    sensor_id        = models.AutoField(primary_key=True)
    type      = models.IntegerField(default=0)
    status    = models.IntegerField(default=0)
    address   = models.CharField(max_length=32, default='客厅')
    necessity = models.BooleanField(default=False)

class Control(models.Model):
    control_id = models.IntegerField()
    PPC_id     = models.IntegerField()

class Load(models.Model):
    PPC_id    = models.IntegerField()
    sensor_id = models.IntegerField()
    switch    = models.BooleanField()

class Record(models.Model):
    record_id = models.AutoField(primary_key=True)
    time      = models.DateTimeField()
    type      = models.IntegerField()
    area      = models.CharField(max_length=32)