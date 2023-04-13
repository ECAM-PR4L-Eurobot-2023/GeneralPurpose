#include "Screen.h"


Screen::Screen() : display(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE)
{}

void Screen::begin()
{
    display.begin();
}

void Screen::draw(int count){
    display.clearBuffer();
    display.setFont(u8g2_font_inr24_mn);
    char buf[4];
    sprintf(buf, "%d", count);
    display.drawStr(70-(20* ((count>9? 1: 0)+(count>99? 1: 0))),44,buf);
    // display.drawStr(20-(20* ((count>9? 1: 0)+(count>99? 1: 0))),20,buf);

    display.sendBuffer();
}