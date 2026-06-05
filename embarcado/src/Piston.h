// Esse código é para caso seja preferivel a utilização de um pistão no lugar do servo.
#pragma once
#include <Arduino.h>

class Piston {
private:
  long _timeToActivate = 0;
  long _delayToStart = 5000;
  unsigned long _timer = micros();
  bool _isWorking = false;

public:
  void begin(long timeToHit, long timeScored) {
    // Ativa e desativa pistão dando inicio ao funcionamento.
    _isWorking = true;
    if (timeScored != 0) {
      _timeToActivate = (timeToHit * 1000) - (timeScored * 1000);
      _timer = micros();
    } else {
      _timeToActivate = timeToHit * 1000;
      _timer = micros();
    }
  }

  void handle() {
    if (!_isWorking) { return; }

    if (_timer + _delayToStart <= micros()) { pinMode(23, HIGH); }
    if (_timer + _delayToStart + 2000000UL <= micros()) { pinMode(23, LOW); }

    if ((_timer + _timeToActivate) <= micros()) {
      pinMode(23, LOW);
      return;
    }
    pinMode(23, HIGH);
    _timer = micros();
    if (_timer + 2000000UL) {
      pinMode(23, LOW);
      _isWorking = false;
    }
  }
};
