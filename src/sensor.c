
#include "sensor.h"
#include <string.h>
#define SMOKE_ERROR_THRESHOLD   80
#define TEMPERATURE_ERROR_THRESHOLD 50

extern int OnKey(char  *cKey);
extern void SetLed(int nID,int nState);  /* id=0 greed led; id=1:red led */
extern int OnArm();
extern int OnDisarm();
extern void HomeAlarmIODrvInit();
extern int OnSensor(int nID);
extern void SetLight(int nState);
extern void SetCall(char *szPhoneNum, int nType, int nZone);

void Sensor()
{
    HomeAlarmIODrvInit();
}

int getKey(char  *cKey)
{
    return OnKey(cKey);
}

void setLed(int nID, int nState)
{
    SetLed(nID, nState);
}

int getSensorStatus(int sensorNumber)
{
    if(sensorNumber <= 8) {
        return OnSensor(sensorNumber);
    }
    else if(sensorNumber <= 11){
        return (OnSensor(sensorNumber) >= SMOKE_ERROR_THRESHOLD);
    }
    else {
        return (OnSensor(sensorNumber) >= TEMPERATURE_ERROR_THRESHOLD);
    }
}

void turnOnLight()
{
    SetLight(1);
}
void turnOffLight()
{
    SetLight(0);
}

int isArm()
{
    return OnArm();
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