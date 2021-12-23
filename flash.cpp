#include "flash.h"
#define EEPROM_SIZE 512

void dumpCfg(DevCfg cfg)
{
    Serial.print("num ");Serial.println(cfg.num);
    Serial.print("refreshTimeval ");Serial.println(cfg.refreshTimeval);
    Serial.print("powerMode ");Serial.println(cfg.powerMode);
    Serial.print("PirSense ");Serial.println(cfg.PirSense);
    Serial.print("darkThreshold ");Serial.println(cfg.darkThreshold);
    Serial.print("lightThreshold ");Serial.println(cfg.lightThreshold);
    Serial.print("enableLineBot ");Serial.println(cfg.enableLineBot);
    Serial.print("enableM2M ");Serial.println(cfg.enableM2M);
    Serial.println("devName " +String(cfg.devName));
    //String("Iot_001").toCharArray(cfg.devName,16);
    Serial.println("devPasswd " +String(cfg.devPasswd));
    //String("11111111").toCharArray(cfg.devPasswd,16);
    Serial.println("serverIP " +String(cfg.serverIP));
    Serial.println("ssid " +String(cfg.ssid));
    Serial.println("ssidPasswd " +String(cfg.ssidPasswd));
    Serial.println("lineToken " +String(cfg.lineToken));
}
int initFlash()
{
  return 0;//EEPROM.begin(EEPROM_SIZE);
}

void writeFlashDefault()
{
  DevCfg cfg={1,0,2,3,110,170, 0,0, "","IoT","192.168.123.234:1880","11111111","Wenhao","11111111","1234321"};
  EEPROM.put(0, cfg);
  
//  cfg.num = 2;
//  EEPROM.put(256, cfg);
  
  Serial.print(" 11 Page0 num: ");
  Serial.println(byte(EEPROM.read(0)));
  
//  Serial.print(" 11 Page1 num: ");
//  Serial.println(byte(EEPROM.read(256)));
}

DevCfg readCfg()
{
  DevCfg cfg={0};
  int offset=0;
  uint8_t page0 = EEPROM.read(0);
//  uint8_t page1 = EEPROM.read(256);
//
//  if(page0==0 || page1==0)
//  {
//    writeFlashDefault();
//    page0 = 1;
//    page1 = 2;
//  }
//  
//  if(page0<page1){
//    offset = 256;
//  }
  if(page0==0)
    writeFlashDefault();
  EEPROM.get(offset,cfg);
//dumpCfg(cfg);
Serial.println(String(EEPROM.length())); + Serial.println("==========================");
  return cfg;
}

void writeCfg(DevCfg cfg)
{
  int offset=0;
  uint8_t page0 = EEPROM.read(0);
//  uint8_t page1 = EEPROM.read(256);
//  
//  if(page0 > page1) // 3, 2
//  {
//    offset = 256;
//    if(page0 < 255)
//      cfg.num = page0 + 1;  // page1 = 4
//    else cfg.num = 2; 
//  }
//  else{
//    if(page1 < 254)
//      cfg.num = page1 + 1;  // page0 = 4
//    else cfg.num = 1; 
//  }
  if(cfg.num==255) cfg.num = 1;
  else cfg.num++;
  offset = 0;
  EEPROM.put(offset,cfg);
}
