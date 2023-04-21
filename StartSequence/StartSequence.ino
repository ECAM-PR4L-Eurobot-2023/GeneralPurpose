

#include <Arduino.h>

#include <ESP32Servo.h>
#include "src/StartPlateSelector.h"

#include "src/data/displacement.h"
#include "src/ros_api/ros_api.h"
#include "src/ros_api/topics.h"

#include "src/screen.h"
#include "src/disguise.h"

#define potar 14
#define startCord 27
#define SERVO_RIGHT_PIN 25 //  fermer = 90 ouvert = 0
#define SERVO_LEFT_PIN 33 // fermer = 0 ouvert = 90
#define SERVO_DOOR 32 // placeholder
#define FAN_PIN 23 
#define sendButtonPin 26
#define DISGUISE_PIN_1 16
#define DISGUISE_PIN_2 17

Servo servoLeft;
Servo servoRight;
Servo servoDoor;

Screen screen;

RosApiCallbacks callbacks{};
RosApi *rosApi;

int count = 0;
int prevCount = 0;
boolean drawn = false;
bool first = true;
bool stop = false;


StartPlateSelector startPlateSelector(potar);
Disguise disguise(DISGUISE_PIN_1, DISGUISE_PIN_2);

void disguiseCallBack(const std_msgs::Empty &msg)
{
  disguise.disguise();
}

void disguiseRelease(const std_msgs::Empty &msg)
{
  disguise.release();
}

void closeDoor(const std_msgs::Empty &msg)
{
  servoDoor.write(90);
}

void openDoor(const std_msgs::Empty &msg)
{
  servoDoor.write(0);
}

void turn_on_fan_callback(const std_msgs::Int16 &msg)
{
  if (msg.data == 1)
  {
    servoLeft.write(0);
    // servoRight.write(180);
    servoRight.write(0);
    
  }
  else if (msg.data == 2)
  {
    servoLeft.write(90);
    servoRight.write(90);
  }
  else{
    servoLeft.write(0);
    servoRight.write(90);
  }
  digitalWrite(FAN_PIN, HIGH);
}

void turn_off_fan_callback(const std_msgs::Empty &msg)
{
  digitalWrite(FAN_PIN, LOW);
  servoLeft.write(0);
  servoRight.write(90);
}

void display_score_callback(const std_msgs::Int16 &msg)
{
  screen.draw(msg.data);
}

void kill(const std_msgs::Empty &kill){
  stop = true;
}

void startSequence()
{
  count = startPlateSelector.getPlateNumber();
  if (prevCount != count)
  {
    drawn = false;
    prevCount = count;
  }
// screen.draw(count);
  if (!drawn)
  {
    screen.draw(count);
  }
  delay(10);
}


void setup(void)
{
  callbacks.on_set_display_score = display_score_callback;
  callbacks.on_start_fan = turn_on_fan_callback;
  callbacks.on_stop_fan = turn_off_fan_callback;
  callbacks.on_open_door = openDoor;
  callbacks.on_close_door = closeDoor;
  callbacks.on_disguise = disguiseCallBack;
  callbacks.on_disguise_release = disguiseRelease;
  rosApi = new RosApi(&callbacks);
  rosApi->begin();
  screen.begin();
  disguise.begin();
  startPlateSelector.begin();
  pinMode(startCord, INPUT_PULLUP);

  pinMode(sendButtonPin, INPUT);
  pinMode(FAN_PIN, OUTPUT);


  servoLeft.attach(SERVO_LEFT_PIN);
  servoRight.attach(SERVO_RIGHT_PIN);
  servoDoor.attach(SERVO_DOOR);

  servoLeft.write(0);
  servoRight.write(90);
  servoDoor.write(90);

  rosApi->run();

  // Serial.println("Waitinggggggggg");
  // Serial.println(digitalRead(startCord));
  while (!digitalRead(startCord))
  {
    rosApi->run();
    startSequence();

    if (digitalRead(sendButtonPin))
    {
      while (digitalRead(sendButtonPin))
      {
        // Serial.println("inloop");
        delay(10);
      }
      rosApi->pub_set_start_plate(count);
    }
  }
  // Serial.println("feur");
  rosApi->pub_send_start();

}

void loop(void)
{
  while(stop){
    killAll();
  }
  rosApi->run();
  disguise.run();
  delay(10);
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
}


void killAll(){
  digitalWrite(FAN_PIN, LOW);
  servoLeft.detach();
  servoRight.detach();
  servoDoor.detach();
  digitalWrite(DISGUISE_PIN_1, LOW);
  digitalWrite(DISGUISE_PIN_2, LOW);
}