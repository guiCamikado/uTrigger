#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPSServer.hpp>
#include <LittleFS.h>
#include <Piston.h>
#include <SSLCert.hpp>
#include <WebServer.h>
#include <WiFi.h>
#include <MorseLed.h>

#include "keys/cert.h"
#include "keys/private_key.h"

using namespace httpsserver;

class ApiController {
private:
  WebServer _httpServer; // gate 80
  SSLCert *_cert;
  HTTPSServer *_httpsServer; // gate 443

  static ApiController *_instance;

  // ── HTTPS: frontend ──────────────────────────────────────────
  static void handleRootStatic(HTTPRequest *req, HTTPResponse *res) { _instance->handleRoot(req, res); }
  static void handleAssetsStatic(HTTPRequest *req, HTTPResponse *res) { _instance->handleAssets(req, res); }
  static void handle404Static(HTTPRequest *req, HTTPResponse *res) { _instance->handle404(req, res); }

  void handleRoot(HTTPRequest *req, HTTPResponse *res) {
    File file = LittleFS.open("/index.html", "r");
    if (!file) {
      res->setStatusCode(404);
      res->println("index.html nao encontrado");
      return;
    }
    res->setHeader("Content-Type", "text/html");
    uint8_t buf[512];
    while (file.available()) {
      size_t n = file.read(buf, sizeof(buf));
      res->write(buf, n);
    }
    file.close();
  }

  void handleAssets(HTTPRequest *req, HTTPResponse *res) {
    String path = String(req->getRequestString().c_str());
    File file = LittleFS.open(path, "r");
    if (!file) {
      res->setStatusCode(404);
      res->println("Arquivo nao encontrado");
      return;
    }

    if (path.endsWith(".js"))
      res->setHeader("Content-Type", "application/javascript");
    else if (path.endsWith(".css"))
      res->setHeader("Content-Type", "text/css");
    else if (path.endsWith(".png"))
      res->setHeader("Content-Type", "image/png");
    else if (path.endsWith(".html"))
      res->setHeader("Content-Type", "text/html");
    else
      res->setHeader("Content-Type", "application/octet-stream");

    uint8_t buf[512];
    while (file.available()) {
      size_t n = file.read(buf, sizeof(buf));
      res->write(buf, n);
    }
    file.close();
  }

  void handle404(HTTPRequest *req, HTTPResponse *res) {
    String path = String(req->getRequestString().c_str());
    File file = LittleFS.open(path, "r");
    if (!file) {
      res->setStatusCode(404);
      res->println("404 Not Found");
      return;
    }

    if (path.endsWith(".html"))
      res->setHeader("Content-Type", "text/html");
    else if (path.endsWith(".js"))
      res->setHeader("Content-Type", "application/javascript");
    else if (path.endsWith(".css"))
      res->setHeader("Content-Type", "text/css");
    else if (path.endsWith(".png"))
      res->setHeader("Content-Type", "image/png");
    else
      res->setHeader("Content-Type", "application/octet-stream");

    uint8_t buf[512];
    while (file.available()) {
      size_t n = file.read(buf, sizeof(buf));
      res->write(buf, n);
    }
    file.close();
  }

  // ── HTTP: /control (without TLS) ─────────────────────────────────
  static void handleControlStatic() { _instance->handleControl(); }

  void handleControl() {
    _httpServer.sendHeader("Access-Control-Allow-Origin", "*");
    _httpServer.sendHeader("Access-Control-Allow-Headers", "*");
    _httpServer.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");

    if (_httpServer.method() == HTTP_OPTIONS) {
      _httpServer.send(204);
      return;
    }

    if (!_httpServer.hasArg("plain")) {
      _httpServer.send(400, "application/json", "{\"success\":false,\"error\":\"sem body\"}");
      return;
    }

    const String &body = _httpServer.arg("plain");
    JsonDocument doc;
    if (deserializeJson(doc, body)) {
      _httpServer.send(400, "application/json", "{\"success\":false,\"error\":\"JSON invalido\"}");
      return;
    }

    // Reset button
    if (doc["reset"].as<bool>() == true) {
      _httpServer.send(200, "application/json", "{\"success\":true,\"message\":\"restarting\"}");
      _httpServer.stop(); // Para servidor e libera memória

      delay(2000);
      ESP.restart();
      return;
    }
    
    // start button
    MorseLed::turnOff();
    piston.begin(doc["hitscore"].as<unsigned long>(), doc["timeScored"].as<unsigned long>());

    _httpServer.send(200, "application/json", "{\"success\":true}");
  }

public:
  Piston piston;

  ApiController() : _httpServer(80), _cert(nullptr), _httpsServer(nullptr) { _instance = this; }

  void begin() {
    // Deletes objects if exists avoiding memory leak
    if (_httpsServer) {
      delete _httpsServer;
      _httpsServer = nullptr;
    }
    if (_cert) {
      delete _cert;
      _cert = nullptr;
    }

    if (!LittleFS.begin(true)) {
      Serial.println("Erro ao montar LittleFS!");
      return;
    }
    Serial.println("LittleFS OK");

    // ── HTTP in 80 (only /control) ───────────────────────────────
    _httpServer.on("/control", handleControlStatic);
    _httpServer.begin();
    Serial.println("HTTP Server iniciado na porta 80");

    // ── HTTPS in 443: (only front-end) ───────────────────────────
    _cert = new SSLCert((unsigned char *)server_cert, sizeof(server_cert), (unsigned char *)private_key,
                        sizeof(private_key));
    _httpsServer = new HTTPSServer(_cert);

    ResourceNode *nodeRoot = new ResourceNode("/", "GET", handleRootStatic);
    ResourceNode *nodeAssets = new ResourceNode("/assets/*", "GET", handleAssetsStatic);
    ResourceNode *node404 = new ResourceNode("", "GET", handle404Static);

    _httpsServer->registerNode(nodeRoot);
    _httpsServer->registerNode(nodeAssets);
    _httpsServer->setDefaultNode(node404);

    _httpsServer->start();
    Serial.println("HTTPS Server iniciado na porta 443");
  }

  // Stop servers in a clean way before turn off WiFi
  void stopServers() {
    Serial.println("Parando servidores...");
    _httpServer.close(); // fecha o socket HTTP (porta 80)
    if (_httpsServer) {
      _httpsServer->stop(); // fecha conexões TLS e o socket HTTPS (porta 443)
    }
    Serial.println("Servidores parados.");
  }

  // Start servers again after WiFi reconnects
  void startServers() {
    Serial.println("Iniciando servidores...");
    _httpServer.begin(); // re-bind porta 80
    if (_httpsServer) {
      _httpsServer->start(); // re-bind porta 443
    }
    Serial.println("Servidores iniciados.");
  }

  void handle() {
    _httpServer.handleClient();
    if (_httpsServer) _httpsServer->loop();
  }
};

ApiController *ApiController::_instance = nullptr;