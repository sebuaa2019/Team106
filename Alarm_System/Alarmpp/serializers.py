from rest_framework import serializers
from Alarmpp import  models

class userserializer(serializers.ModelSerializer):
    class Meta:
        model = models.User
        #fields = ()