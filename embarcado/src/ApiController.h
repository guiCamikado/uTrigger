#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>

#include "keys/cert.h"
#include "keys/private_key.h"
#include "ServoMotor.h"

using namespace httpsserver;

class ApiController {
private:
  WebServer _httpServer;       // porta 80 — só /control
  SSLCert *_cert;
  HTTPSServer *_httpsServer;   // porta 443 — só frontend

  static ApiController *_instance;

  // ── HTTPS: frontend ──────────────────────────────────────────
  static void handleRootStatic(HTTPRequest *req, HTTPResponse *res)   { _instance->handleRoot(req, res); }
  static void handleAssetsStatic(HTTPRequest *req, HTTPResponse *res) { _instance->handleAssets(req, res); }
  static void handle404Static(HTTPRequest *req, HTTPResponse *res)    { _instance->handle404(req, res); }

  void handleRoot(HTTPRequest *req, HTTPResponse *res) {
    File file = LittleFS.open("/index.html", "r");
    if (!file) { res->setStatusCode(404); res->println("index.html nao encontrado"); return; }
    res->setHeader("Content-Type", "text/html");
    uint8_t buf[512];
    while (file.available()) { size_t n = file.read(buf, sizeof(buf)); res->write(buf, n); }
    file.close();
  }

  void handleAssets(HTTPRequest *req, HTTPResponse *res) {
    String path = String(req->getRequestString().c_str());
    File file = LittleFS.open(path, "r");
    if (!file) { res->setStatusCode(404); res->println("Arquivo nao encontrado"); return; }

    if      (path.endsWith(".js"))   res->setHeader("Content-Type", "application/javascript");
    else if (path.endsWith(".css"))  res->setHeader("Content-Type", "text/css");
    else if (path.endsWith(".png"))  res->setHeader("Content-Type", "image/png");
    else if (path.endsWith(".html")) res->setHeader("Content-Type", "text/html");
    else                             res->setHeader("Content-Type", "application/octet-stream");

    uint8_t buf[512];
    while (file.available()) { size_t n = file.read(buf, sizeof(buf)); res->write(buf, n); }
    file.close();
  }

  void handle404(HTTPRequest *req, HTTPResponse *res) {
    String path = String(req->getRequestString().c_str());
    File file = LittleFS.open(path, "r");
    if (!file) { res->setStatusCode(404); res->println("404 Not Found"); return; }

    if      (path.endsWith(".html")) res->setHeader("Content-Type", "text/html");
    else if (path.endsWith(".js"))   res->setHeader("Content-Type", "application/javascript");
    else if (path.endsWith(".css"))  res->setHeader("Content-Type", "text/css");
    else if (path.endsWith(".png"))  res->setHeader("Content-Type", "image/png");
    else                             res->setHeader("Content-Type", "application/octet-stream");

    uint8_t buf[512];
    while (file.available()) { size_t n = file.read(buf, sizeof(buf)); res->write(buf, n); }
    file.close();
  }

  // ── HTTP: /control (sem TLS) ─────────────────────────────────
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

    DynamicJsonDocument doc(512);
    if (deserializeJson(doc, body)) {
      _httpServer.send(400, "application/json", "{\"success\":false,\"error\":\"JSON invalido\"}");
      return;
    }

    servo.cheatLogic(
      doc["hitscore"].as<unsigned long>(),
      doc["pictureTime"].as<unsigned long>(),
      doc["sendTime"].as<unsigned long>(),
      doc["pictureTimeFrame"].as<unsigned long>(),
      doc["timeScored"].as<unsigned long>()
    );

    _httpServer.send(200, "application/json", "{\"success\":true}");
  }

public:
  ServoMotor servo{25};

  ApiController() : _httpServer(80), _cert(nullptr), _httpsServer(nullptr) { _instance = this; }

  void begin() {
    servo.begin();

    if (!LittleFS.begin(true)) {
      Serial.println("Erro ao montar LittleFS!");
      return;
    }
    Serial.println("LittleFS OK");

    // ── HTTP na 80: só /control ───────────────────────────────
    _httpServer.on("/control", handleControlStatic);
    _httpServer.begin();
    Serial.println("HTTP Server iniciado na porta 80");

    // ── HTTPS na 443: só frontend ─────────────────────────────
    _cert = new SSLCert(
      (unsigned char *)server_cert, sizeof(server_cert),
      (unsigned char *)private_key,  sizeof(private_key)
    );
    _httpsServer = new HTTPSServer(_cert);

    ResourceNode *nodeRoot   = new ResourceNode("/",         "GET", handleRootStatic);
    ResourceNode *nodeAssets = new ResourceNode("/assets/*", "GET", handleAssetsStatic);
    ResourceNode *node404    = new ResourceNode("",          "GET", handle404Static);

    _httpsServer->registerNode(nodeRoot);
    _httpsServer->registerNode(nodeAssets);
    _httpsServer->setDefaultNode(node404);

    _httpsServer->start();
    Serial.println("HTTPS Server iniciado na porta 443");
  }

  void handle() {
    _httpServer.handleClient();
    if (_httpsServer) _httpsServer->loop();
    servo.update();
  }
};

ApiController *ApiController::_instance = nullptr;