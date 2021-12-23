#include "networking.h"

int status = WL_IDLE_STATUS;      //connection status
IPAddress my_ip;

String IpAddress2String(const IPAddress& ipAddress)
{
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}


String getIP(){
 return IpAddress2String(WiFi.localIP());
}

String getMAC(){
    String strMac;
    byte mac[6];
    WiFi.macAddress(mac);
    for(int i=5;i>=0;--i){
        strMac += String(mac[i],HEX);
        if(i!=0)
            strMac += String(":");
    }
    return strMac;
}

void enable_WiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connect_WiFi(char *ssid, char *pass) {
  pinMode(13, INPUT_PULLUP);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: " + String(digitalRead(13)));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    if(digitalRead(13))
        status = WiFi.begin(ssid, pass);
    else{
        status = WiFi.beginAP(ssid, pass);
        Serial.println(String(status));
        if(status==WL_AP_LISTENING)
            break;
        }
    // wait 10 seconds for connection:
    delay(10000);
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  my_ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(my_ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(my_ip);
}

