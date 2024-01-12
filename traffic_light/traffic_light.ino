#include <Scheduler.h>
#define SCHEDULER_MAX_TASKS 3

/* == setup == */
Scheduler scheduler;

// config
const int LED_CARS_GREEN  = 13;
const int LED_CARS_YELLOW = 12;
const int LED_CARS_RED    = 11;

const int LED_PED_GREEN   = 9;
const int LED_PED_RED     = 8;

const int CARS_GO_LENGTH     = 3000; 
const int CARS_YELLOW_LENGTH = 1000;
const int PEDS_GO_LENGTH     = 2000;

// definitions
enum TrafficState { cars_go = 0, cars_yellow = 1, peds_go = 2};
TrafficState currentState = cars_go;

/* == logic == */
void setup() {
  pinMode(LED_CARS_GREEN,  OUTPUT);
  pinMode(LED_CARS_YELLOW, OUTPUT);
  pinMode(LED_CARS_RED,    OUTPUT);

  pinMode(LED_PED_GREEN,  OUTPUT);
  pinMode(LED_PED_RED,    OUTPUT);
}

void loop() {
  scheduler.Tick();
  switch(currentState) {
    case cars_yellow:
      // Cars
      digitalWrite(LED_CARS_GREEN,  LOW);
      digitalWrite(LED_CARS_YELLOW, HIGH);
      digitalWrite(LED_CARS_RED,    LOW);

      // Peds
      digitalWrite(LED_PED_GREEN, LOW);
      digitalWrite(LED_PED_RED,   HIGH);

      delay(CARS_YELLOW_LENGTH);
      scheduler.Add(Task(CARS_YELLOW_LENGTH, [](){
        currentState = peds_go;
      }));
      break;
    case peds_go:
      // Cars
      digitalWrite(LED_CARS_GREEN,  LOW);
      digitalWrite(LED_CARS_YELLOW, LOW);
      digitalWrite(LED_CARS_RED,    HIGH);

      // Peds
      digitalWrite(LED_PED_GREEN, HIGH);
      digitalWrite(LED_PED_RED,   LOW);

      delay(PEDS_GO_LENGTH);
      scheduler.Add(Task(PEDS_GO_LENGTH, [](){
        currentState = cars_go;
      }));
      break;
    case cars_go:
      // Cars
      digitalWrite(LED_CARS_GREEN,  HIGH);
      digitalWrite(LED_CARS_YELLOW, LOW);
      digitalWrite(LED_CARS_RED,    LOW);

      // Peds
      digitalWrite(LED_PED_GREEN, LOW);
      digitalWrite(LED_PED_RED,   HIGH);

      delay(CARS_GO_LENGTH);
      scheduler.Add(Task(CARS_GO_LENGTH, [](){
        currentState = cars_yellow;
      }));
      break;
  }
}