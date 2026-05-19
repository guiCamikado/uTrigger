#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "html.h"

class WifiConnection
{
public:
    WebServer server;
    int lastValue = -1;

    WifiConnection() : server(80) {}

    void startWifi(const char *ssid, const char *password)
    {
        // Define IP, Gateway e Submascara
        IPAddress ip(192, 168, 1, 1);
        IPAddress gateway(192, 168, 0, 1);
        IPAddress subnet(255, 255, 255, 0);

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(ip, gateway, subnet);
        WiFi.softAP(ssid, password);

        Serial.print("AP IP: ");
        Serial.println(WiFi.softAPIP());

        server.on("/", HTTP_GET, [this]()
                  { server.send(200, "text/html", INDEX_HTML); });

        server.on("/control", HTTP_POST, [this]()
                  {
            if (!server.hasArg("plain")) {
                server.send(400, "application/json", "{\"error\":\"no body\"}");
                return;
            }

            String body = server.arg("plain");
            Serial.print("Recebido: ");
            Serial.println(body);

            int val = -1;
            int idx = body.indexOf("\"value\"");
            if (idx != -1) {
                int colon = body.indexOf(':', idx);
                if (colon != -1) {
                    val = body.substring(colon + 1).toInt();
                }
            }

            if (val == 0 || val == 1) {
                lastValue = val;
                server.send(200, "application/json", "{\"status\":\"ok\",\"value\":" + String(val) + "}");
            } else {
                server.send(400, "application/json", "{\"error\":\"value must be 0 or 1\"}");
            } });

        server.begin();
        Serial.println("Servidor HTTP iniciado");
    }

    // Precisa ser utilizado no loop
    void handle()
    {
        server.handleClient();
    }
};