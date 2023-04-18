#ifndef DISGUISE_H
#define DISGUISE_H

#include <Arduino.h>

class Disguise
{
private:    
    int pinA, pinB;
    bool on = false;
    unsigned long disguiseStart = 0;

public:
    // Constructor
    Disguise(int, int);

    void begin();
    void disguise();
    void release();
    void turnOff();
    void run();
};

#endif /* DISGUISE_H */