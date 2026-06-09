// Esse código é para caso seja preferivel a utilização de um pistão no lugar do servo.
#pragma once
#include <Arduino.h>

class Piston {
private:
  uint32_t _timeToActivate = 0;
  uint32_t _delayToStart = 5000000UL; // 5s
  uint32_t _startTime = 0;            // Referência fixa

  uint32_t _click1Start;
  uint32_t _click1End;
  uint32_t _click2Start;
  uint32_t _click2End;

  // For Debug
  long long _timeStart;
  long long _timeEnd;

public:
  enum Estado { IDLE, AGUARDANDO_CLIQUE1, CLIQUE1_ATIVO, AGUARDANDO_CLIQUE2, CLIQUE2_ATIVO };
  Estado _estado = IDLE;
  int pino = 27;
  int pino2 = 26;
  void begin(long timeToHit, long timeScored) {

    pinMode(pino, OUTPUT);
    pinMode(pino2, OUTPUT);

    _startTime = micros();
    if (timeScored != 0) {
      _timeToActivate = (uint32_t)(timeToHit + (timeToHit - timeScored)) * 1000UL;
    } else {
      _timeToActivate = (uint32_t)timeToHit * 1000UL;
    }
    _estado = AGUARDANDO_CLIQUE1;

    _click1Start = _startTime + _delayToStart;
    _click1End = _startTime + _delayToStart + 2000000UL;
    _click2Start = _startTime + _delayToStart + _timeToActivate;
    _click2End = _startTime + _delayToStart + _timeToActivate + 2000000UL;
  }

  void handle() {
    if (_estado == IDLE) { return; }

    switch (_estado) {

    case AGUARDANDO_CLIQUE1:
      if (micros() >= _click1Start) {
        digitalWrite(pino, HIGH);
        digitalWrite(pino2, HIGH);
        _estado = CLIQUE1_ATIVO;

        _timeStart = micros();
      }
      break;
    case CLIQUE1_ATIVO:
      if (micros() >= _click1End) {
        digitalWrite(pino, LOW);
        digitalWrite(pino2, LOW);
        _estado = AGUARDANDO_CLIQUE2;
      }
      break;
    case AGUARDANDO_CLIQUE2:
      if (micros() >= _click2Start) {
        digitalWrite(pino, HIGH);
        digitalWrite(pino2, HIGH);
        _estado = CLIQUE2_ATIVO;

        _timeEnd = micros();
      }
      break;
    case CLIQUE2_ATIVO:
      if (micros() >= _click2End) {
        digitalWrite(pino, LOW);
        digitalWrite(pino2, LOW);

        Serial.println("======== LOG DATA =======");
        Serial.println("Time Start: ");
        Serial.println(micros());
        Serial.print("Time End: ");
        Serial.println(micros());
        Serial.println("Delay do primeiro clique ao segundo clique: ");
        Serial.print(_timeEnd - _timeStart);
        Serial.println(" us");
        Serial.print((_timeEnd - _timeStart) / 100);
        Serial.println(" ms");
        Serial.println("========= END LOG DATA ======");
        _estado = IDLE;
      }
      break;

    default:
      break;
    }
  }
};