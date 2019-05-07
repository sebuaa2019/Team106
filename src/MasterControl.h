//
// Created by noname on 2019-04-26.
//

#ifndef SOFTWARE_ENGINEERING_MASTERCONTROL_H
#define SOFTWARE_ENGINEERING_MASTERCONTROL_H

#include <pthread.h>


int getSensorStatus(int sensorNumber);
int turnOnSensor(int sensorNumber);
int turnOffSensor(int sensorNumber);
int getSensorSwitchStatus(int sensorNumber);
int getSensorError(int sensorNumber);
int turnOnSystem();
int turnOffSystem();
int getInstrFromServer();

void * sensorMonitor();
void * serverMonitor();

void warning();
void sendMessageToServer(int ip, int message);
pthread_t ** MasterControl();



#endif //SOFTWARE_ENGINEERING_MASTERCONTROL_H
