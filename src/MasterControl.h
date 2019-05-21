

#ifndef SOFTWARE_ENGINEERING_MASTERCONTROL_H
#define SOFTWARE_ENGINEERING_MASTERCONTROL_H


#include <pthread.h>


#define THREAD_NUMBER 6

#define ALARM_INTERVAL 1000        /* 10^3 msec       //Alarm interval */
#define DOOR_READ_INTERVAL 1000
#define INFRARED_READ_INTERVAL 1000   /* 10^3 msec            //Read infrared sensor interval */
#define WATER_READ_INTERVAL 1000
#define SMOKE_READ_INTERVAL 1000   /* 10^3 msec               //Read smoke sensor interval */
#define TEMPERATURE_READ_INTERVAL 1000

#define MAX_SMOKE_ABNORMAL_VALUE_CONTINUOUS_TIME  8         /* Alarm after reading MAX_SMOKE_ABNORMAL_VALUE_CONTINUOUS_TIME abnormal values from smoke sensor continuously */
#define MAX_SMOKE_NORMAL_VALUE_CONTINUOUS_TIME  8         /* stop alarm after reading MAX_SMOKE_NORMAL_VALUE_CONTINUOUS_TIME normal values from smoke sensor continuously */
#define MAX_TEMPERATURE_ABNORMAL_VALUE_CONTINUOUS_TIME 8
#define MAX_TEMPERATURE_NORMAL_VALUE_CONTINUOUS_TIME 8




/* monitors */
void * doorSensorMonitor();
void * infraredSensorMonitor();
void * waterSensorMonitor();
void * smokeSensorMonitor();
void * temperatureSensorMonitor();
void * serverMonitor();
void * serverRead();

/* warning functions */
void warningDoor(int i);
void warningInfrared(int i);
void warningWater(int i);
void warningSmoke(int i);
void warningTemperature(int i);

void * warningInfraredThread(int i);
void * warningDoorThread(int i);
void * warningWaterThread(int i);

void sendMessageToServer(int ip, int message);
int * MasterControl();

void vxsleep(long int ms);         /* sleep ms milliseconds */

#endif /* SOFTWARE_ENGINEERING_MASTERCONTROL_H */
