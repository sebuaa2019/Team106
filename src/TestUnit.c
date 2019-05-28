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
#include "TestUnit.h"


int test_warningStop = 0;
int * test_retThread;

/* record sensor on/off status */
int test_doorStatus = 1;             /* 0 for off, 1 for on */
int test_infraredStatus = 1;         /* 0 for off, 1 for on */
int test_waterStatus = 1;            /* 0 for off, 1 for on */
int test_smokeStatus = 1;            /* 0 for off, 1 for on */
int test_temperatureStatus = 1;      /* 0 for off, 1 for on */

/* record if the program is set to exit */
int test_programEnd = 0;

/* record if the sensors should warning now */
int test_doorWarning1 = 0;
int test_doorWarning2 = 0;
int test_infraredWarning1 = 0;       /* if the infrared is warning */
int test_infraredWarning2 = 0;
int test_waterWarning1 = 0;
int test_waterWarning2 = 0;
int test_smokeWarning1 = 0;          /* if the smoke is warning */
int test_smokeWarning2 = 0;
int test_temperatureWarning1 = 0;
int test_temperatureWarning2 = 0;



extern void vxsleep(long int ms);



void TestUnit()
{
    test_unlock();
    test_checkSecret();
    test_doorSensorMonitor();
    test_infraredSensorMonitor();
    test_waterSensorMonitor();
    test_smokeSensorMonitor();
    test_temperatureSensorMonitor();
    test_serverMonitor();
}

int test_isArm()
{
    int ret;
    printf("please input arm value(int): ");
    scanf("%d", &ret);
    return ret;
}

void test_unlock()
{
    while(1) {
        if(test_isArm()) {
            printf("isArm value: true\n");
            break;
        }
        else {
            printf("isArm value: false\n");
        }
        vxsleep(100);
    }

    while(1) {
        if(test_checkSecret()) {
            printf("password right\n");
            break;
        }
        else {
            printf("password wrong. please input again\n");
        }
    }
}

int test_getKey(char * cKey)
{
    printf("please input a key(int): ");
    scanf("%c", cKey);
    return 1;
}

int test_checkSecret()
{
    char cKey;
    char secret[10] = "123456";
    char input[10];
    int inputIndex = 0;
    while(1){
        if(test_getKey(&cKey))
        {
            printf("get \'%c\'; ",cKey);
            if(cKey == '*')   /*confirm*/
            {
                input[inputIndex] = '\0';
                printf("input string: %s\n", input);
                break;
            }
            else if(cKey == '#')  /*clear*/
            {
                memset(input, 0, sizeof(input));
                printf("input string: %s\n", input);
                inputIndex = 0;
            }
            else{ /*'0' - '9'*/
                input[inputIndex++] = cKey;
                printf("input string: %s\n", input);
            }

        }
        vxsleep(10);

    }

    if(strcmp(input,secret)==0){
        printf("the input is the same as the secret\n");
        return 1;
    }
    else {
        printf("the input is not the same as the secret\n");
    }
    return 0;

}

int test_getSensorStatus(int sensorNumber)
{
    int ret;
    printf("please input the value of sensor %d: ", sensorNumber);
    scanf("%d", &ret);
    if(sensorNumber <= 8) {
        return ret;
    }
    else if(sensorNumber <= 11) {
        if(ret >= 80) {
            return 1;
        }
        else if(ret >= 0) {
            return 0;
        }
        else {
            return ret;
        }
    }
    else {
        if(ret >= 50) {
            return 1;
        }
        else if(ret >= 0) {
            return 0;
        }
        else {
            return ret;
        }
    }
}

void * test_doorSensorMonitor()
{
    while(1) {
        if(test_doorStatus == 1) {
            int sensorStatus = test_getSensorStatus(0);
            if(sensorStatus == 0) {
                printf("no warning\n");
            }
            else if(sensorStatus == 1) {
                test_doorWarning1 = 1;
                printf("warning door 1\n");
            }
            else if(sensorStatus == -1) {
                printf("get error value -1\n");
                break;
            }
            else {
                printf("for door sensor, you can only input 1, 0, -1\n");
            }

            sensorStatus = test_getSensorStatus(1);
            if(sensorStatus == 0) {
                printf("no warning\n");
            }
            else if(sensorStatus == 1) {
                test_doorWarning2 = 1;
                printf("warning door 2\n");
            }
            else if(sensorStatus == -1) {
                printf("get error value: -1\n");
                break;
            }
            else {
                printf("for door sensor, you can only input 1, 0, -1\n");
            }
        }
    }
    return NULL;
}

void * test_infraredSensorMonitor()
{
    while(1) {
        if(test_infraredStatus == 1) {
            int sensorStatus = test_getSensorStatus(3);
            if (sensorStatus == 0) {
                printf("no warning\n");
            }
            else if (sensorStatus == 1) {
                test_infraredWarning1 = 1;
                printf("warning infrared 1\n");
            }
            else if (sensorStatus == -1) {
                printf("get error value: -1\n");
                break;
            }
            else {
                printf("for infrared sensor, you can only input 1, 0, -1\n");
            }

            sensorStatus = test_getSensorStatus(4);
            if(sensorStatus == 0) {
                printf("no warning\n");
            }
            else if(sensorStatus == 1) {
                test_infraredWarning2 = 1;
                printf("warning infrared 2\n");
            }
            else if(sensorStatus == -1) {
                printf("get error value: -1\n");
                break;
            }
            else {
                printf("for infrared sensor, you can only input 1, 0, -1\n");
            }
        }
    }
    return NULL;
}

void * test_waterSensorMonitor()
{
    while(1) {
        if(test_waterStatus == 1) {
            int sensorStatus = test_getSensorStatus(6);
            if (sensorStatus == 0) {
                printf("no warning\n");
            }
            else if (sensorStatus == 1) {
                test_waterWarning1 = 1;
                printf("warning water 1\n");
                /* waterWarning1 = 0; */
            }
            else if (sensorStatus == -1) {
                printf("get error value: -1\n");
                break;
            }
            else {
                printf("for water sensor, you can only input 1, 0, -1\n");
            }

            sensorStatus = test_getSensorStatus(7);
            if(sensorStatus == 0) {
                printf("no warning\n");
            }
            else if(sensorStatus == 1) {
                test_waterWarning2 = 1;
                printf("warning water 2\n");
            }
            else if(sensorStatus == -1) {
                printf("get error value: -1\n");
                break;
            }
            else {
                printf("for water sensor, you can only input 1, 0, -1\n");
            }
        }
    }
    return NULL;
}

void * test_smokeSensorMonitor()
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
        if(test_smokeStatus == 1) {
            int sensorStatus = test_getSensorStatus(9);
            if (sensorStatus == -1) {
                printf("get error value -1\n");
                break;
            }
            if (warningStatus1 == 0) {
                test_smokeWarning1 = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    printf("get sensor status 0 when warning status is 0; no warning\n");   /*do nothing*/
                }
                else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus1 = 1;
                    abnormalTime1++;
                    printf("get sensor status 1 when warning status is 0; no warning; abnormalTime: %d\n", abnormalTime1);
                }
            }
            else if (warningStatus1 == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus1 = 0;          /*reset warning status */
                    abnormalTime1 = 0;           /* and abnormalTime */
                    printf("get sensor status 0 when warning status is 1; no warning\n");
                }
                else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime1++;
                    printf("get sensor status 1 when warning status is 1; no warning; abnormalTime: %d\n", abnormalTime1);
                    if (abnormalTime1 >= 8) {      /* if get enough abnormal value */
                        warningStatus1 = 2;          /* set warning status to 2 */
                        abnormalTime1 = 0;       /* reset abnormalTime */
                    }
                }
            }
            else if (warningStatus1 == 2) {
                test_smokeWarning1 = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus1 = 3;              /* goto  warning status 3 */
                    normalTime1++;                   /* normalTime add 1 */
                    printf("warning smoke 1; ");
                    printf("get sensor status 0 when warning status is 2; normalTime: %d\n", normalTime1);
                }
                else if (sensorStatus == 1) {        /* if get an abnormal value */
                    printf("warning smoke 1; ");     /* warning */
                    printf("get sensor status 1 when warning status is 2\n");
                }
            }
            else if (warningStatus1 == 3) {
                test_smokeWarning1 = 1;
                if (sensorStatus == 0) {
                    normalTime1++;
                    if (normalTime1 >= 8) {  /* if get enough normal value */
                        warningStatus1 = 0;      /* reset warning status */
                        normalTime1 = 0;         /* and normalTime */
                    }
                    printf("warning smoke 1; ");
                    printf("get sensor status 0 when warning status is 3; normalTime: %d\n", normalTime1);
                }
                else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus1 = 2;                  /* go back to status 2 */
                    normalTime1 = 0;                     /* reset normalTime */
                    printf("warning smoke 1; ");
                    printf("get sensor status 1 when warning status is 3\n");
                }
            }


            sensorStatus = test_getSensorStatus(10);
            if (sensorStatus == -1) {
                printf("get error value -1\n");
                break;
            }
            if (warningStatus2 == 0) {
                test_smokeWarning2 = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    printf("get sensor status 0 when warning status is 0; no warning\n");   /*do nothing*/
                } 
                else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus2 = 1;
                    abnormalTime2++;
                    printf("get sensor status 1 when warning status is 0; no warning; abnormalTime: %d\n", abnormalTime2);
                }
            } else if (warningStatus2 == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus2 = 0;          /*reset warning status */
                    abnormalTime2 = 0;           /* and abnormalTime */
                    printf("get sensor status 0 when warning status is 1; no warning\n");
                } 
                else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime2++;
                    printf("get sensor status 1 when warning status is 1; no warning; abnormalTime: %d\n", abnormalTime2);
                    if (abnormalTime2 >= 8) {      /* if get enough abnormal value */
                        warningStatus2 = 2;          /* set warning status to 2 */
                        abnormalTime2 = 0;       /* reset abnormalTime */
                    }
                }
            } else if (warningStatus2 == 2) {
                test_smokeWarning2 = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus2 = 3;              /* goto  warning status 3 */
                    normalTime2++;                   /* normalTime add 1 */
                    printf("warning smoke 2; ");
                    printf("get sensor status 0 when warning status is 2; normalTime: %d\n", normalTime2);         /* warning */
                } 
                else if (sensorStatus == 1) {        /* if get an abnormal value */
                    printf("warning smoke 2; ");     /* warning */
                    printf("get sensor status 1 when warning status is 2\n");     /* warning */
                }
            } else if (warningStatus2 == 3) {
                test_smokeWarning2 = 1;
                if (sensorStatus == 0) {
                    normalTime2++;
                    if (normalTime2 >= 8) {  /* if get enough normal value */
                        warningStatus2 = 0;      /* reset warning status */
                        normalTime2 = 0;         /* and normalTime */
                    }
                    printf("warning smoke 2; ");
                    printf("get sensor status 0 when warning status is 3; normalTime: %d\n", normalTime2);
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus2 = 2;                  /* go back to status 2 */
                    normalTime2 = 0;                     /* reset normalTime */
                    printf("warning smoke 2; ");
                    printf("get sensor status 1 when warning status is 3\n");
                }
            }
        }
    }
    return NULL;
}

void * test_temperatureSensorMonitor()
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
        if(test_temperatureStatus == 1) {
            int sensorStatus = test_getSensorStatus(9);
            if (sensorStatus == -1) {
                printf("get error value -1\n");
                break;
            }
            if (warningStatus1 == 0) {
                test_temperatureWarning1 = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    printf("get sensor status 0 when warning status is 0; no warning\n");   /*do nothing*/
                }
                else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus1 = 1;
                    abnormalTime1++;
                    printf("get sensor status 1 when warning status is 0; no warning; abnormalTime: %d\n", abnormalTime1);
                }
            }
            else if (warningStatus1 == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus1 = 0;          /*reset warning status */
                    abnormalTime1 = 0;           /* and abnormalTime */
                    printf("get sensor status 0 when warning status is 1; no warning\n");
                }
                else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime1++;
                    printf("get sensor status 1 when warning status is 1; no warning; abnormalTime: %d\n", abnormalTime1);
                    if (abnormalTime1 >= 8) {      /* if get enough abnormal value */
                        warningStatus1 = 2;          /* set warning status to 2 */
                        abnormalTime1 = 0;       /* reset abnormalTime */
                    }
                }
            }
            else if (warningStatus1 == 2) {
                test_temperatureWarning1 = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus1 = 3;              /* goto  warning status 3 */
                    normalTime1++;                   /* normalTime add 1 */
                    printf("warning temperature 1; ");
                    printf("get sensor status 0 when warning status is 2; normalTime: %d\n", normalTime1);
                }
                else if (sensorStatus == 1) {        /* if get an abnormal value */
                    printf("warning temperature 1; ");     /* warning */
                    printf("get sensor status 1 when warning status is 2\n");
                }
            }
            else if (warningStatus1 == 3) {
                test_temperatureWarning1 = 1;
                if (sensorStatus == 0) {
                    normalTime1++;
                    if (normalTime1 >= 8) {  /* if get enough normal value */
                        warningStatus1 = 0;      /* reset warning status */
                        normalTime1 = 0;         /* and normalTime */
                    }
                    printf("warning temperature 1; ");
                    printf("get sensor status 0 when warning status is 3; normalTime: %d\n", normalTime1);
                }
                else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus1 = 2;                  /* go back to status 2 */
                    normalTime1 = 0;                     /* reset normalTime */
                    printf("warning temperature 1; ");
                    printf("get sensor status 1 when warning status is 3\n");
                }
            }


            sensorStatus = test_getSensorStatus(10);
            if (sensorStatus == -1) {
                printf("get error value -1\n");
                break;
            }
            if (warningStatus2 == 0) {
                test_temperatureWarning2 = 0;
                if (sensorStatus == 0) {     /*do nothing*/
                    printf("get sensor status 0 when warning status is 0; no warning\n");   /*do nothing*/
                }
                else if (sensorStatus == 1) {        /* warning status goto 1, abnotmalTime add 1 */
                    warningStatus2 = 1;
                    abnormalTime2++;
                    printf("get sensor status 1 when warning status is 0; no warning; abnormalTime: %d\n", abnormalTime2);
                }
            } else if (warningStatus2 == 1) {
                if (sensorStatus == 0) {     /* if get a normal value when warning status is 1,*/
                    warningStatus2 = 0;          /*reset warning status */
                    abnormalTime2 = 0;           /* and abnormalTime */
                    printf("get sensor status 0 when warning status is 1; no warning\n");
                }
                else if (sensorStatus == 1) {    /* if get an abnormal value when warning status is 1, */
                    abnormalTime2++;
                    printf("get sensor status 1 when warning status is 1; no warning; abnormalTime: %d\n", abnormalTime2);
                    if (abnormalTime2 >= 8) {      /* if get enough abnormal value */
                        warningStatus2 = 2;          /* set warning status to 2 */
                        abnormalTime2 = 0;       /* reset abnormalTime */
                    }
                }
            } else if (warningStatus2 == 2) {
                test_temperatureWarning2 = 1;
                if (sensorStatus == 0) {     /* if get a normal value in warningStatus 2 */
                    warningStatus2 = 3;              /* goto  warning status 3 */
                    normalTime2++;                   /* normalTime add 1 */
                    printf("warning temperature 2; ");
                    printf("get sensor status 0 when warning status is 2; normalTime: %d\n", normalTime2);         /* warning */
                }
                else if (sensorStatus == 1) {        /* if get an abnormal value */
                    printf("warning temperature 2; ");     /* warning */
                    printf("get sensor status 1 when warning status is 2\n");     /* warning */
                }
            } else if (warningStatus2 == 3) {
                test_temperatureWarning2 = 1;
                if (sensorStatus == 0) {
                    normalTime2++;
                    if (normalTime2 >= 8) {  /* if get enough normal value */
                        warningStatus2 = 0;      /* reset warning status */
                        normalTime2 = 0;         /* and normalTime */
                    }
                    printf("warning temperature 2; ");
                    printf("get sensor status 0 when warning status is 3; normalTime: %d\n", normalTime2);
                } else if (sensorStatus == 1) {        /* if get an abnormal value */
                    warningStatus2 = 2;                  /* go back to status 2 */
                    normalTime2 = 0;                     /* reset normalTime */
                    printf("warning temperature 2; ");
                    printf("get sensor status 1 when warning status is 3\n");
                }
            }
        }
    }
    return NULL;
}

void * test_serverMonitor()
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
    printf("server start!\n");
    /* create socket */
    sockFd = socket(AF_INET,SOCK_STREAM,0);

    /* set the net which the socket tend to bind */
    serAddr.sin_family=AF_INET;             /* ip protocol */
    serAddr.sin_port=3333;                  /* port */
    inet_aton(ip, &(serAddr.sin_addr));     /*change ip to 32 bit integer*/
    memset(serAddr.sin_zero, 0, 8);

    if (connect(sockFd, (struct sockaddr*)&serAddr, len)==OK) {
        printf("[client]Connected\n");
    }

    memset(buf, 0, sizeof(buf));

    if(taskSpawn("serverRead", 200, 0, 100000, (FUNCPTR)test_serverRead, sockFd, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR) {
        printf("serverRead create failed\n");
    }

    while(1) {
        /*
         * if the value of infrared/smoke warning change to 1, send warning message
         * if the value of infrared/smoke warning change to 0, do nothing
         */
        printf("please input the value of: the warning status of ten sensors(10 int in total): ");
        scanf("%d", &test_doorWarning1);
        scanf("%d", &test_doorWarning2);
        scanf("%d", &test_infraredWarning1);
        scanf("%d", &test_infraredWarning2);
        scanf("%d", &test_waterWarning1);
        scanf("%d", &test_waterWarning2);
        scanf("%d", &test_smokeWarning1);
        scanf("%d", &test_smokeWarning2);
        scanf("%d", &test_temperatureWarning1);
        scanf("%d", &test_temperatureWarning2);
        if(test_doorWarning1 != lastDoorWarning1) {
            if(test_doorWarning1 == 1) {
                lastDoorWarning1 = test_doorWarning1;
                test_doorWarning1 = 0;
                strcpy(buf, "0");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_doorWarning1 == 0) {
                lastDoorWarning1 = test_doorWarning1;
            }
        }
        if(test_doorWarning2 != lastDoorWarning2) {
            if(test_doorWarning2 == 1) {
                lastDoorWarning2 = test_doorWarning2;
                test_doorWarning2 = 0;
                strcpy(buf, "1");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_doorWarning2 == 0) {
                lastDoorWarning2 = test_doorWarning2;
            }
        }

        if(test_infraredWarning1 != lastInfraredWarning1) {
            if(test_infraredWarning1 == 1) {
                lastInfraredWarning1 = test_infraredWarning1;
                test_infraredWarning1 = 0;
                strcpy(buf, "2");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_infraredWarning1 == 0) {
                lastInfraredWarning1 = test_infraredWarning1;
            }
        }
        if(test_infraredWarning2 != lastInfraredWarning2) {
            if(test_infraredWarning2 == 1) {
                lastInfraredWarning2 = test_infraredWarning2;
                test_infraredWarning2 = 0;
                strcpy(buf, "3");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_infraredWarning2 == 0) {
                lastInfraredWarning2 = test_infraredWarning2;
            }
        }

        if(test_waterWarning1 != lastWaterWarning1) {
            if(test_waterWarning1 == 1) {
                lastWaterWarning1 = test_waterWarning1;
                test_waterWarning1 = 0;
                strcpy(buf, "4");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_waterWarning1 == 0) {
                lastWaterWarning1 = test_waterWarning1;
            }
        }
        if(test_waterWarning2 != lastWaterWarning2) {
            if(test_waterWarning2 == 1) {
                lastWaterWarning2 = test_waterWarning2;
                test_waterWarning2 = 0;
                strcpy(buf, "5");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_waterWarning2 == 0) {
                lastWaterWarning2 = test_waterWarning2;
            }
        }


        if(test_smokeWarning1 != lastSmokeWarning1) {
            if(test_smokeWarning1 == 1) {
                lastSmokeWarning1 = test_smokeWarning1;
                test_smokeWarning1 = 0;
                strcpy(buf, "6");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_smokeWarning1 == 0) {
                lastSmokeWarning1 = test_smokeWarning1;
            }
        }
        if(test_smokeWarning2 != lastSmokeWarning2) {
            if(test_smokeWarning2 == 1) {
                lastSmokeWarning2 = test_smokeWarning2;
                test_smokeWarning2 = 0;
                strcpy(buf, "7");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_smokeWarning2 == 0) {
                lastSmokeWarning2 = test_smokeWarning2;
            }
        }

        if(test_temperatureWarning1 != lastTemperatureWarning1) {
            if(test_temperatureWarning1 == 1) {
                lastTemperatureWarning1 = test_temperatureWarning1;
                test_temperatureWarning1 = 0;
                strcpy(buf, "8");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_temperatureWarning1 == 0) {
                lastTemperatureWarning1 = test_temperatureWarning1;
            }
        }
        if(test_temperatureWarning2 != lastTemperatureWarning2) {
            if(test_temperatureWarning2 == 1) {
                lastTemperatureWarning2 = test_temperatureWarning2;
                test_temperatureWarning2 = 0;
                strcpy(buf, "9");
                write(sockFd, buf, sizeof(buf));
            }
            else if(test_temperatureWarning2 == 0) {
                lastTemperatureWarning2 = test_temperatureWarning2;
            }
        }

    }
    return NULL;
}

void * test_serverRead(int sockFd)
{
    char buf[255];
    int rLen;

    while(1) {
        memset(buf, 0, sizeof(buf));
        rLen = read(sockFd, buf, sizeof(buf));
        if(rLen == 0) {
            printf("get empty command\n");
            break;
        }
        printf("get command: %s\n", buf);
        if(strcmp(buf, "pd") == 0) {
            test_doorStatus = 0;
        }
        else if(strcmp(buf, "pi") == 0) {
            test_infraredStatus = 0;
        }
        else if(strcmp(buf, "pw") == 0) {
            test_waterStatus = 0;
        }
        else if(strcmp(buf, "ps") == 0) {
            test_smokeStatus = 0;
        }
        else if(strcmp(buf, "pt") == 0) {
            test_temperatureStatus = 0;
        }
        else if(strcmp(buf, "rd") == 0) {
            test_doorStatus = 1;
        }
        else if(strcmp(buf, "ri") == 0) {
            test_infraredStatus = 1;
        }
        else if(strcmp(buf, "rw") == 0) {
            test_waterStatus = 1;
        }
        else if(strcmp(buf, "rs") == 0) {
            test_smokeStatus = 1;
        }
        else if(strcmp(buf, "rt") == 0) {
            test_temperatureStatus = 1;
        }
        else if(strcmp(buf, "lon") == 0) {      /* light on */
            printf("light on\n");
        }
        else if(strcmp(buf, "loff") == 0) {     /* light off */
            printf("light off\n");
        }
        else if(buf[0] == 's' && buf[1] == 'c') {       /* setCall */ /* sc 12345678901 nType nZone */
            printf("phone set\n");
        }
        else if(strcmp(buf, "q") == 0) {
            test_programEnd = 1;
        }
        else {
            printf("undefined command: %s\n", buf);
        }
    }
}
