#define SCHEDULER_MAX_TASKS 10
#include <Scheduler.h>

#include <Servo.h>

// initialize stuff
Scheduler scheduler;
Servo servo;
int pos = 0;

void setup() {
  servo.attach(13);
  Serial.begin(9600); // Bluetooth connection
}

void loop() {
  scheduler.Tick();
  
  // Check for Bluetooth input
  // TODO: The AppInventor should block itself until it gets some sort of read receipt, since 9600baud is very slow
  if (Serial.available() > 0) {
    String received = Serial.readString();
    if (received == "servo:0") {
      servo.write(0);
    } else if (received == "servo:1") {
      servo.write(90);
    } else if (received == "servo:2") {
      servo.write(180);
    } else {
      Serial.println("alert:Unknown message - " + received);
    }
  }
}