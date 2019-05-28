
#ifndef SE_TESTUNIT_H
#define SE_TESTUNIT_H


void TestUnit();


/* functions provided in IOSensorDriver.c */
int test_isArm();
int test_getKey(char * cKey);


/* functions provided in sensor.c */
int test_getSensorStatus(int sensorNumber);

/* function need to be test in MasterControl.c */
void test_unlock();
int test_checkSecret();
void * test_doorSensorMonitor();
void * test_infraredSensorMonitor();
void * test_waterSensorMonitor();
void * test_smokeSensorMonitor();
void * test_temperatureSensorMonitor();
void * test_serverMonitor();
void * test_serverRead(int sockFd);


#endif
