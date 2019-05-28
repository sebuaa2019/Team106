

#ifndef SOFTWARE_ENGINEERING_SENSOR_H
#define SOFTWARE_ENGINEERING_SENSOR_H

#define SENSOR_NUMBER 1


void Sensor();
int getSensorStatus(int sensorNumber);

void turnOnLight();
void turnOffLight();
void setPhone(char buf[]);
int isArm();
int isDisArm();
void setLed(int nID, int nState);
int getKey(char  *cKey);

int toInt(char string[]);               /* string to int */


#endif /*SOFTWARE_ENGINEERING_SENSOR_H*/
