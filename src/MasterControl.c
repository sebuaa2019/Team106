//
// Created by noname on 2019-04-26.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zconf.h>
#include "sensor.h"
#include "MasterControl.h"



pthread_t ** MasterControl()
{
    Sensor();       //init sensor

    pthread_t * pthread1 = (pthread_t*)malloc(sizeof(pthread_t));
    if(pthread_create(pthread1, NULL, sensorMonitor, NULL) == -1) {
        printf("create thread1 failed\n");
    }

    pthread_t * pthread2 = (pthread_t*)malloc(sizeof(pthread_t));
    if(pthread_create(pthread2, NULL, serverMonitor, NULL) == -1) {
        printf("create thread2 failed\n");
    }

//    pthread_join(*pthread1, NULL);
//    pthread_join(*pthread2, NULL);

    pthread_t ** pthreadArray = (pthread_t**)malloc(sizeof(pthread_t*));
    pthreadArray[0] = pthread1;
    pthreadArray[1] = pthread2;
    return pthreadArray;
}


void * sensorMonitor()
{
    while(1) {
        printf("1\n");
        sleep(1);
    }
    return NULL;
}

void * serverMonitor()
{
    while(1) {
        printf("2\n");
        sleep(1);
    }
    return NULL;
}