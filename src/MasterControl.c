
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <loglib.h>
#include <sockLib.h>
#include <netinet/in.h>
#include <inetLib.h>
#include <stddef.h>
#include <time.h>
#include <tasklib.h>
#include "sensor.h"
#include "MasterControl.h"


int warningStop = 0;
int * retThread;

int infraredStatus = 1;         /* 0 for closed, 1 for open */
int smokeStatus = 1;            /* 0 for closed, 1 for open */

int programEnd = 0;

int infraredWarning = 0;        /* if the infrared is warning */
int smokeWarning = 0;           /* if the smoke is warning */

int * MasterControl()
{
    /*create return array*/
    retThread = (int*)malloc(sizeof(int) * THREAD_NUMBER);
    Sensor();       /*init sensor*/

    if((retThread[0] = taskSpawn("infrared", 200, 0, 100000, (FUNCPTR)infraredSensorMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread infrared failed\n");
    }

    if((retThread[1] = taskSpawn("smoke", 200, 0, 100000, (FUNCPTR)smokeSensorMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread smoke failed\n");
    }

    if((retThread[2] = taskSpawn("server", 200, 0, 100000, (FUNCPTR)serverMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread server failed\n");
    }

    return retThread;
}


void * infraredSensorMonitor()
{
    while(1) {
        if(infraredStatus == 1) {
            int sensorStatus = getSensorStatus(1);
            if (sensorStatus == 0) {
                ;
            }
            else if (sensorStatus == 1) {
                infraredWarning = 1;
                warningInfrared();
                infraredWarning = 0;
            }
            else if (sensorStatus == -1) {
                break;
            }
        }
        vxsleep(INFRARED_READ_INTERVAL);
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
        if(smokeStatus == 1) {
            int sensorStatus = getSensorStatus(2);
            if (sensorStatus == -1) {
                break;
                /*error*/
            }
            if (warningStatus == 0) {
                smokeWarning = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    ;   /*do nothing*/
                } else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus = 1;
                    abnormalTime++;
                }
            } else if (warningStatus == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus = 0;          /*reset warning status */
                    abnormalTime = 0;           /* and abnormalTime */
                } else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime++;
                    if (abnormalTime >=
                        MAX_SMOKE_ABNORMAL_VALUE_CONTINUOUS_TIME) {      /* if get enough abnormal value */
                        warningStatus = 2;          /* set warning status to 2 */
                        abnormalTime = 0;       /* reset abnormalTime */
                    }
                }
            } else if (warningStatus == 2) {
                smokeWarning = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus = 3;              /* goto  warning status 3 */
                    normalTime++;                   /* normalTime add 1 */
                    warningSmoke();         /* warning */
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningSmoke();     /* warning */
                }
            } else if (warningStatus == 3) {
                if (sensorStatus == 0) {
                    normalTime++;
                    if (normalTime >= MAX_SMOKE_NORMAL_VALUE_CONTINUOUS_TIME) {  /* if get enough normal value */
                        warningStatus = 0;      /* reset warning status */
                        normalTime = 0;         /* and normalTime */
                    }
                    warningSmoke();
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus = 2;                  /* go back to status 2 */
                    normalTime = 0;                     /* reset normalTime */
                    warningSmoke();
                }
            }
        }
        vxsleep(SMOKE_READ_INTERVAL);
    }
    return NULL;
}

/*
 * pi: pause infrared sensor
 * ri: restart
 * ps: pause smoke sensor
 * rs: restart
 * q: exit the program
 */
void * serverMonitor()
{
    struct sockaddr_in serAddr;
    int len;
    char ip[]="192.168.0.14";
    int sockFd, rLen;
    char buf[255] = "";

    int lastInfraredWarning = 0;   /* record the value of infraredWarning when read it last time */
    int lastSmokeWarning = 0;

    len = sizeof(serAddr);

    vxsleep(100);

    /* create socket */
    sockFd = socket(AF_INET,SOCK_STREAM,0);

    /* set the net which the socket tend to bind */
    serAddr.sin_family=AF_INET;             /* ip protocol */
    serAddr.sin_port=8888;                  /* port */
    inet_aton(ip, &(serAddr.sin_addr));     /*change ip to 32 bit integer*/
    memset(serAddr.sin_zero, 0, 8);

    if (connect(sockFd, (struct sockaddr*)&serAddr, len)==OK) {
        printf("[client]Connected\n");
    }

    memset(buf, 0, sizeof(buf));

    if(taskSpawn("serverRead", 200, 0, 100000, (FUNCPTR)serverRead, sockFd, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
        printf("serverRead create failed\n");
    }

    while(1) {
        /*
         * if the value of infrared/smoke warning change to 1, send warning message
         * if the value of infrared/smoke warning change to 0, do nothing
         */
        if(infraredWarning != lastInfraredWarning) {
            if(infraredWarning == 1) {
                lastInfraredWarning = infraredWarning;
                strcpy(buf, "wi");
                write(sockFd, buf, sizeof(buf));
            }
            else if(infraredWarning == 0) {
                lastInfraredWarning = infraredWarning;
            }
        }
        if(smokeWarning != lastSmokeWarning) {
            if(smokeWarning == 1) {
                lastSmokeWarning = smokeWarning;
                strcpy(buf, "ws");
                write(sockFd, buf, sizeof(buf));
            }
            else if(smokeWarning == 0) {
                lastSmokeWarning = smokeWarning;
            }
        }

        vxsleep(1000);
    }
    return NULL;
}

void * serverRead(int sockFd)
{
    char buf[255];
    int rLen;

    while(1) {
        memset(buf, 0, sizeof(buf));
        rLen = read(sockFd, buf, sizeof(buf));
        if(rLen == 0) {
            break;
        }
        if(strcmp(buf, "pi") == 0) {
            infraredStatus = 0;
        }
        else if(strcmp(buf, "ps") == 0) {
            smokeStatus = 0;
        }
        else if(strcmp(buf, "ri") == 0) {
            infraredStatus = 1;
        }
        else if(strcmp(buf, "rs") == 0) {
            smokeStatus = 1;
        }
        else if(strcmp(buf, "q") == 0) {
            programEnd = 1;
        }
        else {
            printf("undefined command: %s\n", buf);
        }
    }
}

/*
 * warn function
 */
void warningInfrared()
{
    int i = 0;
    while(i<10) {
        printf("warning infrared\n");
        vxsleep(ALARM_INTERVAL);
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








void vxsleep(long int ms)
{
    int m = sysClkRateGet();   /*get core frequency*/
    m = 1000/m;
    m = ms/m + 1;    /* taskDelay(n) in fact delay (n-1)tick~n*tick*/
    taskDelay(m);
}