#ifndef _DEVICEIO_H_
#define _DEVICEIO_H_

#include "Arduino.h"

bool checkBtnPressed();
//void btnISR();
void setBtnPin(int OutputPin );
void setOutputPin(int OutputPin);
void setOutputPinLevel(int OutputPin, int isHight );
void setLamp(bool bOnOff );
bool lampState();
void setSensorPin(int sensorPin);
int getSensorValue();
int getTriger();
void setTriger(int times);

#endif
