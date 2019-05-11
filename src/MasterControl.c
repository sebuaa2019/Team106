
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sensor.h"
#include "MasterControl.h"


int warningStop = 0;


pthread_t ** MasterControl()
{


    /*create thread for infrared sensor*/
    pthread_t * pthread_infrared = (pthread_t*)malloc(sizeof(pthread_t));

    /*create thread for smoke sensor*/
    pthread_t * pthread_smoke = (pthread_t*)malloc(sizeof(pthread_t));

    /*create thread for server*/
    pthread_t * pthread_server = (pthread_t*)malloc(sizeof(pthread_t));

    /*create return array*/
    pthread_t ** pthreadArray = (pthread_t**)malloc(THREAD_NUMBER * sizeof(pthread_t*));

    Sensor();       /*init sensor*/

    if(pthread_create(pthread_infrared, NULL, infraredSensorMonitor, NULL) == -1) {
        printf("create thread infrared failed\n");
    }

    if(pthread_create(pthread_smoke, NULL, smokeSensorMonitor, NULL) == -1) {
        printf("create thread smoke failed\n");
    }

    if(pthread_create(pthread_server, NULL, serverMonitor, NULL) == -1) {
        printf("create thread server failed\n");
    }

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
            warningInfrared();
        }
        else if(sensorStatus == -1) {
            break;
        }
        usleep(INFRARED_READ_INTERVAL);
    }
    return NULL;
}


/*
 * when start,
 * in status 0, if get an abnormal value, goto status 1
 * in status 1, if get a normal value, go back to status 0
 *              if continuously get abnormal value, goto status 2
 * in status 2, if get a normal value, goto status 3
 * in status 3, if get an abnormal value, go back to status 2
 *              if continuously get normal value, goto status 0
 * warning in status 2 and 3
 */
void * smokeSensorMonitor()
{
    int warningStatus = 0;
    /* 0 for waiting,
     * 1 for get first abnormal value, if continue to get abnormal values, show warning
     * 2 for warning status
     * 3 for warning status but get several normal values, in this status, if continuously get normal status, will go back to 0
     */

    int abnormalTime = 0;       /*times of get abnormal value continuously*/
    int normalTime = 0;         /*times of get normal value continuously*/

    while(1) {
        int sensorStatus = getSensorStatus(2);
        if(sensorStatus == -1) {
            break;
            /*error*/
        }
        if(warningStatus == 0) {
            if(sensorStatus == 0) {     /*do nothing*/
                ;   /*do nothing*/
            }
            else if(sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                warningStatus = 1;
                abnormalTime++;
            }
        }
        else if(warningStatus == 1) {
            if(sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                warningStatus = 0;          /*reset warning status */
                abnormalTime = 0;           /* and abnormalTime */
            }
            else if(sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                abnormalTime++;
                if(abnormalTime >= MAX_SMOKE_ABNORMAL_VALUE_CONTINUOUS_TIME) {      /* if get enough abnormal value */
                    warningStatus = 2;          /* set warning status to 2 */
                    abnormalTime = 0;       /* reset abnormalTime */
                }
            }
        }
        else if(warningStatus == 2) {
            if(sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                warningStatus = 3;              /* goto  warning status 3 */
                normalTime++;                   /* normalTime add 1 */
                warningSmoke();         /* warning */
            }
            else if(sensorStatus == 1) {        /* if get an abnormal value */
                warningSmoke();     /* warning */
            }
        }
        else if(warningStatus == 3) {
            if(sensorStatus == 0) {
                normalTime++;
                if(normalTime >= MAX_SMOKE_NORMAL_VALUE_CONTINUOUS_TIME) {  /* if get enough normal value */
                    warningStatus = 0;      /* reset warning status */
                    normalTime = 0;         /* and normalTime */
                }
                warningSmoke();
            }
            else if(sensorStatus == 1) {        /* if get an abnormal value */
                warningStatus = 2;                  /* go back to status 2 */
                normalTime = 0;                     /* reset normalTime */
                warningSmoke();
            }
        }
        usleep(SMOKE_READ_INTERVAL);
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
void warningInfrared()
{
    int i = 0;
    while(i<10) {
        printf("warning infrared\n");
        usleep(ALARM_INTERVAL);
        i++;
    }
}

void warningSmoke()
{
    printf("\t\t\t\t\t\t\t\twarning smoke\n");
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
    else {      /* error */
        return -1;
    }
}