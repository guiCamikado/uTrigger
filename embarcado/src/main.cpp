#include <Arduino.h>

#include "ApiController.h"
#include "WifiConnection.h"
#include <ESP32Servo.h>

WifiConnection wifi;
// ApiController api;
Servo servo;

void setup() {
  Serial.begin(115200);
  // wifi.startWifi("ESP32", "camikado");
  wifi.connectToWifi(***REMOVED***" ", "***REMOVED***");
  wifi.api.servo.begin();
  wifi.api.initiateRoutes();
  wifi.led.setup();
}

void loop() {
  wifi.handle();
}
