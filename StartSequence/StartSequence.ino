#include <analogWrite.h>
#include <ESP32PWM.h>
#include <ESP32Servo.h>
#include <ESP32Tone.h>

#include <Arduino.h>

#include "src/StartPlateSelector.h"

#include "src/data/displacement.h"
#include "src/ros_api/ros_api.h"
#include "src/ros_api/topics.h"

#include "src/screen.h"

#define DISGUISE_TIME 1500 //placeholder

#define potar 14
#define startCord 27
#define SERVO_RIGHT_PIN 25 //  fermer = 90 ouvert = 0
#define SERVO_LEFT_PIN 33 // fermer = 0 ouvert = 90
#define SERVO_DOOR 32 // placeholder
#define FAN_PIN 23 
#define sendButtonPin 26
#define DISGUISE_PIN 16

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
bool disguising = false;
unsigned long disguiseStart = 0;

StartPlateSelector startPlateSelector(potar);

void disguise(const std_msgs::Empty &msg)
{
  digitalWrite(DISGUISE_PIN, HIGH);
  disguising = true;
  disguiseStart = millis();
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
  else
  {
    servoLeft.write(90);
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
  rosApi = new RosApi(&callbacks);
  rosApi->begin();
  screen.begin();
  startPlateSelector.begin();
  pinMode(startCord, INPUT_PULLUP);

  pinMode(sendButtonPin, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(DISGUISE_PIN, OUTPUT);

  servoLeft.attach(SERVO_LEFT_PIN);
  servoRight.attach(SERVO_RIGHT_PIN);
  servoDoor.attach(SERVO_DOOR);

  servoLeft.write(0);
  servoRight.write(90);
  servoDoor.write(90);

  rosApi->run();
  Serial.println("Waitinggggggggg");
  while (digitalRead(startCord))
  {
    rosApi->run();
    startSequence();

    if (digitalRead(sendButtonPin))
    {
      while (digitalRead(sendButtonPin))
      {
        Serial.println("inloop");
        delay(10);
      }
      rosApi->pub_set_start_plate(count);
    }
  }
  Serial.println("feur");
  rosApi->pub_send_start();

}

void loop(void)
{
  rosApi->run();
  if (disguising && millis()-disguiseStart > DISGUISE_TIME)
  {
    digitalWrite(DISGUISE_PIN, LOW);
    disguising = false;
  }
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
