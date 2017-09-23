#pragma once
#include <SPI.h>
#include "Arduino.h"
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <EEPROM.h>

typedef void (* handlerFunc)(void* args);

#define AddChannel(X, Y) registerChannel(X, sizeof(Y))
#define Send(CH, CONT) publish(CH, &CONT)
#define SendPointer(CH, CONT) publish(CH, CONT)
#define Print(CH, CONT) publish(CH, CONT)

struct channel_t {
    // Using 16-bit channel number is to comply
    // with the alignment on ESP8266 (16-bit).
    uint16_t ch_id;
    uint16_t size;
};

class HoneyNode {
    // The Infra
    RF24 radio;
    RF24Network network;
    RF24Mesh mesh;
    uint8_t nodeID;
    // The Middleware
    uint8_t registry[32];
    handlerFunc callbacks[32];
    public:
        HoneyNode(uint8_t CE_pin, uint8_t CS_pin);
        void begin();
        void begin(uint8_t nodeID);
        void begin(uint8_t nodeID, uint8_t radio_channel);
        void update();
        void registerChannel(uint8_t channel, uint8_t size);
        void listenTo(uint8_t channel, handlerFunc callback);
        uint8_t publish(uint8_t channel, void *payload);
        uint8_t publish(uint8_t channel, String payload);
    private:
        uint8_t serialSetID();
        uint8_t write(void *payload, uint8_t ch, uint8_t len);
};
