#include "HoneyNode.h"

HoneyNode::HoneyNode(uint8_t CE_pin, uint8_t CS_pin)
    : radio(RF24(CE_pin, CS_pin)), network(RF24Network(radio)), mesh(RF24Mesh(radio, network)) {
}

void HoneyNode::begin() {
    serialSetID();
    Serial.println(F("Connecting..."));
    mesh.begin(123, RF24_2MBPS);
}

void HoneyNode::serialSetID() {
    nodeID = EEPROM.read(0);
    if (nodeID > 0 && nodeID < 254) {
        mesh.setNodeID(nodeID);
    } else {
        Serial.println(F("Address not set. Please set address"));
        while (!mesh.getNodeID()) {
            nodeID = Serial.parseInt();
            mesh.setNodeID(nodeID);
            EEPROM.write(0, nodeID);
        }
    }
    Serial.print("Node ID set to ");
    Serial.println(nodeID);
}

void HoneyNode::update() {
    mesh.update();
    while (network.available()) {
        RF24NetworkHeader header;
        size_t data_size = network.peek(header);
        size_t channel_size = registry[header.type - 64];
        if (channel_size != data_size)
            Serial.println(F("Warning: Data size mismatch."));
        byte *payload;
        payload = new byte[channel_size];
        network.read(header, payload, channel_size);
        callbacks[header.type - 64](payload);
        delete[] payload;
    }
}

void HoneyNode::registerChannel(uint8_t channel, uint8_t size) {
    if (channel >=64) channel -= 64;
    registry[channel] = size;
    // Send the channel information to router
    channel_t ch;
    ch.ch_id = channel;
    ch.size = size;
    Serial.print("Registering ch#");
    Serial.print(channel);
    Serial.print(" with size ");
    Serial.print(size);
    Serial.print("...");
    uint8_t flag = 1;
    while (flag > 0)
        flag = write(&ch, 64, sizeof(ch));
    Serial.println("Done!");
}

uint8_t HoneyNode::write(void *payload, uint8_t ch, uint8_t len) {
    /* Returns:
    - 0: Normal
    - 1: Send failed, test OK
    - 2: Send failed, address is lost (will block)
    - 3: Invalid send call
    */
    auto my_id = mesh.getNodeID(mesh.mesh_address);
    if (my_id < 0 || my_id != nodeID) mesh.renewAddress();
    if (!mesh.write(payload, ch, len)) {
      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
        return 2;
      } else return 1;
    } else return 0;
}

uint8_t HoneyNode::publish(uint8_t channel, void *payload) {
    // Writes a binary payload
    if (channel >= 64) channel -= 64;
    if (channel <= 31 && channel >= 0)
        return write(payload, channel + 64, registry[channel]);
    else return 3;
}

uint8_t HoneyNode::publish(uint8_t channel, String payload) {
    // Write a string payload
    if (channel >= 64) channel -= 64;
    if (channel >= 32 && channel <= 63)
        return write((void *)payload.c_str(), channel + 64, payload.length() + 1);
    else return 3;
}

void HoneyNode::listenTo(uint8_t channel, handlerFunc callback) {
    if (channel >=64) channel -= 64;
    callbacks[channel] = callback;
}
