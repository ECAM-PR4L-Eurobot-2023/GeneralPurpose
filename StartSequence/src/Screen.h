#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>


class Screen
{
private:    
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C display;

public:
    // Constructor
    Screen();

    void begin();
    void draw(int);
};

#endif /* SCREEN_H */