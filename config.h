#include "flash.h"
#include "networking.h"
#include "deviceIO.h"
#include "dutyTask.h"
#include "webSvr.h"

WiFiServer server(80);            //server socket

DevCfg cfg={0};

int ledPin = 12;
int btnKey = 8;
int sensorPin = A0;

int lightSensorVaule = 0;
int pirTimeout = 10*10; // 10sec
bool isPirTrigged = false;


