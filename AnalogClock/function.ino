
  void identifier(){
    uint16_t identifier = tft.readID();
    if(identifier == 0x9325){
      Serial.println(F("Found ILI9325 LCD driver"));
    } 
    else if(identifier == 0x9328){      
      Serial.println(F("Found ILI9328 LCD driver"));
    } 
    else if(identifier == 0x4535){
      Serial.println(F("Found LGDP4535 LCD driver"));
    } 
    else if(identifier == 0x7575){
      Serial.println(F("Found HX8347G LCD driver"));
    }
    else if(identifier == 0x9595){      
      Serial.println(F("Found HX8347-I LCD driver"));
    }
    else if(identifier == 0x4747){
      Serial.println(F("Found HX8347-D LCD driver"));
    }     
    else if(identifier == 0x8347){
      Serial.println(F("Found HX8347-A LCD driver"));
    } 
    else if(identifier == 0x9341){
      Serial.println(F("Found ILI9341 LCD driver"));
    } 
    else if(identifier == 0x7783){
      Serial.println(F("Found ST7781 LCD driver"));
    } 
    else if(identifier == 0x8230){
      Serial.println(F("Found UC8230 LCD driver"));
    } 
    else if(identifier == 0x8357){
      Serial.println(F("Found HX8357D LCD driver"));
    } 
    else if(identifier == 0x0101){
      identifier = 0x9341;
      Serial.println(F("Found 0x9341 LCD driver"));
    } 
    else if(identifier == 0x7793){
      Serial.println(F("Found ST7793 LCD driver"));
    } 
    else if(identifier == 0xB509){
      Serial.println(F("Found R61509 LCD driver"));
    } 
    else if(identifier == 0x9486){
      Serial.println(F("Found ILI9486 LCD driver"));
    } 
    else if(identifier == 0x9488){
      Serial.println(F("Found ILI9488 LCD driver"));
    } 
    else{
      Serial.print(F("Unknown LCD driver chip: "));
      Serial.println(identifier, HEX);
      Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
      Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
      Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
      Serial.println(F("If using the breakout board, it should NOT be #defined!"));
      Serial.println(F("Also if using the breakout, double-check that all wiring"));
      Serial.println(F("matches the tutorial."));
      identifier = 0x9486;
    }
    tft.begin(identifier);
  }

  void initSetting(){
    tft.setRotation(1);                   // Lanscape
    tft.fillScreen(CREF_BACKGROUND);
    tft.setTextSize(TEXT_SIZE);
    tft.setFont(&FreeMono9pt7b);
  
    calc_Hands(n_hands, n_tme);              
    drawFace();
    drawBrand();
  
    draw_NewHourHand();                   // draw hour hand
    draw_NewMinuteHand();
    draw_NewSecondHand();

    TextSecond(o_tme.Second, CREF_BACKGROUND);    
    TextSecond(n_tme.Second, CREF_TIME);
    TextMinute(o_tme.Minute, CREF_BACKGROUND);
    TextMinute(n_tme.Minute, CREF_TIME);
    TextHour(o_tme.Hour, CREF_BACKGROUND);
    TextHour(n_tme.Hour, CREF_TIME);

    printDate(o_tme.Day, o_tme.Month, o_tme.Year, CREF_BACKGROUND);
    printDate(n_tme.Day, n_tme.Month, n_tme.Year, CREF_DATE);
  }

  void drawFace(){
    int i = 0, angle = 0;
    float x, y;

    // Draw outer frame
    tft.drawCircle(Xo, Yo, RADIUS + 21, FACE);
    tft.drawCircle(Xo, Yo, RADIUS + 20, FACE);
  
    // Draw inner frame
    tft.drawCircle(Xo, Yo, RADIUS + 12, FACE);
    tft.drawCircle(Xo, Yo, RADIUS + 11, FACE);
    tft.drawCircle(Xo, Yo, RADIUS + 10, FACE);
  
    //Draw Numeric point
    for (i = 0; i <= 12; i++){
      x = Xo + RADIUS * cos(angle * M_PI / 180);
      y = Yo + RADIUS * sin(angle * M_PI / 180);
      tft.drawCircle(x, y, 2, NUMERIC_POINT);
      angle += 30;
    }
  
    for (i = 0; i < 360; i += 6) {
      tft.drawPixel(Xo + RADIUS * cos(i * M_PI / 180), Yo + RADIUS * sin(i * M_PI / 180), NUMERIC_POINT);
    }
    tft.fillCircle(Xo, Yo-1, 3, FACE);        // draw center dot
  }

  void drawBrand(){
    tft.setTextColor(CREF_TEXT_BRAND);
    tft.setTextSize(0);
    tft.setCursor(Xo-92, 90);
    tft.print("Analog Clock V0.1");
  }

  void calc_SecondHand(TME t, LINE &ps ){
    float angle; // in radian
    //  int Xa, Ya, Xb, Yb;
    angle =  t.Second * 0.1044 - 1.566;         // if 1 sec : angle = -1.4616
    ps.a.x = Xo + (S_LEN) * cos(angle);         // 240 +(75)*0.9997 = 314.9055 
    ps.a.y = Yo + (S_LEN) * sin(angle);         // 160 +(75)*-0.0255= -5.9925
    angle += 3.142; // +180 degree
    ps.b.x = Xo + (S_TAIL) * cos(angle);
    ps.b.y = Yo + (S_TAIL) * sin(angle);
  }

  void calc_Hands(HAND_SET &hs, TME t){
    calc_SecondHand(t, hs.Sec);
    calc_HourMinHand(MINUTE_HAND, t, hs.Min);
    calc_HourMinHand(HOUR_HAND, t, hs.Hour);
  }

// type = 0 -> Minute
// type = 1 -> Hour

  void calc_HourMinHand(uint8_t hour_hand, TME t, HAND_POINTS &ps){
    float angle;
    if (hour_hand == HOUR_HAND) // 1 == hour, 0 == minute
      angle = t.Hour * 0.524 + t.Minute * 0.0087 - 1.571; // (theta + h*30 + (m*30/60))* M_PI/180
    else
      angle = t.Minute * 0.1044 - 1.571;  // (theta + n_tme.Minute*6)*M_PI/180
  
    ps.a.x = Xo + ((hour_hand) ? H_LEN : M_LEN) * cos(angle);
    ps.a.y = Yo + ((hour_hand) ? H_LEN : M_LEN) * sin(angle);
    angle += 3.142; //+180 degree to get the tail
    ps.b.x = Xo + ((hour_hand) ? H_TAIL : M_TAIL) * cos(angle);
    ps.b.y = Yo + ((hour_hand) ? H_TAIL : M_TAIL) * sin(angle);
    angle += 1.571; //+90 degree to get the side point
    ps.e.x = Xo + ((hour_hand) ? H_SIDE : M_SIDE) * cos(angle);
    ps.e.y = Yo + ((hour_hand) ? H_SIDE : M_SIDE) * sin(angle);
    angle += 3.142; //+180 degree to get other side point
    ps.f.x = Xo + ((hour_hand) ? H_SIDE : M_SIDE) * cos(angle);
    ps.f.y = Yo + ((hour_hand) ? H_SIDE : M_SIDE) * sin(angle);
  }

  void draw_NewHand(HAND_POINTS ps, int color){
    tft.drawLine(ps.a.x, ps.a.y, ps.e.x, ps.e.y, color);
    tft.drawLine(ps.a.x, ps.a.y, ps.f.x, ps.f.y, color);
    tft.drawLine(ps.b.x, ps.b.y, ps.e.x, ps.e.y, color);
    tft.drawLine(ps.b.x, ps.b.y, ps.f.x, ps.f.y, color);
  }

  void draw_NewMinuteHand(){
    draw_NewHand(n_hands.Min, CREF_MINUTE);
  }

  void draw_NewHourHand(){
    draw_NewHand(n_hands.Hour, CREF_HOUR);
  }
  
  void draw_NewSecondHand(){
    tft.drawLine(n_hands.Sec.a.x, n_hands.Sec.a.y, n_hands.Sec.b.x, n_hands.Sec.b.y, CREF_SECOND);
    tft.fillCircle(n_hands.Sec.b.x, n_hands.Sec.b.y, 3, CREF_SECOND);
  }
  
  void cdraw_SecondHand(){
    tft.fillCircle(o_hands.Sec.b.x, o_hands.Sec.b.y, 3, CREF_BACKGROUND); 
    tft.fillCircle(n_hands.Sec.b.x, n_hands.Sec.b.y, 3, CREF_SECOND);
    tft.drawLine(o_hands.Sec.a.x, o_hands.Sec.a.y, o_hands.Sec.b.x, o_hands.Sec.b.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Sec.a.x, n_hands.Sec.a.y, n_hands.Sec.b.x, n_hands.Sec.b.y, CREF_SECOND);
    tft.fillCircle(n_hands.Sec.b.x, n_hands.Sec.b.y, 3, CREF_SECOND);
  }

  void cdraw_MinuteHand(){
    tft.drawLine(o_hands.Min.b.x, o_hands.Min.b.y, o_hands.Min.f.x, o_hands.Min.f.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Min.b.x, n_hands.Min.b.y, n_hands.Min.f.x, n_hands.Min.f.y, CREF_MINUTE);
    
    tft.drawLine(o_hands.Min.b.x, o_hands.Min.b.y, o_hands.Min.e.x, o_hands.Min.e.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Min.b.x, n_hands.Min.b.y, n_hands.Min.e.x, n_hands.Min.e.y, CREF_MINUTE);
  
    tft.drawLine(o_hands.Min.a.x, o_hands.Min.a.y, o_hands.Min.e.x, o_hands.Min.e.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Min.a.x, n_hands.Min.a.y, n_hands.Min.e.x, n_hands.Min.e.y, CREF_MINUTE);
    
    tft.drawLine(o_hands.Min.a.x, o_hands.Min.a.y, o_hands.Min.f.x, o_hands.Min.f.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Min.a.x, n_hands.Min.a.y, n_hands.Min.f.x, n_hands.Min.f.y, CREF_MINUTE);
  }
  
  void cdraw_HourHand(){
    tft.drawLine(o_hands.Hour.b.x, o_hands.Hour.b.y, o_hands.Hour.f.x, o_hands.Hour.f.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Hour.b.x, n_hands.Hour.b.y, n_hands.Hour.f.x, n_hands.Hour.f.y, CREF_HOUR);
    
    tft.drawLine(o_hands.Hour.b.x, o_hands.Hour.b.y, o_hands.Hour.e.x, o_hands.Hour.e.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Hour.b.x, n_hands.Hour.b.y, n_hands.Hour.e.x, n_hands.Hour.e.y, CREF_HOUR);
  
    
    tft.drawLine(o_hands.Hour.a.x, o_hands.Hour.a.y, o_hands.Hour.e.x, o_hands.Hour.e.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Hour.a.x, n_hands.Hour.a.y, n_hands.Hour.e.x, n_hands.Hour.e.y, CREF_HOUR);
    
    tft.drawLine(o_hands.Hour.a.x, o_hands.Hour.a.y, o_hands.Hour.f.x, o_hands.Hour.f.y, CREF_BACKGROUND);
    tft.drawLine(n_hands.Hour.a.x, n_hands.Hour.a.y, n_hands.Hour.f.x, n_hands.Hour.f.y, CREF_HOUR);
  }

  // convert minute from INT to CHAR
  void conv_num2char(int min){
    char digit[10] = {'0','1','2','3','4','5','6','7','8','9'};
    char* ret;
    digit_ch[0] = '0';
    digit_ch[1] = '0';
    //digit_ch[2]=NULL;
    if (min > 59) return;
    digit_ch[0]=digit[min/10]; 
    digit_ch[1]= digit[min - min/10*10];
    digit_ch[2]=NULL;
  //  ret[2[= NULL;
    //return ret;
  }

  // Print Date in format DD-MM-YYYY
  void printDate(int d, int m, int y, int color){
    tft.setTextColor(color); //CREF_DATE);
    tft.setCursor(Xo-45, Yo+80);
    tft.print(d); tft.print('-');
    tft.print(m); tft.print('-');
    tft.print(y); 
  }

  void TextHour(int number, int color){
    tft.setCursor(Xo-38, Yo+55);
    conv_num2char(number);
    tft.setTextColor(color);
    tft.print(digit_ch);
  }
  
  void TextMinute(int number, int color){
    tft.setCursor(Xo-10, Yo+55);
    conv_num2char(number);
    tft.setTextColor(color);
    tft.print(digit_ch);
  }
  
  void TextSecond(int number, int color){
    tft.setCursor(Xo+20, Yo+55);
    conv_num2char(number);
    tft.setTextColor(color);
    tft.print(digit_ch);
  }
  
  void showTimeColon(){
    tft.setTextColor(CREF_TIME);
    tft.setCursor(Xo-18, Yo+55);
    tft.print(":");
    tft.setCursor(Xo+12, Yo+55);
    tft.print(":");
  }

  String getValue(String data, char separator, int index){
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;
  
    for(int i=0; i<=maxIndex && found<=index; i++){
      if(data.charAt(i)==separator || i==maxIndex){
          found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
    }
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
  }
  
