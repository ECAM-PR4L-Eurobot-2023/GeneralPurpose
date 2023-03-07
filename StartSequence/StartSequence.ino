#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "src/StartPlateSelector.h"

#include "src/data/displacement.h"
#include "src/ros_api/msg/Displacement.h"
#include "src/ros_api/ros_api.h"
#include "src/ros_api/topics.h"



#define Potar 34

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C

RosApiCallbacks callbacks {};
RosApi *rosApi;

int count = 0;
int prevCount = 0;
boolean drawn = false;

StartPlateSelector startPlateSelector(Potar);

void setup(void) {
    u8g2.begin();
    startPlateSelector.begin();
    Serial.begin(115200);
}

void loop(void) {
    count = startPlateSelector.getPlateNumber();
    if(prevCount != count){
        drawn = false;
        prevCount = count;
    }

  if(!drawn){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_inr24_mn);
    char buf[4];
    sprintf(buf, "%d", count);
    u8g2.drawStr(70-(20* ((count>9? 1: 0)+(count>99? 1: 0))),44,buf);
    u8g2.sendBuffer();
  }
}