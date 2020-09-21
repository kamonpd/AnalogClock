/**************************************************
  Base on: https://ceezblog.info/2015/05/15/arduino-analog-clock-with-ds1307/
  
  Title: Clock Life
  Author: kamonpd
  Version: 1.0
  Update: 18/7/2020
 
   Modules:
 - Arduino Uno
 - LCD 3.5" TFT module 320X480px for Arduino Uno

  Wiring:
  - TFT display     Arduino Uno
          GND       -   GND
          VCC       -   3.3v
          LCD_RESET -   A4
          LCD_CS    -   A3
          LCD_CD    -   A2
          LCD_WR    -   A1
          LCD_RD    -   A0
  
///**************************************************/

  #include "Adafruit_GFX.h"
  #include <MCUFRIEND_kbv.h>
  #include "struct.h"
  #include <fonts/FreeMono9pt7b.h>
  
  #define TME tmElements_t
  
  #define BLACK           0x0000      /*   0,   0,   0 */
  #define NAVY            0x000F      /*   0,   0, 128 */
  #define DARKGREEN       0x03E0      /*   0, 128,   0 */
  #define DARKCYAN        0x03EF      /*   0, 128, 128 */
  #define MAROON          0x7800      /* 128,   0,   0 */
  #define PURPLE          0x780F      /* 128,   0, 128 */
  #define OLIVE           0x7BE0      /* 128, 128,   0 */
  #define LIGHTGREY       0xC618      /* 192, 192, 192 */
  #define DARKGREY        0x7BEF      /* 128, 128, 128 */
  #define BLUE            0x001F      /*   0,   0, 255 */
  #define GREEN           0x07E0      /*   0, 255,   0 */
  #define CYAN            0x07FF      /*   0, 255, 255 */
  #define RED             0xF800      /* 255,   0,   0 */
  #define MAGENTA         0xF81F      /* 255,   0, 255 */
  #define YELLOW          0xFFE0      /* 255, 255,   0 */
  #define WHITE           0xFFFF      /* 255, 255, 255 */
  #define ORANGE          0xFD20      /* 255, 165,   0 */
  #define GREENYELLOW     0xAFE5      /* 173, 255,  47 */
  
  #define CREF_BACKGROUND BLACK
  #define FACE            ORANGE
  #define CREF_SECOND     RED
  #define CREF_MINUTE     CYAN
  #define CREF_HOUR       CYAN
  #define NUMERIC_POINT   WHITE
  #define CREF_HELLO      RED
  #define CREF_TEXT       DARKCYAN
  #define CREF_TEXT_BRAND ORANGE
  #define CREF_TIME       MAGENTA
  #define CREF_DATE       GREENYELLOW
  
  #define MINUTE_HAND  0
  #define HOUR_HAND  1
  
  // CONFIG YOUR CHANGE HERE
  #define Xo 240                // center point
  #define Yo 160                // center point
  #define RADIUS 130            // radius of the clock face
  
  #define S_LEN  120             // second hand
  #define S_TAIL  30
  
  #define M_LEN  110             // minute hand
  #define M_TAIL  21
  #define M_SIDE  8
  
  #define H_LEN  70             // hour hand
  #define H_TAIL  18
  #define H_SIDE  12
  
  #define TEXT_SIZE 1
  
  char digit_ch[3]={'0','0',NULL}; // a buffer for convert number to char array
  
  #define LCD_RESET A4          // Can alternately just connect to Arduino's reset pin
  #define LCD_CS A3             // Chip Select goes to Analog 3
  #define LCD_CD A2             // Command/Data goes to Analog 2
  #define LCD_WR A1             // LCD Write goes to Analog 1
  #define LCD_RD A0             // LCD Read goes to Analog 0
  
  HAND_SET o_hands;
  HAND_SET n_hands;
  
  TME lo_tme;
  TME ln_tme;
  
  TME n_tme;
  TME o_tme;
  
  MCUFRIEND_kbv tft;

  void setup(){        
    
    Serial.begin(115200);
    Serial.println(F("TFT LCD test"));
    tft.reset();
    identifier();
  
    initSetting();
    copyHandSet(o_hands, n_hands);
    copyTME(o_tme, n_tme);
    calc_Hands(n_hands, ln_tme); 
  }

  void loop(){    
    
    while(Serial.available()){
      String msg = Serial.readStringUntil('\n');
      msg.trim();
      Serial.println(msg);
      
      n_tme.Hour = getValue(msg,':', 0).toInt(); 
      n_tme.Minute = getValue(msg,':', 1).toInt();
      n_tme.Second = getValue(msg,':', 2).toInt();
      n_tme.Day = getValue(msg,':', 3).toInt(); 
      n_tme.Month = getValue(msg,':',4).toInt();  
      n_tme.Year = getValue(msg,':', 5).toInt();
      
      ln_tme.Hour = getValue(msg,':', 6).toInt();             // time life
      ln_tme.Minute = getValue(msg,':',7).toInt();  
      ln_tme.Second = getValue(msg,':', 8).toInt();

//      ln_tme.Hour = getValue(msg,':', 0).toInt();             // test time life
//      ln_tme.Minute = getValue(msg,':',1).toInt();  
//      ln_tme.Second = getValue(msg,':', 2).toInt();
       
//      Serial.println(n_tme.Hour);
//      Serial.println(n_tme.Minute);
//      Serial.println(n_tme.Second);
//      Serial.println(n_tme.Day);
//      Serial.println(n_tme.Month);
//      Serial.println(n_tme.Year);
 
           
    }

  if(o_tme.Second != n_tme.Second){           // second change or time ticked  
    TextSecond(o_tme.Second, CREF_BACKGROUND);    
    TextSecond(n_tme.Second, CREF_TIME);
    
   
    if(o_tme.Minute != n_tme.Minute){
      TextMinute(o_tme.Minute, CREF_BACKGROUND);
      TextMinute(n_tme.Minute, CREF_TIME);
    }

    if(o_tme.Hour != n_tme.Hour){
      TextHour(o_tme.Hour, CREF_BACKGROUND);
      TextHour(n_tme.Hour, CREF_TIME);
    }

    if(o_tme.Day != n_tme.Day 
      || o_tme.Month != n_tme.Month 
      || o_tme.Year != n_tme.Year){                
        printDate(o_tme.Day, o_tme.Month, o_tme.Year, CREF_BACKGROUND);
        printDate(n_tme.Day, n_tme.Month, n_tme.Year, CREF_DATE);
    }

    
          
      
    if(lo_tme.Second != ln_tme.Second){
      calc_Hands(n_hands, ln_tme);
      int x = ln_tme.Second;
      
      if(x>=50||x<=10){
        drawBrand();
      }
  
      if (x>=20||x<=40){
        TextSecond(n_tme.Second, CREF_TIME);
        TextMinute(n_tme.Minute, CREF_TIME);
        TextHour(n_tme.Hour, CREF_TIME);
        printDate(n_tme.Day, n_tme.Month, n_tme.Year, CREF_DATE);
        showTimeColon();
      }
  
      cdraw_SecondHand(); 
                                 
      if(lo_tme.Minute != ln_tme.Minute) {
        cdraw_MinuteHand();
        cdraw_HourHand();                         
      }

     if (lo_tme.Hour != ln_tme.Hour){
        cdraw_HourHand();         
      }
      
      draw_NewHourHand();      
      draw_NewMinuteHand();
      draw_NewSecondHand(); 
      tft.fillCircle(Xo, Yo-1, 4, FACE);        // draw center dot
      
      copyTME(lo_tme, ln_tme);
      copyHandSet(o_hands, n_hands);
    }

    copyTME(o_tme, n_tme);

  }
      
//      copyHandSet(o_hands, n_hands);
//      copyTME(o_tme, n_tme);
//      copyTME(lo_tme, ln_tme);  
                    
    delay(200);
  }
