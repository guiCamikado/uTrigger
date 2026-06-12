#pragma once
#include <ESP32Servo.h>

// This class is for the use of a servoMotor (In case of the piston not being userfull)
class ServoMotor {
private:
  Servo servo;
  unsigned long _timeToTrigger = 0;
  int _defaultSpin = 180;
  int _signalPin = 25;
  bool _waitingReturn = false;
  bool _waitingTrigger = false;

public:
  // Bob the builder
  ServoMotor(int signalPin) : _signalPin(signalPin) {}

  void begin() {
    servo.setPeriodHertz(50);
    servo.attach(_signalPin, 500, 2400);
  }

  void cheatLogic(long hitscore, long pictureTime, long sentTime, long pictureTimeFrame, long timeScored) {
    long timePassed;

    if (timeScored && timeScored != 0) {
      timePassed = (((sentTime - pictureTime) + (pictureTimeFrame * 1000))) + (timeScored * 1000 - hitscore * 1000);
    } else {
      timePassed = ((sentTime - pictureTime) + (pictureTimeFrame * 1000));
    }

    long timeRemaining = (hitscore * 1000) - timePassed;

    Serial.print("[servo] timePassed: ");
    Serial.println(timePassed);
    Serial.print("[servo] timeRemaining: ");
    Serial.println(timeRemaining);

    if (timeRemaining <= 0) {
      // If time already passed - discharge imediattly
      Serial.println("[servo] disparo imediato!");
      servo.write(_defaultSpin);
      _timeToTrigger = micros() + 5000000UL;
      _waitingReturn = true;
      _waitingTrigger = false;
    } else {
      // schedule a discharge
      _timeToTrigger = micros() + (unsigned long)timeRemaining;
      _waitingTrigger = true;
      Serial.print("[servo] agendado em us: ");
      Serial.println(timeRemaining);
    }
  }

  void update() {
    if (_waitingTrigger && ((long)(micros() - _timeToTrigger) >= 0)) {
      servo.write(_defaultSpin);
      _timeToTrigger = micros() + 5000000UL;
      _waitingReturn = true;
      _waitingTrigger = false;
    }

    if (_waitingReturn && (long)(micros() - _timeToTrigger) >= 0) {
      _waitingReturn = false;

      servo.write(0);
    }
  }

  void moveFoward() { servo.write(180); }
  void moveBackward() { servo.write(0); }
};
