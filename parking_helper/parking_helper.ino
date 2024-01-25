#include <Scheduler.h>

// HC-SR04 distance sensor controller class
// TODO: Move this into its own file!
class DistanceSensor {
  public:
  	DistanceSensor(int trigPin, int echoPin) {
      _TrigPin = trigPin;
      pinMode(_TrigPin, OUTPUT);
      
      _EchoPin = echoPin;
      pinMode(_EchoPin, INPUT);
  	};
  
  	// Returns distance in CM
    float GetDistance() {
      float duration;

      // Send TRIG for 10 microseconds;
      // "sends out an 8 cycle sonic burst from the transmitter";
      digitalWrite(_TrigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(_TrigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(_TrigPin, LOW);

      // Get output;
      // This waits for PROXIMITY_TRIG to be HIGH,
      // records how long until it gets set back to LOW (microseconds),
      // then resumes execution;
      duration = pulseIn(_EchoPin, HIGH);

      // speed = distance / time;
      // distance = time * speed;
      // the speed is the speed of sound, 0.0343;
      // we need to divide by 2 because the sound wave goes
      // to the object and back;
      return (duration * 0.0343) / 2;
    }
  private:
  	int _TrigPin;
  	int _EchoPin;
};

// Piezo controller class
// TODO: Also move this into its own file lol
class Piezo {
  public:
  	Piezo(int outputPin) {
      _OutputPin = outputPin;
      pinMode(_OutputPin, OUTPUT);
  	}
  
  	void Tone(unsigned int frequency) {
      tone(_OutputPin, frequency);
    }
  	void Tone(unsigned int frequency, unsigned long duration) {
      tone(_OutputPin, frequency, duration);
  	}
  
  	void NoTone() {
      noTone(_OutputPin);
  	}
  private:
  	int _OutputPin;
};

enum ParkingLedStripState { off = 0, green = 1, yellow = 2, red = 3 };

class ParkingLedStrip {
  public:
  	ParkingLedStrip(int greenLed, int yellowLedLeft, int yellowLedRight, int redLedLeft, int redLedRight) {
      _GreenLed = greenLed;
      pinMode(_GreenLed, OUTPUT);
      
      _YellowLedLeft = yellowLedLeft;
      pinMode(_YellowLedLeft, OUTPUT);
      
      _YellowLedRight = yellowLedRight;
      pinMode(_YellowLedRight, OUTPUT);
      
      _RedLedLeft = redLedLeft;
      pinMode(_RedLedLeft, OUTPUT);
      
      _RedLedRight = redLedRight;
      pinMode(_RedLedRight, OUTPUT);
    }
  	void SetState(ParkingLedStripState newState) {
      switch(newState) {
        case off:
          digitalWrite(_GreenLed, LOW);
          digitalWrite(_YellowLedLeft, LOW);
          digitalWrite(_YellowLedRight, LOW);
          digitalWrite(_RedLedLeft, LOW);
       	  digitalWrite(_RedLedRight, LOW);
          break;
       	case green:
          digitalWrite(_GreenLed, HIGH);
          digitalWrite(_YellowLedLeft, LOW);
          digitalWrite(_YellowLedRight, LOW);
          digitalWrite(_RedLedLeft, LOW);
       	  digitalWrite(_RedLedRight, LOW);
          break;
        case yellow:
          digitalWrite(_GreenLed, HIGH);
          digitalWrite(_YellowLedLeft, HIGH);
          digitalWrite(_YellowLedRight, HIGH);
          digitalWrite(_RedLedLeft, LOW);
       	  digitalWrite(_RedLedRight, LOW);
          break;
        case red:
          digitalWrite(_GreenLed, HIGH);
          digitalWrite(_YellowLedLeft, HIGH);
          digitalWrite(_YellowLedRight, HIGH);
          digitalWrite(_RedLedLeft, HIGH);
       	  digitalWrite(_RedLedRight, HIGH);
          break;
      }
  	}
  private:
  	int _GreenLed;
    int _YellowLedLeft;
  	int _YellowLedRight;
  	int _RedLedLeft;
  	int _RedLedRight;
};

// Initialize stuff
Scheduler scheduler;
DistanceSensor distanceSensor(0, 1);
Piezo speaker(2);
ParkingLedStrip strip(5, 6, 4, 7, 3);

// Main logic
void parkingLoop() {
  float distance = distanceSensor.GetDistance();
  if (distance < 30) {
    // You're about to call your insurance lmao
    strip.SetState(red);
    speaker.Tone(900, 30);
    scheduler.Add(Task(70, parkingLoop));
  } else if (distance < 60) {
    // Bit close there
    strip.SetState(yellow);
    speaker.Tone(700, 60);
    scheduler.Add(Task(200, parkingLoop));
  } else if (distance < 100) {
    // Oh hey, there's somerthing!
    strip.SetState(green);
    speaker.Tone(500, 80);
    scheduler.Add(Task(400, parkingLoop));
  /*} else if (distance < 200) {
    // Hmm...?
    strip.SetState(green);
    speaker.Tone(300, 100);
    scheduler.Add(Task(600, parkingLoop));*/
  } else {
    strip.SetState(off);
    scheduler.Add(Task(600, parkingLoop));
  }
}

void setup()
{
  parkingLoop();
}

void loop()
{
  scheduler.Tick();
}