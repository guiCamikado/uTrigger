#include <Arduino.h>

#include "WifiConnection.h"

WifiConnection wifi;

void setup() {
  Serial.begin(115200);
  // wifi.startWifi("ESP32", "12345678");
  wifi.connectWifi(***REMOVED***" ", "***REMOVED***");
}

void loop() { wifi.handle(); }
