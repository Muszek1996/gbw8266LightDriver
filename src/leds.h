//
// Created by jakub on 02.11.18.
//

#ifndef GBW8266LIGHTDRIVER_LED_H
#define GBW8266LIGHTDRIVER_LED_H
#include <Arduino.h>
#define ledsCount 7

namespace leds{
    static uint8_t pins[ledsCount] = {D1,D2,D3,D4,D5,D6,D7};
    static uint16_t val[ledsCount];


    static void writeLeds(){
        for(int i = 0;i<ledsCount;i++){
                analogWrite(pins[i],val[i]);
    }
    }

    static void init(){
        for(int i = 0;i<ledsCount;i++){
            val[i]=0;
        }
        writeLeds();
    }

    static void pinModes(){
        for(int i = 0;i<ledsCount;i++){
            pinMode(pins[i],OUTPUT);
        }
        init();
    }

    static void setVals(uint8_t * arr){
        for(int i = 0;i<ledsCount;i++){
            val[i]=arr[i];
            Serial.print("Setting led [");Serial.print(i);Serial.print("] with val: ");Serial.println(arr[i]);
        }
    }





}


#endif //GBW8266LIGHTDRIVER_LED_H
