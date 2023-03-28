#include <analogWrite.h>
#include <ESP32PWM.h>
#include <ESP32Servo.h>
#include <ESP32Tone.h>

#include <Arduino.h>

#include "src/StartPlateSelector.h"

#include "src/data/displacement.h"
#include "src/ros_api/msg/Displacement.h"
#include "src/ros_api/ros_api.h"
#include "src/ros_api/topics.h"

#include "src/screen.h"



#define potar 36

#define startCord 32

#define SERVO_LEFT_PIN 25  //placeholder
#define SERVO_RIGHT_PIN 33  //placeholder

#define FAN_PIN 27 //placeholder

Servo servoLeft;
Servo servoRight;

Screen screen;

RosApiCallbacks callbacks {};
RosApi *rosApi;

int count = 0;
int prevCount = 0;
boolean drawn = false;
bool first = true;

StartPlateSelector startPlateSelector(potar);

void turn_on_fan_callback(const std_msgs::Int16& msg){
  if(msg.data == 1){
    servoLeft.write(0);
    servoRight.write(180);
  }else{
    servoLeft.write(90);
    servoRight.write(0);
  }
  digitalWrite(FAN_PIN, HIGH);
}

void turn_off_fan_callback(const std_msgs::Empty& msg){
  digitalWrite(FAN_PIN, LOW);
  servoLeft.write(0);
  servoRight.write(180);
}

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
    callbacks.on_start_fan = turn_on_fan_callback;
    callbacks.on_stop_fan = turn_off_fan_callback;
    rosApi = new RosApi(&callbacks);
    rosApi->begin();
    screen.begin();
    startPlateSelector.begin();
    pinMode(startCord, INPUT);

    servoLeft.attach(SERVO_LEFT_PIN);
    servoRight.attach(SERVO_RIGHT_PIN);
    servoLeft.write(180);
    servoRight.write(0);
}

void loop(void) {
  // rosApi->run();
  // if(first){
  //   while(digitalRead(startCord)){
  //     rosApi->run();
  //     startSequence();
  //     Serial.println("Waiting");
  //   }
  //   Serial.println("Received");
  //   first = false;
  //   rosApi->pub_distance_reached();
  //   rosApi->pub_set_start_plate(count);
  // }
  // std_msgs::Int16 mess = std_msgs::Int16();
  // mess.data = 1;
  // delay(2000);
  // turn_on_fan_callback(mess);
  //   delay(2000);
  // turn_off_fan_callback(std_msgs::Empty());
  //   delay(2000);
  //   mess.data = 0;
  // turn_on_fan_callback(mess);
  



// servoLeft.write(90);
//     servoRight.write(0);
//     delay(2000);
//         servoLeft.write(0);
//     servoRight.write(180);
servoLeft.write(0);
servoRight.write(0);
// delay(2000);
// servoLeft.write(10);
// servoRight.write(10);
// delay(2000);

}
