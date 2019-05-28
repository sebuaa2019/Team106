from enum import Enum
class Sensortype(Enum):
    Door = 0
    Infrared = 1
    Water = 2
    Smoke = 3
    Temperature = 4

class Recordtype(Enum):
    Door = 0
    Infrared = 1
    Water = 2
    Smoke = 3
    Temperature = 4

class Onoff():
    On = 1
    Off = 0

class Status():
    Success = 0
    Fail = 1