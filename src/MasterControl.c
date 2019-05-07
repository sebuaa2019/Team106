//
// Created by noname on 2019-04-26.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zconf.h>
#include "sensor.h"
#include "MasterControl.h"


int warningStop = 0;


pthread_t ** MasterControl()
{
    Sensor();       //init sensor

    //create thread for infrared sensor
    pthread_t * pthread_infrared = (pthread_t*)malloc(sizeof(pthread_t));
    if(pthread_create(pthread_infrared, NULL, infraredSensorMonitor, NULL) == -1) {
        printf("create thread infrared failed\n");
    }

    //create thread for smoke sensor
    pthread_t * pthread_smoke = (pthread_t*)malloc(sizeof(pthread_t));
    if(pthread_create(pthread_smoke, NULL, smokeSensorMonitor, NULL) == -1) {
        printf("create thread smoke failed\n");
    }

    pthread_t * pthread_server = (pthread_t*)malloc(sizeof(pthread_t));
    if(pthread_create(pthread_server, NULL, serverMonitor, NULL) == -1) {
        printf("create thread server failed\n");
    }


    pthread_t ** pthreadArray = (pthread_t**)malloc(THREAD_NUMBER * sizeof(pthread_t*));
    pthreadArray[0] = pthread_infrared;
    pthreadArray[1] = pthread_smoke;
    pthreadArray[2] = pthread_server;

    return pthreadArray;
}


void * infraredSensorMonitor()
{
    while(1) {
        int sensorStatus = getSensorStatus(1);
        if(sensorStatus == 0) {
            ;
        }
        else if(sensorStatus == 1) {
            warning();
        }
        else if(sensorStatus == -1) {
            break;
        }
        sleep(1);
    }
    return NULL;
}

void * smokeSensorMonitor()
{
    while(1) {
        int sensorStatus = getSensorStatus(2);
        sleep(1);
    }
    return NULL;
}

void * serverMonitor()
{
    while(1) {
        sleep(1);
    }
    return NULL;
}


/*
 * warn function
 */
void warning()
{
    int i = 0;
    while(i<10) {
        printf("warning\n");
        sleep(ALARM_INTERVAL);
        i++;
    }
}

/*
 * sensorNumber: 1 for infrared, 2 for smoke
 */
int getSensorStatus(int sensorNumber)
{
    if(sensorNumber == 1) {
        return getInfraredSensorStatus();
    }
    else if(sensorNumber == 2) {
        return getSmokeSensorStatus();
    }
    else {      //error
        return -1;
    }
}