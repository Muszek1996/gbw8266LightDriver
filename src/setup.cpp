#include "http.h"
#include "ws.h"
#include <ESP8266WiFi.h>
//
// Created by jakub on 01.11.18.
//
void WiFiBegin();

ESP8266WebServer webServer(80);

void setup(){
    Serial.begin(115200);
    leds::pinModes();
    WiFiBegin();
    http::begin(&webServer);
    ws::startWebSocket();
}


void loop(){
    http::handle();
    ws::webSocketLoop();
}

void WiFiBegin(){
    WiFi.begin("TURBO", "K0nr4f4l");             // Connect to the network
    Serial.print("Connecting to ");
    Serial.print("TURBO"); Serial.println(" ...");

    int i = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        delay(250);
        Serial.print(++i); Serial.print(' ');
    }
}
