#include "StartPlateSelector.h"


StartPlateSelector::StartPlateSelector(int potarPin) : potarPin(potarPin)
{}

void StartPlateSelector::begin()
{
    pinMode(potarPin, INPUT);
    count = 0;
    prevCount = 0;
}

int StartPlateSelector::getPlateNumber()
{
    return analogRead(potarPin)/455;
}