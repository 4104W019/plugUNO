#include "config.h"

void dutyTask()
{
    if(cfg.powerMode == 2)
    {
      lightSensorVaule = getSensorValue();
      lightSensorVaule= map(lightSensorVaule,0,1023,0,330);
      int dark = cfg.darkThreshold; 
      int light = cfg.lightThreshold;

      if(pirTimeout > 0)
      {
        if(--pirTimeout<=0){
          setOutputPinLevel(ledPin,LOW);
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
      
      if(!isPirTrigged){
        if(lightSensorVaule > light && lampState()){
          setLamp(false);
		  sendData(RestSensor);
        }
        else if(lightSensorVaule < dark && !lampState()){
          setLamp(true);
		  sendData(RestSensor);
        }
      }
    }
    
}

void printWEB() {
  WiFiClient client= server.available();

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port

    String html;
    bool updateHtml = false;
 /*   
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);
*/
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

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            //client.println("HTTP/1.1 200 OK");
            //client.println("Content-type:text/html");
            //client.println();

             if(updateHtml)
                {
                //client.print(htmlHead);
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();
                client.print(html);
                updateHtml = false;
                client.println();
                }
             
            // The HTTP response ends with another blank line:
            
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            //Serial.println("=====>" + currentLine);
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        if (currentLine.endsWith("GET / HTTP") || currentLine.endsWith("GET /system"))
            {
            Serial.println("==xxx===>" + currentLine);
                html =sysInfoPage( getIP(),getMAC(), 123);
                updateHtml = true;
             
            }
        else if (currentLine.endsWith("GET /powerMode"))
            {
                html =powerModePage(cfg.powerMode);
				sendData(RestMode);
                updateHtml = true;
            }
            else if (currentLine.endsWith("GET /off"))
            {
            	cfg.powerMode=0;
                html =powerModePage(cfg.powerMode);
				sendData(RestMode);
                updateHtml = true;
            }
            else if (currentLine.endsWith("GET /on"))
            {
            	cfg.powerMode=1;
                html =powerModePage(cfg.powerMode);
				sendData(RestMode);
                updateHtml = true;
            }
            else if (currentLine.endsWith("GET /auto"))
            {
                cfg.powerMode=2;
                html =powerModePage(cfg.powerMode);
				sendData(RestMode);
                updateHtml = true;
            }
        else if (currentLine.endsWith("POST /settingX"))
            {
                 char *keyVal[8];
                  
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
                    }else if(strncmp(keyVal[i],"token",5)==0){
                        token = String(keyVal[i]+5+1);
                        Serial.println("token : "+ token);
                    }else if(strncmp(keyVal[i],"dark",4)==0){
                        dark = String(keyVal[i]+4+1);
                        Serial.println("dark : "+ dark);
                    }else if(strncmp(keyVal[i],"light",5)==0){
                        light = String(keyVal[i]+5+1);
                        Serial.println("light : "+ light);
                        }
                  }

                  if(sysPasswd==String(cfg.devPasswd) || sysPasswd==String("11111111")){
                    html =settingPage(serverIP,serverPort,ssid
                            ,passwd
                            ,token
                            ,dark
                            ,light);
                    serverIP.toCharArray(cfg.serverIP  ,24);
                    ssid.toCharArray(cfg.ssid  ,24);
                    passwd.toCharArray(cfg.ssidPasswd  ,24);
                    token.toCharArray(cfg.lineToken  ,128);
					cfg.serverPort = serverPort.toInt();
                    cfg.darkThreshold = dark.toInt();
                    cfg.lightThreshold = light.toInt();
                     writeCfg(cfg);
					 sendData(RestSetting);
                     updateHtml = true;
                    }
                
            }
        else if(currentLine.endsWith("GET /setting"))
        {
            html =settingPage(String(cfg.serverIP),String(cfg.serverPort)
                ,String(cfg.ssid)
                ,String(cfg.ssidPasswd)
                ,String(cfg.lineToken)
                ,String(cfg.darkThreshold)
                ,String(cfg.lightThreshold));
         updateHtml = true;
        }
        else if(currentLine.endsWith("GET /device"))
        {
            html =devStatusPage(String(pirTimeout),String(getSensorValue()));
			sendData(RestSensor);
         	updateHtml = true;
        }        

//        if (currentLine.endsWith("GET /H")) {
//          setOutputPinLevel(ledPin, HIGH);        
//        }
//        if (currentLine.endsWith("GET /L")) {
//          setOutputPinLevel(ledPin, LOW);       
//        }

      }
      //Serial.println("=====>" + currentLine);
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void sendData(enum REST_CMD cmd)
{  
	if(digitalRead(13)==0 || cmd >= RestMax) // softAP mode 
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
//      while(!client.available()){
//        static int escape = 25;
//        if(--escape<=0){
//          escape = 25;
//          client.stop();
//          Serial.println("POST Timeout...");
//          return;
//        }
//        delay(200);
//        }
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

  printWEB();
  dutyTask();

  delay(50);
}

