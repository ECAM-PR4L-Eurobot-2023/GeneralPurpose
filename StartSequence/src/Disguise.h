#ifndef DISGUISE_H
#define DISGUISE_H

#include <Arduino.h>

#define Disguised 1
#define notDisguised 0

class Disguise
{
private:    
    int pinA, pinB;
    bool on = false;
    unsigned long disguiseStart = 0;
    bool state = notDisguised;

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