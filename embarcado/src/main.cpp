#include <Arduino.h>

#include "ApiController.h"
#include "WifiConnection.h"
// #include <ESP32Servo.h>
#include "Piston.h"

#include <MorseLed.h>

WifiConnection wifi;
// ApiController api;
// Servo servo;
MorseLed morse;

void setup() {
  Serial.begin(115200);
  wifi.startWifi("ESP32", "camikado");
  // wifi.connectToWifi(***REMOVED***" ", "***REMOVED***");
  wifi.api.begin();
  wifi.led.setup();
  morse.setup();
}

void loop() {
  wifi.handle();
  // morse.handle();
}
