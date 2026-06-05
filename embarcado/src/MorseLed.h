#pragma once
#include <Arduino.h>

class MorseLed {
private:
  const int pin = 2;
  const char *text = "entao voce sabe ler morse   neeeeerd";

  int charIndex = 0;
  int symbolIndex = 0;

  String morse = "";
  unsigned long lastTime = 0;
  bool ledState = false;
  const int dotTime = 60;

  String toMorse(char c) {
    switch (toupper(c)) {
    case 'A':
      return ".-";
    case 'B':
      return "-...";
    case 'C':
      return "-.-.";
    case 'D':
      return "-..";
    case 'E':
      return ".";
    case 'F':
      return "..-.";
    case 'G':
      return "--.";
    case 'H':
      return "....";
    case 'I':
      return "..";
    case 'J':
      return ".---";
    case 'K':
      return "-.-";
    case 'L':
      return ".-..";
    case 'M':
      return "--";
    case 'N':
      return "-.";
    case 'O':
      return "---";
    case 'P':
      return ".--.";
    case 'Q':
      return "--.-";
    case 'R':
      return ".-.";
    case 'S':
      return "...";
    case 'T':
      return "-";
    case 'U':
      return "..-";
    case 'V':
      return "...-";
    case 'W':
      return ".--";
    case 'X':
      return "-..-";
    case 'Y':
      return "-.--";
    case 'Z':
      return "--..";
    case ' ':
      return " ";
    default:
      return " ";
    }
    return "";
  }

public:
  void setup() {
    pinMode(pin, OUTPUT);
    morse = toMorse(text[0]);
  }

  void handle() {
    if (millis() - lastTime < dotTime) return;
    lastTime = millis();

    // espaço
    if (morse == " ") {
      digitalWrite(pin, LOW);
      delay(dotTime * 4);
      nextLetter();
      return;
    }

    ledState = !ledState;
    if (ledState) {
      digitalWrite(pin, HIGH);
      if (morse[symbolIndex] == '-') { delay(dotTime * 2); }
    } else {
      digitalWrite(pin, LOW);
      symbolIndex++;
      // próxima letra
      if (symbolIndex >= morse.length()) {
        delay(dotTime * 2);
        nextLetter();
      }
    }
  }

  void nextLetter() {
    symbolIndex = 0;
    charIndex++;
    // loop infinito
    if (text[charIndex] == '\0') {
      charIndex = 0;
      delay(dotTime * 6);
    }
    morse = toMorse(text[charIndex]);
  }
};