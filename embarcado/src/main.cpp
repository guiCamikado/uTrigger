#include <Arduino.h>

#include "ApiController.h"
#include "Piston.h"
#include "WifiConnection.h"
#include "keys/secretKeys.h"

#include <MorseLed.h>

WifiConnection wifi;
MorseLed morse;
TaskHandle_t pistonTaskHandle;

void setup() {
  Serial.begin(115200);

  // wifi.startWifi(AP_SSID, AP_PASSWORD);
  wifi.connectToWifi(STA_SSID, STA_PASSWORD);
  wifi.api.begin();
  wifi.led.setup();
  morse.setup();
}

void loop() {
  if (wifi.api.piston._estado == Piston::IDLE) {
    wifi.turnOn();
    wifi.handle();
    morse.turnOn();
  } else {
    wifi.turnOff();
  }

  wifi.api.piston.handle();
}