#include "webSvr.h"

/*========================
 * Web Serice
 * void create_Json()
 * void RESTinit()
=========================*/
#if 0
StaticJsonDocument<250> jsonDocument;
char buffer[250];

void create_json(char *tag, float value, char *unit) {  
  jsonDocument.clear();
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);  
}

void add_json_object(char *tag, float value, char *unit) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit; 
}
#endif



//=====================
// Web Pages
//=====================
String headPage()
{
  String msg =    "<h3><table border='1'><tr>";
    msg +=    "<td><form action='system'><input type='submit' style='width:100px;height:50px;font-size:18px' value='SysInfo'></form></td>";
    msg +=    "<td><form action='powerMode'><input type='submit' style='width:100px;height:50px;font-size:18px' value='Mode'></form></td>";
    msg +=    "<td><form action='setting'><input type='submit' style='width:100px;height:50px;font-size:18px' value='Setting'></form></td>";
    msg +=    "<td><form action='device'><input type='submit' style='width:100px;height:50px;font-size:18px' value='DevStatus'></form></td>";
    msg +=    "</tr></table></h3><hr>";

    return msg;
}


String sysInfoPage(String strMyAPIP_, String strMyIP, uint32_t fwVer)
{
    String msg = headPage();

        msg += "<h2>System Infomation</h2><br>";
        msg +=    "<h3><table border='1' style='font-size:20px'>";
        //msg +=    "<tr><td>Chip model </td><td>" + String(ESP.getChipModel()) + "</td></tr>";
        msg +=    "<tr><td>IP </td><td>" + strMyAPIP_ + "</td></tr>";
        msg +=    "<tr><td>MAC </td><td>" + strMyIP+  "</td></tr>";
        msg +=    "<tr><td>FW Ver</td><td>" + String(fwVer)+ "</td></tr>";
        msg +=    "</table></h3><hr>";

        return msg;
        
}

String powerModePage(int powerMode)
{
  String msg = headPage();
        msg += "<h2>Power Mode</h2><br>";
        msg +=    "<h3><table border='1'><tr>";
        
        msg +=    "<td><form action='on'><input type='submit' style='width:88px;height:50px;font-size:20px";
        if(powerMode==1)
          msg += ";background-color:yellow' value='Led On'></form></td>";
        else msg += "' value='Led On'></form></td>";
        
        msg +=    "<td><form action='off'><input type='submit' style='width:88px;height:50px;font-size:20px";
        if(powerMode==0)
          msg += ";background-color:yellow' value='Led Off'></form></td>";
        else msg += "' value='Led Off'></form></td>";
        
        msg +=    "<td><form action='auto'><input type='submit' style='width:88px;height:50px;font-size:20px";
        if(powerMode==2)
          msg += ";background-color:yellow' value='Auto'></form></td>";
        else msg += "' value='Auto'></form></td>";
        
        msg +=    "</tr></table></h3><hr>";
   
        return msg;
}

String settingPage(
    String serverIP,
    String serverPort, 
    String ssid, 
    String ssid_passwd, 
    String lineToken_,
    String dark_threshold,
    String light_threshold,
    int pirMode, 
    int enableM2M)
{
  String msg = headPage();

          msg += "<h2>Device Setting</h2><br>";
          msg +=    "<form action='settingX' method='post'>";
          msg +=    "<div  style='width:880px;height:50px;font-size:20px'><table border='1'>";
          msg +=    "<tr>Password : <input type='password' name='sysPasswd' value='' /></tr><br>";
           msg +=    "<tr>serverIP : <input type='text' name='serverIP' value="+serverIP+" /><input type='text' name='serverPort' value="+serverPort+" /></tr><br>";
          msg +=    "<tr>SSID : <input type='text' name='ssid' value="+ssid+" /></tr><br>";
          msg +=    "<tr>SSID PW : <input type='password' name='passwd' value="+ssid_passwd+" /></tr><br>";
          //msg +=    "<tr>token : <input type='text' name='token' value="+lineToken_+" /></tr><br>";
          msg +=    "<tr>Dark Threshold : <input type='text' name='dark' value="+ dark_threshold+" /></tr><br>";
          msg +=    "<tr>Light Threshold : <input type='text' name='light' value="+light_threshold+" /></tr><br>";

          msg +=    "<tr>PIR Mode :";
          if(pirMode == 0)
            {
                msg +="<label><input type='radio' name='pirMode' value=0 checked>Off</label>"
                    "<label><input type='radio' name='pirMode' value=1>On</label></tr><br>";
            }
          else
            {
                msg +="<label><input type='radio' name='pirMode' value=0>Off</label>"
                    "<label><input type='radio' name='pirMode' value=1 checked>On</label></tr><br>";
            }

          msg +=    "<tr>oneM2M :";
          if(enableM2M == 0)
            {
                msg +="<label><input type='radio' name='enableM2M' value=0 checked>Disable</label>"
                    "<label><input type='radio' name='enableM2M' value=1>Enable</label></tr><br>";
            }
          else
            {
                msg +="<label><input type='radio' name='enableM2M' value=0>Disable</label>"
                    "<label><input type='radio' name='enableM2M' value=1 checked>Enable</label></tr><br>";
            }

          msg +=    "<input type='submit'  style='width:88px;height:50px;font-size:20px' value='Save'/>";
          msg +=    "</table></div></form>";
           
        return msg;
}


String devStatusPage(String strPirState, String strAdcState)
{
  String msg = headPage();
        msg += "<h2>Device Status</h2><br>";
        msg +=    "<h3><table border='1' style='font-size:20px'>";
        msg +=    "<tr><td>Sensor </td><td>Value</td></tr>";
        msg +=    "<tr><td>PIR </td><td>" + strPirState+ "</td></tr>";
        msg +=    "<tr><td>ADC33 </td><td>" +strAdcState+ "</td></tr>";
        msg +=    "</table></h3>";

        //msg += "<h2>Export Logs</h2><br>";
        msg +=    "<form action='device'><input type='submit' style='width:188px;height:50px;font-size:20px' value='Log Export'></form><br>";
        msg +=    "<hr>";

        return msg;
}


