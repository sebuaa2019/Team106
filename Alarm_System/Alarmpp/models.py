from django.db import models
from django.contrib.auth.models import AbstractUser
# Create your models here.

class User(AbstractUser):
    phone = models.CharField(max_length=20, unique=True)
    name = models.CharField(max_length=20)
    img_path = models.ImageField(upload_to='img')

class Control(models.Model):
    user_id    = models.IntegerField()
    PPC_id     = models.IntegerField()

class Load(models.Model):
    PPC_id    = models.IntegerField()
    sensor_id = models.IntegerField()

class Record(models.Model):
    PPC_id    = models.IntegerField()
    date      = models.DateTimeField()
    time      = models.IntegerField()
    type      = models.IntegerField()
