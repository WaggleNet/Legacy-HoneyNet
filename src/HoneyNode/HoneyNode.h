#pragma once
#include <SPI.h>
#include "Arduino.h"
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <EEPROM.h>

#define AddChannel(X, Y) registerChannel(X, sizeof(Y))
#define Send(CH, CONT) publish(CH, (byte*)& CONT)
#define SendPointer(CH, CONT) publish(CH, (bytes*) CONT)
#define Print(CH, CONT) publish(CH, CONT)

class HoneyNode {
    // The Infra
    RF24 radio;
    RF24Network network;
    RF24Mesh mesh;
    uint8_t nodeID;
    // The Middleware
    uint8_t registry[32];
    public:
        HoneyNode(uint8_t CE_pin, uint8_t CS_pin);
        void begin();
        void update();
        void registerChannel(uint8_t channel, uint8_t size);
        uint8_t publish(uint8_t channel, byte *payload);
        uint8_t publish(uint8_t channel, String payload);
    private:
        void serialSetID();
        uint8_t write(uint8_t ch, byte *payload, uint8_t len);
};
