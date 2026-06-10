#pragma once

#include <Arduino.h>
#include <JsonHandler.h>
#include <MorseLed.h>
#include <WebServer.h>
#include <WiFi.h>

#include <ApiController.h>

class WifiConnection {
private:
  String getArgs() // Recebe Post
  {
    server.sendHeader("Access-Control-Allow-Origin", "*");

    // Wip
    Serial.println("--- /control POST recebido ---");
    Serial.print("Num args: ");
    Serial.println(server.args());

    if (!server.hasArg("plain")) {
      Serial.println("Erro: sem body");
      server.send(400, "application/json", "{\"error\":\"no body\"}");
      return "{}";
    }
    String json = server.arg("plain");
    Serial.print("Body: ");
    Serial.println(json);
    return json;
  }
  void loadDefaultHttpOptionsConfig() // Define configurações de rota
  {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  }
  enum Mode { AP_MODE, STA_MODE }; // AP Roteia, STA se conecta
  Mode _estado;

  const char *_ssid;
  const char *_password;

public:
  WebServer server;
  JsonHandler json;
  ApiController api;
  MorseLed led;

  WifiConnection() : server(80) {}

  void startWifi(const char *ssid, const char *password) {
    _ssid = ssid;
    _password = password;
    _estado = AP_MODE;

    IPAddress ip(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(ssid, password);

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
  }

  void connectToWifi(const char *ssid, const char *password) {
    _ssid = ssid;
    _password = password;
    _estado = STA_MODE;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.println("Conectando");
    while (WiFi.status() != WL_CONNECTED) {
      delay(50);
      Serial.print(".");
    }
    Serial.println("WiFi conectado!");
    Serial.println("");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }

  void turnOff() { WiFi.mode(WIFI_OFF); }

  void turnOn() {
    if (WiFi.getMode() != WIFI_OFF) return;
    if (_estado == AP_MODE) {
      startWifi(_ssid, _password);
    } else {
      connectToWifi(_ssid, _password);
    }
  }

  void handle() {
    api.handle();
    // led.handle(); // Conexão com WIFI trava a aplicação então o led não executa até a finalização do mesmo podendo
    // esse ser instanciado em qualquer lugar do main
  }
};