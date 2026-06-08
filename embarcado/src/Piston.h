// Esse código é para caso seja preferivel a utilização de um pistão no lugar do servo.
#pragma once
#include <Arduino.h>

class Piston {
private:
  unsigned long _timeToActivate = 0;
  unsigned long _delayToStart = 5000000UL; // 5s
  unsigned long _startTime = 0;            // Referência fixa

  enum Estado { IDLE, AGUARDANDO_CLIQUE1, CLIQUE1_ATIVO, AGUARDANDO_CLIQUE2, CLIQUE2_ATIVO };
  Estado _estado = IDLE;

public:
  int pino = 27;
  int pino2 = 26;
  void begin(long timeToHit, long timeScored) {
    pinMode(pino, OUTPUT);
    pinMode(pino2, OUTPUT);
    _startTime = micros();

    if (timeScored != 0) {
      _timeToActivate = (unsigned long)(timeToHit + (timeToHit - timeScored)) * 1000UL;
    } else {
      _timeToActivate = (unsigned long)timeToHit * 1000UL;
    }
    _estado = AGUARDANDO_CLIQUE1;
  }

  void handle() {
    if (_estado == IDLE) { return; }

    switch (_estado) {

    case AGUARDANDO_CLIQUE1:
      if (micros() >= _startTime + _delayToStart) {
        digitalWrite(pino, HIGH);
        digitalWrite(pino2, HIGH);
        Serial.println("CliqueInicial");
        _estado = CLIQUE1_ATIVO;
      }
      break;
    case CLIQUE1_ATIVO:
      if (micros() >= _startTime + _delayToStart + 2000000UL) {
        digitalWrite(pino, LOW);
        digitalWrite(pino2, LOW);
        Serial.println("DesativaçãoDoClickInicial");
        _estado = AGUARDANDO_CLIQUE2;
      }
      break;
    case AGUARDANDO_CLIQUE2:
      if (micros() >= _startTime + _delayToStart + _timeToActivate) {
        digitalWrite(pino, HIGH);
        digitalWrite(pino2, HIGH);
        Serial.println("Segundo Clique");
        _estado = CLIQUE2_ATIVO;
      }
      break;
    case CLIQUE2_ATIVO:
      if (micros() >= _startTime + _delayToStart + _timeToActivate + 2000000UL) {
        digitalWrite(pino, LOW);
        digitalWrite(pino2, LOW);
        Serial.println("Desativação do Segundo Clique");
        _estado = IDLE;
      }
      break;

    default:
      break;
    }
  }
};