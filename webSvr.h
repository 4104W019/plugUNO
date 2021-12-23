#ifndef _WEBSVR_H_
#define _WEBSVR_H_

#include "Arduino.h"
#include "flash.h"

#include <WiFiNINA.h>
#include <ArduinoJson.h>

void create_json(char *tag, float value, char *unit);
void add_json_object(char *tag, float value, char *unit);

String sysInfoPage(String strMyAPIP_, String strMyIP, int chipID);
String powerModePage(int powerMode);
String settingPage(String serverIP, String ssid, String ssid_passwd, String lineToken,String dark_threshold,String light_threshold);
String devStatusPage(String strPirState, String strAdcState);

#endif
