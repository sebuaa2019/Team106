
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

/* record sensor on/off status */
int doorStatus = 1;             /* 0 for off, 1 for on */
int infraredStatus = 1;         /* 0 for off, 1 for on */
int waterStatus = 1;            /* 0 for off, 1 for on */
int smokeStatus = 1;            /* 0 for off, 1 for on */
int temperatureStatus = 1;      /* 0 for off, 1 for on */

/* record if the program is set to exit */
int programEnd = 0;

/* record if the sensors should warning now */
int doorWarning1 = 0;
int doorWarning2 = 0;
int infraredWarning1 = 0;       /* if the infrared is warning */
int infraredWarning2 = 0;
int waterWarning1 = 0;
int waterWarning2 = 0;
int smokeWarning1 = 0;          /* if the smoke is warning */
int smokeWarning2 = 0;
int temperatureWarning1 = 0;
int temperatureWarning2 = 0;

/* the main function of MasterControl */
int * MasterControl()
{
    /*create return array*/
    retThread = (int*)malloc(sizeof(int) * THREAD_NUMBER);
    Sensor();       /*init sensor*/
    
    if((retThread[0] = taskSpawn("door", 200, 0, 100000, (FUNCPTR)doorSensorMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread door failed\n");
    }

    if((retThread[1] = taskSpawn("infrared", 200, 0, 100000, (FUNCPTR)infraredSensorMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread infrared failed\n");
    }
    
    if((retThread[2] = taskSpawn("water", 200, 0, 100000, (FUNCPTR)waterSensorMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread water failed\n");
    }

    if((retThread[3] = taskSpawn("smoke", 200, 0, 100000, (FUNCPTR)smokeSensorMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread smoke failed\n");
    }
    
    if((retThread[4] = taskSpawn("temperature", 200, 0, 100000, (FUNCPTR)temperatureSensorMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread temperature failed\n");
    }

    if((retThread[5] = taskSpawn("server", 200, 0, 100000, (FUNCPTR)serverMonitor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        printf("create thread server failed\n");
    }

    return retThread;
}

/* the monitor of door sensor */
void * doorSensorMonitor()
{
    while(1) {
        if(doorStatus == 1) {
            int sensorStatus = getSensorStatus(0);
            if(sensorStatus == 0) {
                ;
            }
            else if(sensorStatus == 1) {
                doorWarning1 = 1;
                warningDoor(1);
            }
            else if(sensorStatus == -1) {
                break;
            }

            sensorStatus = getSensorStatus(1);
            if(sensorStatus == 0) {
                ;
            }
            else if(sensorStatus == 1) {
                doorWarning2 = 1;
                warningDoor(2);
            }
            else if(sensorStatus == -1) {
                break;
            }
        }
        vxsleep(DOOR_READ_INTERVAL);
    }
}

/* the monitor of infrared sensor */
void * infraredSensorMonitor()
{
    while(1) {
        if(infraredStatus == 1) {
            int sensorStatus = getSensorStatus(3);
            if (sensorStatus == 0) {
                ;
            }
            else if (sensorStatus == 1) {
                infraredWarning1 = 1;
                warningInfrared(1);
                /* infraredWarning1 = 0; */
            }
            else if (sensorStatus == -1) {
                break;
            }

            sensorStatus = getSensorStatus(4);
            if(sensorStatus == 0) {
                ;
            }
            else if(sensorStatus == 1) {
                infraredWarning2 = 1;
                warningInfrared(2);
            }
            else if(sensorStatus == -1) {
                break;
            }
        }
        vxsleep(INFRARED_READ_INTERVAL);
    }
    return NULL;
}

/* the monitor of water sensor */
void * waterSensorMonitor()
{
    while(1) {
        if(waterStatus == 1) {
            int sensorStatus = getSensorStatus(6);
            if (sensorStatus == 0) {
                ;
            }
            else if (sensorStatus == 1) {
                waterWarning1 = 1;
                warningWater(1);
                /* waterWarning1 = 0; */
            }
            else if (sensorStatus == -1) {
                break;
            }

            sensorStatus = getSensorStatus(7);
            if(sensorStatus == 0) {
                ;
            }
            else if(sensorStatus == 1) {
                waterWarning2 = 1;
                warningWater(2);
            }
            else if(sensorStatus == -1) {
                break;
            }
        }
        vxsleep(WATER_READ_INTERVAL);
    }
    return NULL;
}

/* the monitor of smoke sensor */
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
    int warningStatus1 = 0;
    int warningStatus2 = 0;
    /* 0 for waiting,
     * 1 for get first abnormal value, if continue to get abnormal values, show warning
     * 2 for warning status
     * 3 for warning status but get several normal values, in this status, if continuously get normal status, will go back to 0
     */

    int abnormalTime1 = 0;       /*times of get abnormal value continuously*/
    int abnormalTime2 = 0;
    int normalTime1 = 0;         /*times of get normal value continuously*/
    int normalTime2 = 0;

    while(1) {
        if(smokeStatus == 1) {
            int sensorStatus = getSensorStatus(9);
            if (sensorStatus == -1) {
                break;
                /*error*/
            }
            if (warningStatus1 == 0) {
                smokeWarning1 = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    ;   /*do nothing*/
                } else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus1 = 1;
                    abnormalTime1++;
                }
            } else if (warningStatus1 == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus1 = 0;          /*reset warning status */
                    abnormalTime1 = 0;           /* and abnormalTime */
                } else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime1++;
                    if (abnormalTime1 >=
                        MAX_SMOKE_ABNORMAL_VALUE_CONTINUOUS_TIME) {      /* if get enough abnormal value */
                        warningStatus1 = 2;          /* set warning status to 2 */
                        abnormalTime1 = 0;       /* reset abnormalTime */
                    }
                }
            } else if (warningStatus1 == 2) {
                smokeWarning1 = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus1 = 3;              /* goto  warning status 3 */
                    normalTime1++;                   /* normalTime add 1 */
                    warningSmoke(1);         /* warning */
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningSmoke(1);     /* warning */
                }
            } else if (warningStatus1 == 3) {
                if (sensorStatus == 0) {
                    normalTime1++;
                    if (normalTime1 >= MAX_SMOKE_NORMAL_VALUE_CONTINUOUS_TIME) {  /* if get enough normal value */
                        warningStatus1 = 0;      /* reset warning status */
                        normalTime1 = 0;         /* and normalTime */
                    }
                    warningSmoke(1);
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus1 = 2;                  /* go back to status 2 */
                    normalTime1 = 0;                     /* reset normalTime */
                    warningSmoke(1);
                }
            }


            sensorStatus = getSensorStatus(10);
            if (sensorStatus == -1) {
                break;
                /*error*/
            }
            if (warningStatus2 == 0) {
                smokeWarning1 = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    ;   /*do nothing*/
                } else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus2 = 1;
                    abnormalTime2++;
                }
            } else if (warningStatus2 == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus2 = 0;          /*reset warning status */
                    abnormalTime2 = 0;           /* and abnormalTime */
                } else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime2++;
                    if (abnormalTime2 >=
                        MAX_SMOKE_ABNORMAL_VALUE_CONTINUOUS_TIME) {      /* if get enough abnormal value */
                        warningStatus2 = 2;          /* set warning status to 2 */
                        abnormalTime2 = 0;       /* reset abnormalTime */
                    }
                }
            } else if (warningStatus2 == 2) {
                smokeWarning1 = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus2 = 3;              /* goto  warning status 3 */
                    normalTime2++;                   /* normalTime add 1 */
                    warningSmoke(2);         /* warning */
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningSmoke(2);     /* warning */
                }
            } else if (warningStatus2 == 3) {
                if (sensorStatus == 0) {
                    normalTime2++;
                    if (normalTime2 >= MAX_SMOKE_NORMAL_VALUE_CONTINUOUS_TIME) {  /* if get enough normal value */
                        warningStatus2 = 0;      /* reset warning status */
                        normalTime2 = 0;         /* and normalTime */
                    }
                    warningSmoke(2);
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus2 = 2;                  /* go back to status 2 */
                    normalTime2 = 0;                     /* reset normalTime */
                    warningSmoke(2);
                }
            }
        }
        vxsleep(SMOKE_READ_INTERVAL);
    }
    return NULL;
}

/* the monitor of temperature sensor */
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
void * temperatureSensorMonitor()
{
    int warningStatus1 = 0;
    int warningStatus2 = 0;
    /* 0 for waiting,
     * 1 for get first abnormal value, if continue to get abnormal values, show warning
     * 2 for warning status
     * 3 for warning status but get several normal values, in this status, if continuously get normal status, will go back to 0
     */

    int abnormalTime1 = 0;       /*times of get abnormal value continuously*/
    int abnormalTime2 = 0;
    int normalTime1 = 0;         /*times of get normal value continuously*/
    int normalTime2 = 0;

    while(1) {
        if(temperatureStatus == 1) {
            int sensorStatus = getSensorStatus(9);
            if (sensorStatus == -1) {
                break;
                /*error*/
            }
            if (warningStatus1 == 0) {
                temperatureWarning1 = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    ;   /*do nothing*/
                } else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus1 = 1;
                    abnormalTime1++;
                }
            } else if (warningStatus1 == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus1 = 0;          /*reset warning status */
                    abnormalTime1 = 0;           /* and abnormalTime */
                } else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime1++;
                    if (abnormalTime1 >=
                        MAX_TEMPERATURE_ABNORMAL_VALUE_CONTINUOUS_TIME) {      /* if get enough abnormal value */
                        warningStatus1 = 2;          /* set warning status to 2 */
                        abnormalTime1 = 0;       /* reset abnormalTime */
                    }
                }
            } else if (warningStatus1 == 2) {
                temperatureWarning1 = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus1 = 3;              /* goto  warning status 3 */
                    normalTime1++;                   /* normalTime add 1 */
                    warningTemperature(1);         /* warning */
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningTemperature(1);     /* warning */
                }
            } else if (warningStatus1 == 3) {
                if (sensorStatus == 0) {
                    normalTime1++;
                    if (normalTime1 >= MAX_TEMPERATURE_NORMAL_VALUE_CONTINUOUS_TIME) {  /* if get enough normal value */
                        warningStatus1 = 0;      /* reset warning status */
                        normalTime1 = 0;         /* and normalTime */
                    }
                    warningTemperature(1);
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus1 = 2;                  /* go back to status 2 */
                    normalTime1 = 0;                     /* reset normalTime */
                    warningTemperature(1);
                }
            }


            sensorStatus = getSensorStatus(10);
            if (sensorStatus == -1) {
                break;
                /*error*/
            }
            if (warningStatus2 == 0) {
                temperatureWarning1 = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    ;   /*do nothing*/
                } else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus2 = 1;
                    abnormalTime2++;
                }
            } else if (warningStatus2 == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus2 = 0;          /*reset warning status */
                    abnormalTime2 = 0;           /* and abnormalTime */
                } else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime2++;
                    if (abnormalTime2 >=
                        MAX_TEMPERATURE_ABNORMAL_VALUE_CONTINUOUS_TIME) {      /* if get enough abnormal value */
                        warningStatus2 = 2;          /* set warning status to 2 */
                        abnormalTime2 = 0;       /* reset abnormalTime */
                    }
                }
            } else if (warningStatus2 == 2) {
                temperatureWarning1 = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus2 = 3;              /* goto  warning status 3 */
                    normalTime2++;                   /* normalTime add 1 */
                    warningTemperature(2);         /* warning */
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningTemperature(2);     /* warning */
                }
            } else if (warningStatus2 == 3) {
                if (sensorStatus == 0) {
                    normalTime2++;
                    if (normalTime2 >= MAX_TEMPERATURE_NORMAL_VALUE_CONTINUOUS_TIME) {  /* if get enough normal value */
                        warningStatus2 = 0;      /* reset warning status */
                        normalTime2 = 0;         /* and normalTime */
                    }
                    warningTemperature(2);
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus2 = 2;                  /* go back to status 2 */
                    normalTime2 = 0;                     /* reset normalTime */
                    warningTemperature(2);
                }
            }
        }
        vxsleep(TEMPERATURE_READ_INTERVAL);
    }
    return NULL;
}

/* the monitor of server */
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

    int lastDoorWarning1 = 0;
    int lastDoorWarning2 = 0;
    int lastInfraredWarning1 = 0;   /* record the value of infraredWarning when read it last time */
    int lastInfraredWarning2 = 0;
    int lastWaterWarning1 = 0;
    int lastWaterWarning2 = 0;
    int lastSmokeWarning1 = 0;
    int lastSmokeWarning2 = 0;
    int lastTemperatureWarning1 = 0;
    int lastTemperatureWarning2 = 0;

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
        if(doorWarning1 != lastDoorWarning1) {
            if(doorWarning1 == 1) {
                lastDoorWarning1 = doorWarning1;
                strcpy(buf, "wd1");
                write(sockFd, buf, sizeof(buf));
            }
            else if(doorWarning1 == 0) {
                lastDoorWarning1 = doorWarning1;
            }
        }
        if(doorWarning2 != lastDoorWarning2) {
            if(doorWarning2 == 1) {
                lastDoorWarning2 = doorWarning2;
                strcpy(buf, "wd2");
                write(sockFd, buf, sizeof(buf));
            }
            else if(doorWarning2 == 0) {
                lastDoorWarning2 = doorWarning2;
            }
        }
        
        if(infraredWarning1 != lastInfraredWarning1) {
            if(infraredWarning1 == 1) {
                lastInfraredWarning1 = infraredWarning1;
                strcpy(buf, "wi1");
                write(sockFd, buf, sizeof(buf));
            }
            else if(infraredWarning1 == 0) {
                lastInfraredWarning1 = infraredWarning1;
            }
        }
        if(infraredWarning2 != lastInfraredWarning2) {
            if(infraredWarning2 == 1) {
                lastInfraredWarning2 = infraredWarning2;
                strcpy(buf, "wi2");
                write(sockFd, buf, sizeof(buf));
            }
            else if(infraredWarning2 == 0) {
                lastInfraredWarning2 = infraredWarning2;
            }
        }

        if(waterWarning1 != lastWaterWarning1) {
            if(waterWarning1 == 1) {
                lastWaterWarning1 = waterWarning1;
                strcpy(buf, "ww1");
                write(sockFd, buf, sizeof(buf));
            }
            else if(waterWarning1 == 0) {
                lastWaterWarning1 = waterWarning1;
            }
        }
        if(waterWarning2 != lastWaterWarning2) {
            if(waterWarning2 == 1) {
                lastWaterWarning2 = waterWarning2;
                strcpy(buf, "ww2");
                write(sockFd, buf, sizeof(buf));
            }
            else if(waterWarning2 == 0) {
                lastWaterWarning2 = waterWarning2;
            }
        }

        if(smokeWarning1 != lastSmokeWarning1) {
            if(smokeWarning1 == 1) {
                lastSmokeWarning1 = smokeWarning1;
                strcpy(buf, "ws1");
                write(sockFd, buf, sizeof(buf));
            }
            else if(smokeWarning1 == 0) {
                lastSmokeWarning1 = smokeWarning1;
            }
        }
        if(smokeWarning2 != lastSmokeWarning2) {
            if(smokeWarning2 == 1) {
                lastSmokeWarning2 = smokeWarning2;
                strcpy(buf, "ws2");
                write(sockFd, buf, sizeof(buf));
            }
            else if(smokeWarning2 == 0) {
                lastSmokeWarning2 = smokeWarning2;
            }
        }

        if(temperatureWarning1 != lastTemperatureWarning1) {
            if(temperatureWarning1 == 1) {
                lastTemperatureWarning1 = temperatureWarning1;
                strcpy(buf, "wt1");
                write(sockFd, buf, sizeof(buf));
            }
            else if(temperatureWarning1 == 0) {
                lastTemperatureWarning1 = temperatureWarning1;
            }
        }
        if(temperatureWarning2 != lastTemperatureWarning2) {
            if(temperatureWarning2 == 1) {
                lastTemperatureWarning2 = temperatureWarning2;
                strcpy(buf, "wt2");
                write(sockFd, buf, sizeof(buf));
            }
            else if(temperatureWarning2 == 0) {
                lastTemperatureWarning2 = temperatureWarning2;
            }
        }

        vxsleep(1000);
    }
    return NULL;
}

/* the thread that read instructions from server */
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

        if(strcmp(buf, "pd") == 0) {
            doorStatus = 0;
        }
        else if(strcmp(buf, "pi") == 0) {
            infraredStatus = 0;
        }
        else if(strcmp(buf, "pw") == 0) {
            waterStatus = 0;
        }
        else if(strcmp(buf, "ps") == 0) {
            smokeStatus = 0;
        }
        else if(strcmp(buf, "pt") == 0) {
            temperatureStatus = 0;
        }
        else if(strcmp(buf, "rd") == 0) {
            doorStatus = 1;
        }
        else if(strcmp(buf, "ri") == 0) {
            infraredStatus = 1;
        }
        else if(strcmp(buf, "rw") == 0) {
            waterStatus = 1;
        }
        else if(strcmp(buf, "rs") == 0) {
            smokeStatus = 1;
        }
        else if(strcmp(buf, "rt") == 0) {
            temperatureStatus = 1;
        }
        else if(strcmp(buf, "lon") == 0) {
            turnOnLight();
        }
        else if(strcmp(buf, "loff") == 0) {
            turnOffLight();
        }
        else if(buf[0] == 's' && buf[1] == 'c') {       /* setCall */ /* sc 12345678901 nType nZone */
            setPhone(buf);
        }
        else if(strcmp(buf, "q") == 0) {
            programEnd = 1;
        }
        else {
            printf("undefined command: %s\n", buf);
        }
    }
}




/* called when the door sensor warns */
void warningDoor(int i)
{
    if(i == 1) {
        if(taskSpawn("wd1", 200, 0, 100000, (FUNCPTR)warningDoorThread, i, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
            printf("create thread warning door 1 failed\n");
        }
    }
    else if(i == 2) {
        if(taskSpawn("wd2", 200, 0, 100000, (FUNCPTR)warningDoorThread, i, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
            printf("create thread warning door 2 failed\n");
        }
    }
}

/* the thread created in warningDoor() */
void * warningDoorThread(int i)
{
    if(i == 1) {
        int j = 0;
        while(j<10) {
            printf("warning door 1\n");
            vxsleep(ALARM_INTERVAL);
            j++;
        }
        doorWarning1 = 0;
    }
    else if(i == 2) {
        int j = 0;
        while(j<10) {
            printf("warning door 2\n");
            vxsleep(ALARM_INTERVAL);
            j++;
        }
        doorWarning2 = 0;
    }
    return NULL;
}

/* called when the infrared sensor warns */
void warningInfrared(int i)
{
    if(i == 1) {
        if(taskSpawn("wi1", 200, 0, 100000, (FUNCPTR)warningInfraredThread, i, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
            printf("create thread warning infrared 1 failed\n");
        }
    }
    else if (i == 2) {
        if(taskSpawn("wi2", 200, 0, 100000, (FUNCPTR)warningInfraredThread, i, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
            printf("create thread warning infrared 2 failed\n");
        }
    }

}

/* the thread created in warningInfrared() */
void * warningInfraredThread(int i)
{
    if(i == 1) {
        int j = 0;
        while(j<10) {
            printf("\twarning infrared 1\n");
            vxsleep(ALARM_INTERVAL);
            j++;
        }
        infraredWarning1 = 0;
    }
    else if(i == 2) {
        int j = 0;
        while(j<10) {
            printf("\twarning infrared 2\n");
            vxsleep(ALARM_INTERVAL);
            j++;
        }
        infraredWarning2 = 0;
    }
    return NULL;
}

/* called when the water sensor warns */
void warningWater(int i)
{
    if(i == 1) {
        if(taskSpawn("wd1", 200, 0, 100000, (FUNCPTR)warningWaterThread, i, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
            printf("create thread warning water 1 failed\n");
        }
    }
    else if(i == 2) {
        if(taskSpawn("wd2", 200, 0, 100000, (FUNCPTR)warningWaterThread, i, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
            printf("create thread warning water 2 failed\n");
        }
    }
}

/* the thread created in warningWater() */
void * warningWaterThread(int i)
{
    if(i == 1) {
        int j = 0;
        while(j<10) {
            printf("\t\twarning water 1\n");
            vxsleep(ALARM_INTERVAL);
            j++;
        }
        waterWarning1 = 0;
    }
    else if(i == 2) {
        int j = 0;
        while(j<10) {
            printf("\t\twarning water 2\n");
            vxsleep(ALARM_INTERVAL);
            j++;
        }
        waterWarning2 = 0;
    }
    return NULL;
}

/* called when the smoke sensor warns */
void warningSmoke(int i)
{
    printf("\t\t\twarning smoke %d\n", i);
}

/* called when the temperature sensor warns */
void warningTemperature(int i)
{
    printf("\t\t\t\twarning temperature %d\n", i);
}







void vxsleep(long int ms)
{
    int m = sysClkRateGet();   /*get core frequency*/
    m = 1000/m;
    m = ms/m + 1;    /* taskDelay(n) in fact delay (n-1)tick~n*tick*/
    taskDelay(m);
}