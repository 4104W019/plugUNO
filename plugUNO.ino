#include "config.h"

void dutyTask()
{
	lightSensorVaule = getSensorValue();

	int dark = cfg.darkThreshold; 
	int light = cfg.lightThreshold;

    if(WiFi.status()!= WL_CONNECTED || cfg.powerMode == 2)
    {
		if(lightSensorVaule >= light){
			if(lampState()){
				setLamp(false);
				sendData(RestSensor);
				pirTimeout = 0;
				isPirTrigged = false;
			}
		}
		else if(lightSensorVaule <= dark)
		{
			if(!lampState() && lightSensorVaule < dark){
				setLamp(true);
				sendData(RestSensor);
				pirTimeout = 0;
				isPirTrigged = false;
			}
		}
		else// if(lightSensorVaule < light)
		{
			if(pirTimeout > 0)
			{
				if(--pirTimeout<=0){
					setOutputPinLevel(ledPin,LOW);
					setTriger(0);
					isPirTrigged = false;
					sendData(RestPIR);
				}
			}
			else{
				if(checkBtnPressed())
				{
					pirTimeout = 10*10;
					setOutputPinLevel(ledPin,HIGH);
					isPirTrigged = true;
					sendData(RestPIR);
				}
			}
		}		
    }
	else{
		if(lampState() != cfg.powerMode){
			setLamp(cfg.powerMode);
			sendData(RestSensor);
			pirTimeout = 0;
			isPirTrigged = false;
		}
	}
	    
}

void printWEB() {
  WiFiClient client= server.available();

  if (client) {                             // if you get a client,
    Serial.println("============ new client ============");           // print a message out the serial port

    String html;
    enum REST_CMD updateHtml=RestMax;

    String htmlHead="HTTP/1.1 200 OK\r\n"
    "Content-type:text/html\r\n";
  
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

             if(updateHtml!=RestMax)
                {
                //client.print(htmlHead);
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();
                client.print(html);
                client.println();
                }

            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        if (currentLine.endsWith("GET / HTTP") || currentLine.endsWith("GET /system"))
        {
            html =sysInfoPage( getIP(),getMAC(), FWVER);
            updateHtml = RestNA;
         
        }
        else if (currentLine.endsWith("GET /powerMode"))
        {
            html =powerModePage(cfg.powerMode);
            updateHtml = RestMode;
        }
        else if (currentLine.endsWith("GET /off"))
        {
        	cfg.powerMode=0;
            html =powerModePage(cfg.powerMode);
            updateHtml = RestMode;
        }
        else if (currentLine.endsWith("GET /on"))
        {
        	cfg.powerMode=1;
            html =powerModePage(cfg.powerMode);
            updateHtml = RestMode;
        }
        else if (currentLine.endsWith("GET /auto"))
        {
            cfg.powerMode=2;
            html =powerModePage(cfg.powerMode);
            updateHtml = RestMode;
        }
		else if (currentLine.endsWith("POST /device_threshold"))
        {
            char *keyVal[4];
			String dark="";
            String light="";
			String power="";
		    String postForm  = client.readString();
			int idx = postForm.indexOf("power_mode");
			
			postForm = postForm.substring(idx);
			char *pch = postForm.c_str();
			pch = strtok (pch,"&");
			idx = 0;
            keyVal[idx] = pch;
			while (pch != NULL)
            {
                idx++;
                pch = strtok (NULL, "&");
                keyVal[idx] = pch;
            }  
			for(int i=0; i<idx;++i)
		    {
		     	if(strncmp(keyVal[i],"dark_thresh",11)==0){
                    dark = String(keyVal[i]+11+1);
					Serial.println(String(keyVal[i])+" : " + dark);
			 		cfg.darkThreshold=dark.toInt();
             	}
			 	else if(strncmp(keyVal[i],"light_thresh",12)==0){
                    light = String(keyVal[i]+12+1);
					Serial.println(String(keyVal[i])+" : " + light);
					cfg.lightThreshold=light.toInt();
             	}
			 	else if(strncmp(keyVal[i],"power_mode",10)==0){
                    power = String(keyVal[i]+10+1);
					
					if(power=="on") 
						cfg.powerMode=1;
					else if (power=="off") 
						cfg.powerMode=0;
					else 
						cfg.powerMode=2;
					Serial.println(String(keyVal[i])+" : " + String(cfg.powerMode));
			 	}
		    }
			html =String("");
            updateHtml = RestMode;
			client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
			break;
        }
        else if (currentLine.endsWith("POST /settingX"))
        {
             char *keyVal[12];
              
              String postForm  = client.readString();

              int idx = postForm.indexOf("sysPasswd"); 
              postForm = postForm.substring(idx);
              char *pch = postForm.c_str();
              pch = strtok (pch,"&");
              idx = 0;
              keyVal[idx] = pch;
              while (pch != NULL)
              {
                idx++;
                pch = strtok (NULL, "&");
                keyVal[idx] = pch;
              }

              String sysPasswd="";
              String serverIP="";
			  String serverPort="";
              String ssid="";
              String passwd="";
              String token="";
				
              String dark="";
              String light="";
              
              
              for(int i=0; i<idx;++i)
              {
                  if(strncmp(keyVal[i],"sysPasswd",9)==0){
                    sysPasswd = String(keyVal[i]+9+1);
                    Serial.println("sysPasswd : "+ sysPasswd);
                    }
                  else if(strncmp(keyVal[i],"serverIP",8)==0){
                    serverIP = String(keyVal[i]+8+1);
                    Serial.println("serverIP : "+ serverIP);
                    }
				  else if(strncmp(keyVal[i],"serverPort",10)==0){
                    serverPort = String(keyVal[i]+10+1);
                    Serial.println("serverPort : "+ serverPort);
                    }
                  else if(strncmp(keyVal[i],"ssid",4)==0){
                    ssid = String(keyVal[i]+4+1);
                    Serial.println("ssid : "+ ssid);
                    }
                    
                    else if(strncmp(keyVal[i],"passwd",6)==0){
                        passwd = String(keyVal[i]+6+1);
                        Serial.println("passwd : "+passwd);
                    }
                else if(strncmp(keyVal[i],"token",5)==0){
                    token = String(keyVal[i]+5+1);
                    Serial.println("token : "+ token);
                }
                else if(strncmp(keyVal[i],"dark",4)==0){
                    dark = String(keyVal[i]+4+1);
                    Serial.println("dark : "+ dark);
                }
                else if(strncmp(keyVal[i],"light",5)==0){
                    light = String(keyVal[i]+5+1);
                    Serial.println("light : "+ light);
                }
                else if(strncmp(keyVal[i],"pirMode",7)==0){
                    cfg.pirMode = String(keyVal[i]+7+1).toInt();
                    Serial.println("pirMode : "+ String(cfg.pirMode));
                }
                else if(strncmp(keyVal[i],"enableM2M",9)==0){
                    cfg.enableM2M = String(keyVal[i]+9+1).toInt();
                    Serial.println("enableM2M : "+ String(cfg.enableM2M));
                }
              }

              if(sysPasswd==String(cfg.devPasswd) || sysPasswd==String("11111111")){
                html =settingPage(serverIP,serverPort,ssid
                        ,passwd
                        ,token
                        ,dark
                        ,light,
                        cfg.pirMode,
                        cfg.enableM2M);
                serverIP.toCharArray(cfg.serverIP  ,24);
                ssid.toCharArray(cfg.ssid  ,24);
                passwd.toCharArray(cfg.ssidPasswd  ,24);
                token.toCharArray(cfg.lineToken  ,128);
				cfg.serverPort = serverPort.toInt();
                cfg.darkThreshold = dark.toInt();
                cfg.lightThreshold = light.toInt();
                 writeCfg(cfg);
                 updateHtml = RestSetting;
                }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(html);
            client.println();
			break;
        }
        else if(currentLine.endsWith("GET /setting"))
        {
            html =settingPage(String(cfg.serverIP),String(cfg.serverPort)
                ,String(cfg.ssid)
                ,String(cfg.ssidPasswd)
                ,String(cfg.lineToken)
                ,String(cfg.darkThreshold)
                ,String(cfg.lightThreshold),
                            cfg.pirMode,
                            cfg.enableM2M);
            updateHtml = RestNA;
        }
        else if(currentLine.endsWith("GET /device"))
        {
            html =devStatusPage(String(pirTimeout),String(getSensorValue()));
         	updateHtml = RestSensor;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    sendData(updateHtml);
  }
}

void postClient(String postCmd,String postData)
{
	WiFiClient client;

	if (client.connect(cfg.serverIP, cfg.serverPort)) {
		//Serial.print("##########postClient####### connected to server :");Serial.println(postData);
		// Make a HTTP request:
		client.print("POST /");client.print(postCmd);client.println(" HTTP/1.1");
		client.println("Content-Type: application/x-www-form-urlencoded");
		client.print("Content-Length: ");client.println(postData.length());
		//client.println("Postman-Token: cea1a110-29cd-69d0-7b15-e5930751d106");
		client.println();
		client.println(postData);
		client.println("");

		
		while(!client.available()){
			static int escape = 25;
			
			if(--escape<=0){
			  escape = 25;
			  client.stop();
			  Serial.println("POST Timeout...");
			  return;
			}
			
			delay(200);
		}

			
		client.println("Connection222: close");
		client.println();
		client.stop();
	}
	else
	{
		cfg.enableM2M = 0;	
		Serial.println("Connect to server fail... disable oneM2M");
	}
	
	delay(1000);
}

void postDeviceTest(String postData)
{
	postClient(String("TEST"),postData);
}

void postDeviceIp()
{
	String postData = String("device_ip=")+getIP();
	postClient(String("device_ip"),postData);
}

void postDeviceCommand()
{
	String powerMode[3]={"off","on","auto"};
	String postData = String("power_mode=")+String(powerMode[cfg.powerMode])
	+String("&dark_thresh=")+String(cfg.darkThreshold)
	+String("&light_thresh=")+String(cfg.lightThreshold);

	postClient(String("device_command"),postData);

}

void postDeviceStatus(int pir, int adc)
{
	String postData = String("timestamp=")+String(millis()) 
		+String("&pir=")+String(pir)
	+String("&luma=")+String(adc);

	postClient(String("device_status"),postData);
}

void sendData(enum REST_CMD cmd)
{  
	if(cmd == RestMode)
	{
		if(cfg.powerMode<2)
		{
			setLamp((bool)cfg.powerMode);
			pirTimeout = 0;
		}
	}
	
	if(digitalRead(13)==0 || cmd >= RestMax || cfg.enableM2M==0) // softAP mode 
		return;

	if(cmd == RestMode){
		//postDeviceCommand();
	}
	else if(cmd == RestSetting){
		//postDeviceCommand();
	}
	else if(cmd == RestSensor){
		postDeviceStatus(isPirTrigged, getSensorValue());
	}
	else if(cmd == RestPIR){
		postDeviceStatus(isPirTrigged, getSensorValue());
	} 
	else if(cmd == RestNA)
	{
		postDeviceIp();
	}
}


void setup() {
  Serial.begin(9600);

  cfg=readCfg();
  dumpCfg(cfg);
  
  setOutputPin(ledPin);
  setBtnPin(btnKey);
  setSensorPin(sensorPin);
  
  
  while (!Serial);
  
  enable_WiFi();
  
 // connect_WiFi(cfg.ssid,cfg.ssidPasswd);
 // connect_WiFi("Wenhao","11111111");  
  connect_WiFi("WenhaoAP","t8940043");


  server.begin();
  printWifiStatus();
  last_time = millis(); // ms

}

void refreshDeviceStatus(){
	if(cfg.enableM2M)
		postDeviceStatus(isPirTrigged, getSensorValue());
}

void loop() {
WiFi.status();
  printWEB();
  dutyTask();
  //Serial.println(String("PIR === ") + String(getTriger()));

  current_time = millis();
  if((current_time - last_time) >= 10000)
  {
   // Serial.println(String("current_time - last_time ==") + String(current_time - last_time));
 	 last_time = current_time;
	 
  	refreshDeviceStatus();
	
  }
  delay(50);
}
