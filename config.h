#include "flash.h"
#include "networking.h"
#include "deviceIO.h"
#include "dutyTask.h"
#include "webSvr.h"

#define FWVER (202201121)

void(* resetFunc) (void) = 0; //declare reset function @ address 0

WiFiServer server(80);            //server socket

DevCfg cfg={0};

int ledPin = 12;
int btnKey = 11;
int sensorPin = A0;

unsigned long last_time = 0;
unsigned long current_time = 0;

int lightSensorVaule = 0;
int pirTimeout = 0;//10*10; // 10sec
bool isPirTrigged = false;


