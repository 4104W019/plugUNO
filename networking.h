#ifndef _NETWROKING_H_
#define _NETWROKING_H_

#include "Arduino.h"
#include <WiFiNINA.h>

String getIP();
String getMAC();

void enable_WiFi();
void connect_WiFi(char *ssid, char *pass);
void printWifiStatus();
#endif
