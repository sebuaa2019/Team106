//
// Created by noname on 2019-04-26.
//

#ifndef SOFTWARE_ENGINEERING_MASTERCONTROL_H
#define SOFTWARE_ENGINEERING_MASTERCONTROL_H


#include <pthread.h>


#define THREAD_NUMBER 3


int getSensorStatus(int sensorNumber);      //1 for infrared, 2 for smoke
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

void warning();
void sendMessageToServer(int ip, int message);
pthread_t ** MasterControl();



#endif //SOFTWARE_ENGINEERING_MASTERCONTROL_H
