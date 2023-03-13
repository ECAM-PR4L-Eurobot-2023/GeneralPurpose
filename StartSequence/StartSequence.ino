#include <Arduino.h>

#include "src/StartPlateSelector.h"

#include "src/data/displacement.h"
#include "src/ros_api/msg/Displacement.h"
#include "src/ros_api/ros_api.h"
#include "src/ros_api/topics.h"

#include "src/screen.h"



#define potar 36

#define startCord 32

Screen screen;

RosApiCallbacks callbacks {};
RosApi *rosApi;

int count = 0;
int prevCount = 0;
boolean drawn = false;

StartPlateSelector startPlateSelector(potar);

void display_score_callback(const std_msgs::Int16& msg) {
    screen.draw(msg.data);
}

void startSequence(){
  count = startPlateSelector.getPlateNumber();
  if(prevCount != count){
      drawn = false;
      prevCount = count;
  }

  if(!drawn){
    screen.draw(count);
  }
      delay(10);
}

void setup(void) {
    callbacks.on_set_display_score = display_score_callback;
    rosApi = new RosApi(&callbacks);
    rosApi->begin();
    screen.begin();
    startPlateSelector.begin();
    // Serial.begin(115200);
    pinMode(startCord, INPUT);
    rosApi->run();
    rosApi->pub_distance_reached();
    while(!digitalRead(startCord)){
      startSequence();
    }
     
    rosApi->pub_set_start_plate(count);
}

void loop(void) {
  rosApi->run();
  delay(25);
}
