from django.db import models
from django.contrib.auth.models import AbstractUser
# Create your models here.

class User(AbstractUser):
    phone = models.CharField(max_length=20, default="null")
    name = models.CharField(max_length=20)

class Sensor(models.Model):
    sensor_id = models.IntegerField(primary_key=True)
    type      = models.IntegerField()
    area      = models.CharField(max_length=20)
    switch    = models.IntegerField(default=0)

class Control(models.Model):
    user_id    = models.IntegerField()
    PPC_id     = models.IntegerField()

class Load(models.Model):
    PPC_id    = models.IntegerField()
    sensor_id = models.IntegerField()

class Record(models.Model):
    record_id = models.AutoField(primary_key=True)
    sensor_id = models.IntegerField()
    date      = models.DateTimeField()
    time      = models.IntegerField()
    type      = models.IntegerField()
