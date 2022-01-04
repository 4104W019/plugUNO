#ifndef _WEBSVR_H_
#define _WEBSVR_H_

#include "Arduino.h"
#include "flash.h"

#include <WiFiNINA.h>
#include <ArduinoJson.h>

enum REST_CMD{
	RestMode= 0,
	RestSetting,
	RestSensor,
	RestPIR,
	RestNA,
	RestMax
};

void create_json(char *tag, float value, char *unit);
void add_json_object(char *tag, float value, char *unit);

String sysInfoPage(String strMyAPIP_, String strMyIP, uint32_t fwVer);

String powerModePage(int powerMode);

String settingPage(
    String serverIP,
    String serverPort,
    String ssid, 
    String ssid_passwd, 
    String lineToken,
    String dark_threshold,
    String light_threshold,
    int pirMode,
    int enableM2M);

String devStatusPage(String strPirState, String strAdcState);



#endif
