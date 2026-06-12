#pragma once
#include <Arduino.h>

class MorseLed {
public:
  void setup() {
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    // morse = toMorse(text[0]);
  }

  static void turnOff() { digitalWrite(2, LOW); }
  static void turnOn() { digitalWrite(2, HIGH); }
};