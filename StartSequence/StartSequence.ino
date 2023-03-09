#include <Arduino.h>

#include "src/StartPlateSelector.h"

#include "src/data/displacement.h"
#include "src/ros_api/msg/Displacement.h"
#include "src/ros_api/ros_api.h"
#include "src/ros_api/topics.h"

#include "src/screen.h"



#define potar 34

#define startCord

Screen screen;

RosApiCallbacks callbacks {};
RosApi *rosApi;

int count = 0;
int prevCount = 0;
boolean drawn = false;

StartPlateSelector startPlateSelector(potar);

void setup(void) {
    screen.begin();
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
    screen.draw(count);
  }
}
