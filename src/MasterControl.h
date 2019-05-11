

#ifndef SOFTWARE_ENGINEERING_MASTERCONTROL_H
#define SOFTWARE_ENGINEERING_MASTERCONTROL_H


#include <pthread.h>


#define THREAD_NUMBER 3
#define ALARM_INTERVAL 1000        /* 10^3 msec       //Alarm interval */
#define INFRARED_READ_INTERVAL 1000   /* 10^3 msec            //Read infrared sensor interval */
#define SMOKE_READ_INTERVAL 1000   /* 10^3 msec               //Read smoke sensor interval */
#define MAX_SMOKE_ABNORMAL_VALUE_CONTINUOUS_TIME  8         /* Alarm after reading MAX_SMOKE_ABNORMAL_VALUE_CONTINUOUS_TIME abnormal values from smoke sensor continuously */
#define MAX_SMOKE_NORMAL_VALUE_CONTINUOUS_TIME  8         /* stop alarm after reading MAX_SMOKE_NORMAL_VALUE_CONTINUOUS_TIME normal values from smoke sensor continuously */


int getSensorStatus(int sensorNumber);      /* 1 for infrared, 2 for smoke */
int turnOnSensor(int sensorNumber);
int turnOffSensor(int sensorNumber);
int getSensorSwitchStatus(int sensorNumber);
int getSensorError(int sensorNumber);
int turnOnSystem();
int turnOffSystem();
int getInstrFromServer();

void * infraredSensorMonitor();
void * smokeSensorMonitor();
void * serverMonitor();

void warningInfrared();
void warningSmoke();
void sendMessageToServer(int ip, int message);
int * MasterControl();

void vxsleep(long int ms);         /* sleep ms milliseconds */

#endif /* SOFTWARE_ENGINEERING_MASTERCONTROL_H */
