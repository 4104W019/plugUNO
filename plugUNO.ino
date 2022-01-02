#include "config.h"

void dutyTask()
{
	lightSensorVaule = getSensorValue();
	lightSensorVaule= map(lightSensorVaule,0,1023,0,100);
	int dark = cfg.darkThreshold; 
	int light = cfg.lightThreshold;
	//Serial.println(String("lightSensorVaule === ") + String(lightSensorVaule));

    if(WiFi.status()!= WL_CONNECTED || cfg.powerMode == 2)
    {
		if(lightSensorVaule >= light){
			if(lampState()){
				Serial.println(String("lightSensorVaule === ") + String(lightSensorVaule));
				setLamp(false);
				sendData(RestSensor);
			}
		}
		else if(lightSensorVaule < light)
		{
			if(cfg.pirMode==0)
			{
				if(!lampState() && lightSensorVaule < dark){
					Serial.println(String("lightSensorVaule === ") + String(lightSensorVaule));
					setLamp(true);
					sendData(RestSensor);
				}
			}
			else
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
    }
	else{
		if(lampState() != cfg.powerMode){
			setLamp(cfg.powerMode);
			sendData(RestSensor);
		}
	}
	    
}

void printWEB() {
  WiFiClient client= server.available();

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port

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
        else if (currentLine.endsWith("POST /settingX"))
            {
                 char *keyVal[12];
                  
                  String postForm  = client.readString();

                  int idx = postForm.indexOf("sysPasswd");
                  postForm = postForm.substring(idx);
                  //Serial.println(F("final request 222333: "));Serial.println(postForm);
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

	String postData;//aaa=2&bbb=3
	if(cmd == RestMode)
		postData = String("CMD=PowerMode&VALUE=")+String(cfg.powerMode);
	else if(cmd == RestSetting)
		postData = String("CMD=Setting&DarkThres=")
					+ String(cfg.darkThreshold)
					+ String("&LightThres=")
					+ String(cfg.lightThreshold);
	else if(cmd == RestSensor)
		postData = String("CMD=Seneor&ADC=")+String(getSensorValue());
	else if(cmd == RestPIR)
		postData = String("CMD=PIR&VAL=")+String(isPirTrigged);

      WiFiClient client;
      //Serial.println("\nStarting connection to server..."+ String(cfg.serverIP));
      // if you get a connection, report back via serial:
      if (client.connect(cfg.serverIP, cfg.serverPort)) {
        Serial.print("connected to server :");Serial.println(postData);
        // Make a HTTP request:
        client.println("POST /TEST HTTP/1.1");
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
//      
//        while (client.available()) {
//          char c = client.read();
//          Serial.print(c);
//        }
        
        client.println("Connection: close");
        client.println();
        client.stop();
      }
      else
      {
        Serial.println(client.connect(cfg.serverIP, cfg.serverPort));
      }
      delay(1000);
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
  connect_WiFi(cfg.ssid,cfg.ssidPasswd);

  server.begin();
  printWifiStatus();

}

void loop() {
WiFi.status();
  printWEB();
  dutyTask();
  //Serial.println(String("PIR === ") + String(getTriger()));

  delay(50);
}
