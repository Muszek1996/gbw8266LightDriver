//
// Created by jakub on 02.11.18.
//

#ifndef GBW8266LIGHTDRIVER_WS_H
#define GBW8266LIGHTDRIVER_WS_H

#include <WebSocketsServer.h>
#include "LinkedList.h"
#include "leds.h"
#include <Arduino.h>
#include <ArduinoJson.h>

namespace ws {
    static WebSocketsServer webSocketsServer(81);
    static LinkedList<uint8_t> *clients = new LinkedList<uint8_t>();

    static void webSocketLoop() {
        webSocketsServer.loop();
    }

    static void sendAll(uint8_t *payload){
        for(int i = 0;i<clients->size();i++)
            webSocketsServer.sendTXT(clients->get(i),payload);
    }

    static void handleDataReceived(uint8_t num, uint8_t *payload, size_t lenght) {
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);
        sendAll(payload);
        if (root.containsKey("type")) {
            if (root["type"] == "analogArr") {
                JsonArray& jsonArray = root["vals"];
                for(size_t i =0;i<jsonArray.size();i++){
                    jsonArray.copyTo(leds::val);
                    leds::writeLeds();
                }
            }else if (root["type"] == "windowAction") {
                if (root["data"] == "0") {

                } else if (root["data"] == "1") {

                } else {

                }
            }

        }
    }



    void static webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload,
                               size_t lenght) { // When a WebSocket message is received
        switch (type) {
            case WStype_DISCONNECTED:             // if the websocket is disconnected
                Serial.printf("[%u] Disconnected!\n", num);
                for (int i = 0; i < clients->size(); i++) {
                    if (clients->get(i) == num) {
                        clients->remove(i);
                    }
                }
                break;
            case WStype_CONNECTED: {              // if a new websocket connection is established
                clients->add(num);
                IPAddress ip = webSocketsServer.remoteIP(num);
                char msg[50];
                sprintf(msg, "[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                StaticJsonBuffer<100> jsonBuffer;
                JsonObject &root = jsonBuffer.createObject();
                root["type"] = "Data";
                root["data"] = msg;
                String a;
                root.printTo(a);
                webSocketsServer.sendTXT(num, a);
            }
                break;
            case WStype_TEXT: {                   // if new text data is received
                handleDataReceived(num, payload, lenght);
            }
        }
    }


    static void startWebSocket() { // Start a WebSocket server
        webSocketsServer.begin();                          // start the websocket server
        webSocketsServer.onEvent(
                webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
        Serial.println("WebSocket server started.");
    }


}


#endif //GBW8266LIGHTDRIVER_WS_H
