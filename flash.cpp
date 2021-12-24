#include "flash.h"

void dumpCfg(DevCfg cfg)
{
    Serial.print("num ");Serial.println(cfg.num);
    Serial.print("refreshTimeval ");Serial.println(cfg.refreshTimeval);
    Serial.print("powerMode ");Serial.println(cfg.powerMode);
    Serial.print("pirMode ");Serial.println(cfg.pirMode);
    Serial.print("darkThreshold ");Serial.println(cfg.darkThreshold);
    Serial.print("lightThreshold ");Serial.println(cfg.lightThreshold);
    Serial.print("enableLineBot ");Serial.println(cfg.enableLineBot);
    Serial.print("enableM2M ");Serial.println(cfg.enableM2M);
    Serial.println("devName " +String(cfg.devName));
    //String("Iot_001").toCharArray(cfg.devName,16);
    Serial.println("devPasswd " +String(cfg.devPasswd));
    //String("11111111").toCharArray(cfg.devPasswd,16);
    Serial.println("serverIP " +String(cfg.serverIP));
	Serial.println("serverPort " +String(cfg.serverPort));
    Serial.println("ssid " +String(cfg.ssid));
    Serial.println("ssidPasswd " +String(cfg.ssidPasswd));
    Serial.println("lineToken " +String(cfg.lineToken));
}

int initFlash()
{
  return 0;
}

void writeFlashDefault()
{  
   DevCfg cfg={
    /*.num=*/ 1,
    /*.refreshTimeval=*/0,
    /*.powerMode=*/2,
    /*.pirMode=*/1,
    /*.darkThreshold=*/0,
    /*.lightThreshold=*/1023,
    /*.enableLineBot=*/0,
    /*.enableM2M=*/0,
    /*.serverPort=*/1880,
    /*.reverse=*/"",
    /*.devName=*/"UNO Plug",
    /*.devPasswd=*/"22222222",
    /*.serverIP=*/"",
    /*.ssid=*/"UnoAP",
    /*.ssidPasswd=*/"11111111",
    /*.lineToken=*/"NotSupport"
    };

  EEPROM.put(0, cfg);
  
  Serial.println(byte(EEPROM.read(0)));
  
}

DevCfg readCfg()
{
	DevCfg cfg={0};
	int offset=0;
	uint8_t page0 = EEPROM.read(0);

	if(page0==0)
	writeFlashDefault();

	EEPROM.get(offset,cfg);

	Serial.println(String(EEPROM.length())); + Serial.println("==========================");
	
	return cfg;
}

void writeCfg(DevCfg cfg)
{
	int offset=0;
	uint8_t page0 = EEPROM.read(0);

	if(cfg.num==255) cfg.num = 1;
	else cfg.num++;

	EEPROM.put(offset,cfg);
}
