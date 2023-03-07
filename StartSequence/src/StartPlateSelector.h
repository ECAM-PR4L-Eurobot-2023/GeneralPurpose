#ifndef STARTPLATESELECTOR_H
#define STARTPLATESELECTOR_H

#include <Arduino.h>


class StartPlateSelector
{
private:    
    const int potarPin;
    int count, prevCount;

public:
    // Constructor
    StartPlateSelector(int);

    void begin();
    int getPlateNumber();
};

#endif /* STARTPLATESELECTOR_H */