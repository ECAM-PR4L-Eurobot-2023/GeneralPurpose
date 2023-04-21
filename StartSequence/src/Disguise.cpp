#include "Disguise.h"

#define TIMEON 5000

Disguise::Disguise(int pinA, int pinB) : pinA(pinA), pinB(pinB)
{}

void Disguise::begin()
{
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
}

void Disguise::disguise()
{
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    on = true;
    state = Disguised;
    disguiseStart = millis();
}

void Disguise::release()
{
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    state = notDisguised;
    on = true;
    disguiseStart = millis();
}

void Disguise::turnOff()
{
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    on = false;
}

void Disguise::run(){
    if (on && millis()-disguiseStart > TIMEON)
    {
        turnOff();
        if (state){
            release();
        }
    }
}
