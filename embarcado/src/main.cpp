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
TaskHandle_t pistonTaskHandle;

void setup() {
  Serial.begin(115200);
  // wifi.startWifi("ESP32", "camikado");
  wifi.connectToWifi(***REMOVED***" ", "***REMOVED***");
  wifi.api.begin();
  wifi.led.setup();
  morse.setup();
}

void loop() {
  if (wifi.api.piston._estado == Piston::IDLE)
  {
    wifi.handle();
  }
  
  wifi.api.piston.handle();
  // morse.handle();
}
