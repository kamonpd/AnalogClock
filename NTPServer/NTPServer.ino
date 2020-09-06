#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <time.h>

ESP8266WebServer server;
bool flag = true;
uint16_t elapse_time = 0;

int timezone = 7 * 3600;    //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                //กำหนดค่า Date Swing Time

SoftwareSerial mySerial(14, 4); // RX, TX
String mes;

const char* ssid = "Your_SSID";
const char* password = "Your_Password";

void setup() {
  mySerial.begin(115200);
  mySerial.println("Booting");
  
  pinMode(2,OUTPUT);
  digitalWrite(2,1);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    mySerial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266s
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    mySerial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    mySerial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    mySerial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    mySerial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      mySerial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      mySerial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      mySerial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      mySerial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      mySerial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  mySerial.println("Ready");
  mySerial.print("IP address: ");
  mySerial.println(WiFi.localIP());

  server.on("/restart",[](){
    server.send(200,"text/plain", "...Restarting...");
    delay(1000);
    mySerial.println("restart esp8266");
    ESP.restart();
  });

  server.on("/upload",[](){
    server.send(200,"text/plain", "....Going into wireless upload mode....");
    flag = true;
    elapse_time = 0;
    mySerial.println("upload_status");
  });

  server.begin();
  
}

void loop() {
  
  if(flag==true)
  {
    uint16_t start_time = millis();
    while(elapse_time <= 25000)
    {
      ArduinoOTA.handle();
      elapse_time = millis()-start_time;
      delay(10);
    }
    flag = false;
  }
  server.handleClient();

  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");    //ดีงเวลาปัจจุบันจาก Server อีกครั้ง
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  
  mySerial.print(p_tm->tm_hour);
  mySerial.print(":");
  mySerial.print(p_tm->tm_min);
  mySerial.print(":");
  mySerial.print(p_tm->tm_sec);
  mySerial.print(":");
  mySerial.print(p_tm->tm_mday);
  mySerial.print(":");
  mySerial.print(p_tm->tm_mon +1);
  mySerial.print(":");
  mySerial.print(p_tm->tm_year+1900);
  mySerial.println("");
  
  delay(1000);  
}
