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

int day, month, year, h, m, s;
//int b_h, b_m, b_s, b_day, b_month, b_year;

int b_day = 12;
int b_month = 6;
int b_year = 1995;
int b_h = 7;
int b_m = 0;
int b_s = 0;

int i_year = 65;
double x_s = (((24.00/i_year)*60*60)/31556952);

int a_month[] = {29,31,28,31,30,31,30,31,31,30,31,30,31};

double l_t = 0;
double l_t_h = 0;
double l_t_m = 0;
double l_t_s = 0;

int l_h = 0; 
int l_m = 0; 
int l_s = 0; 
int l_day = 0; 
int l_month = 0; 
int l_year = 0;
int sum_day = 0;
int n_year = 0;
int l_all_s = 0;


SoftwareSerial mySerial(14, 4); // RX, TX
String mes;

const char* ssid = "ssid";
const char* password = "password";

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

  h = p_tm->tm_hour;
  m = p_tm->tm_min;
  s = p_tm->tm_sec;
  day = p_tm->tm_mday;
  month = p_tm->tm_mon +1;
  year = 1900 + p_tm->tm_year;

  callifetime();
  
  mySerial.print(h);
  mySerial.print(":");
  mySerial.print(m);
  mySerial.print(":");
  mySerial.print(s);
  mySerial.print(":");
  mySerial.print(day);
  mySerial.print(":");
  mySerial.print(month);
  mySerial.print(":");
  mySerial.print(year);
  mySerial.print(":");
  mySerial.print((int)l_t_h);
  mySerial.print(":");
  mySerial.print((int)l_t_m);
  mySerial.print(":");
  mySerial.print((int)l_t_s);
  mySerial.println("");
  
  delay(1000);  
}
