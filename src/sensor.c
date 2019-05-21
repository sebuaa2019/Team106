
#include "sensor.h"
#include <string.h>
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
/*because there is no sensor now, use this instead of the return value of the sensor*/

int smokeSensorStatusValue[100] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        1,  10, 30, 50, 70, 80, 90, 95, 98, 99,
        99, 99, 99, 99, 99, 99, 99, 99, 99, 95,
        90, 88, 86, 80, 50, 40, 30, 30, 30, 30,
        30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
        30, 10, 30, 50, 70, 80, 90, 90, 95, 96,
        99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 90, 85, 80, 50, 30, 20, 10,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


extern int OnSensor(int nID);
extern void SetLight(int nState);
extern void SetCall(char *szPhoneNum, int nType, int nZone);

void Sensor()
{
       
}


int getSensorStatus(int sensorNumber)
{
    return OnSensor(sensorNumber);
}

void turnOnLight()
{
    SetLight(1);
}
void turnOffLight()
{
    SetLight(0);
}

void setPhone(char buf[])
{
    int ptr_buf = 0;
    char tel[20];
    char nType[20];
    char nZone[20];
    int nType_int;
    int nZone_int;
    int i;

    while(buf[ptr_buf] != ' ') {  /* jump sc */
        ptr_buf++;
    }

    while(buf[ptr_buf] == ' ') {  /* jump space between sc and tel */
        ptr_buf++;
    }

    i = 0;
    while(buf[ptr_buf] != ' ') {  /* copy tel */
        tel[i] = buf[ptr_buf];
        i++;
        ptr_buf++;
    }
    tel[i] = 0;

    while(buf[ptr_buf] == ' ') {    /* jump space between tel and nType */
        ptr_buf++;
    }

    i = 0;
    while(buf[ptr_buf] != ' ') { /* copy nType */
        nType[i] = buf[ptr_buf];
        i++;
        ptr_buf++;
    }
    nType[i] = 0;

    while(buf[ptr_buf] == ' ') {     /* jump space between nType and nZone */
        ptr_buf++;
    }

    i = 0;
    while(buf[ptr_buf] != '\0') {   /* copy nZone */
        nZone[i] = buf[ptr_buf];
        i++;
        ptr_buf++;
    }
    nZone[i] = 0;

    nType_int = toInt(nType);
    nZone_int = toInt(nZone);

    SetCall(tel, nType_int, nZone_int);
}

int toInt(char string[])
{
    int i = 0;
    int ret = 0;
    int l = strlen(string);
    while(i < l) {
        ret *= 10;
        ret += string[i] - '0';
        i++;
    }
    return ret;
}

/*
int getInfraredSensorStatus()
{
    infraredIndex++;
    infraredIndex %= 100;
    return infraredSensorStatusValue[infraredIndex] >= infraredErrorThreshold;
}


int getSmokeSensorStatus()
{
    smokeIndex++;
    smokeIndex %= 100;
    return smokeSensorStatusValue[smokeIndex] >= smokeErrorThreshold;
}
*/