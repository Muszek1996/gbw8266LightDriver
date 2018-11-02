//
// Created by jakub on 01.11.18.
//

#ifndef GBW8266LIGHTDRIVER_HTTP_H
#define GBW8266LIGHTDRIVER_HTTP_H

#include <FS.h>
#include <ESP8266WebServer.h>


namespace http{

    static ESP8266WebServer *webServer;

    static void handle() {
        webServer->handleClient();
    }

    static String getContentType(String filename) {// convert the file extension to the MIME type
        if(filename.endsWith(".htm")) return "text/html";
        else if(filename.endsWith(".html")) return "text/html";
        else if(filename.endsWith(".css")) return "text/css";
        else if(filename.endsWith(".js")) return "application/javascript";
        else if(filename.endsWith(".png")) return "image/png";
        else if(filename.endsWith(".gif")) return "image/gif";
        else if(filename.endsWith(".jpg")) return "image/jpeg";
        else if(filename.endsWith(".ico")) return "image/x-icon";
        else if(filename.endsWith(".xml")) return "text/xml";
        else if(filename.endsWith(".pdf")) return "application/x-pdf";
        else if(filename.endsWith(".zip")) return "application/x-zip";
        else if(filename.endsWith(".gz")) return "application/x-gzip";
        return "text/plain";
    }

    static bool handleFileRead(String path) { // send the right file to the client (if it exists)

        Serial.println("handleFileRead: " + path);
        if(path.endsWith("/")) path += "index.html";           // If a folder is requested, send the index file
        String contentType = getContentType(path);             // Get the MIME type
        String pathWithGz = path + ".gz";
        if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){  // If the file exists, either as a compressed archive, or normal
            if(SPIFFS.exists(pathWithGz))                          // If there's a compressed version available
                path += ".gz";                                         // Use the compressed version
            File file = SPIFFS.open(path, "r");                    // Open the file
            webServer->streamFile(file, contentType);    // Send it to the client
            file.close();                                          // Close the file again
            Serial.println(String("\tSent file: ") + path);
            return true;
        }
        Serial.println(String("\tFile Not Found: ") + path);
        return false;                                          // If the file doesn't exist, return false
    }

    static void begin(ESP8266WebServer *srv) {
        webServer = srv;

        SPIFFS.begin();                           // Start the SPI Flash Files System

        webServer->onNotFound([]() {                              // If the client requests any URI
            if (!handleFileRead(webServer->uri()))                  // send it if it exists
                webServer->send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
                Serial.println("client handle");
        });

        webServer->begin();
        Serial.println("HTTP server started");
    }

}




#endif //GBW8266LIGHTDRIVER_HTTP_H
