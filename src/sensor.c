//
// Created by noname on 2019-04-26.
//

#include "sensor.h"
#define  infraredErrorThreshold  99
#define  smokeErrorThreshold   86  
int  infraredIndex = 0;
int  smokeIndex = 0;

int infraredSensorStatusValue[100] = {
        7, 49, 73, 58, 30, 72, 44, 78, 23, 9,
        40, 65, 92, 42, 87, 3, 27, 29, 40, 12,
        3, 69, 9, 57, 60, 33, 99, 78, 16, 35,
        97, 26, 12, 67, 10, 33, 79, 49, 79, 21,
        67, 72, 93, 36, 85, 45, 28, 91, 94, 57,
        1, 53, 8, 44, 68, 90, 24, 96, 30, 3,
        22, 66, 49, 24, 1, 53, 77, 8, 28, 33,
        98, 81, 35, 13, 65, 14, 63, 36, 25, 69,
        15, 94, 29, 1, 17, 95, 5, 4, 51, 98,
        88, 23, 5, 82, 52, 66, 16, 37, 38, 44
};
//because there is no sensor now, use this instead of the return value of the sensor

int smokeSensorStatusValue[100] = {
        61, 9, 66, 52, 20, 40, 68, 44, 4, 50,
        40, 5, 28, 78, 7, 86, 86, 86, 86, 86,
        86, 86, 48, 61, 80, 75, 24, 6, 48, 65,
        54, 58, 8, 95, 70, 71, 52, 10, 10, 32,
        65, 40, 28, 28, 15, 0, 28, 82, 76, 28,
        7, 77, 72, 40, 16, 80, 12, 40, 80, 51,
        94, 48, 72, 84, 42, 26, 88, 60, 16, 51,
        0, 12, 20, 89, 75, 84, 91, 88, 75, 9,
        45, 16, 14, 66, 39, 55, 50, 88, 72, 54,
        0, 71, 90, 60, 12, 46, 28, 8, 68, 84
};


void Sensor()
{
       
}


int getInfraredSensorStatus()
{
      return infraredSensorStatusValue[infraredIndex++] >= infraredErrorThreshold;
}


int getSmokeSensorStatus()
{
     return  smokeSensorStatusValue[smokeIndex++] >= smokeErrorThreshold;
}