#include "deviceIO.h"

int pinOuts[5]; // TBD
int pinNumbers = 0; // TBD

int numberKeyPress = 0;
bool pressed = false; 
bool lampState_=false;
int sensorPin_=0;
bool checkBtnPressed()
{
  bool isTrigged = false;
  if(pressed && numberKeyPress>=1) {
    numberKeyPress=0;
    pressed=false;
    isTrigged = true;;
  }
  return isTrigged;
}
void btnISR()
{
  numberKeyPress += 1;
  pressed = true;
}

void setBtnPin(int OutputPin ){
  pinMode(OutputPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(OutputPin), btnISR, FALLING);
}


void setOutputPin(int OutputPin)
{
  pinOuts[pinNumbers] = OutputPin;
  pinNumbers++;
  pinMode(OutputPin, OUTPUT);
  pinMode(25, OUTPUT); // for testing
}


void setOutputPinLevel(int OutputPin, int isHight )
{
  digitalWrite(OutputPin, isHight);
  digitalWrite(25, isHight); // for testing
}

void setLamp(bool bOnOff){
  digitalWrite( pinOuts[0] , bOnOff);
  lampState_ = bOnOff;
  
}
bool lampState(){
  return lampState_;
}

void setSensorPin(int sensorPin)
{
  pinMode(sensorPin, INPUT);
  sensorPin_ = sensorPin;
}

int getSensorValue()
{
  return analogRead(sensorPin_);
}