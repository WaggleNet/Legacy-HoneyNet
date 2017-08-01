#include "HoneyNode.h"

HoneyNode::HoneyNode(uint8_t CE_pin, uint8_t CS_pin)
    : radio(RF24(CE_pin, CS_pin)), network(RF24Network(radio)), mesh(RF24Mesh(radio, network)) {
}

void HoneyNode::begin() {
    serialSetID();
    Serial.println(F("Connecting to the mesh..."));
    mesh.begin();
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
}

void HoneyNode::registerChannel(uint8_t channel, uint8_t size) {
    if (channel >=64) channel -= 64;
    registry[channel] = size;
}

uint8_t HoneyNode::write(uint8_t ch, byte *payload, uint8_t len) {
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
        Serial.println("Connection lost: Renewing Address");
        mesh.renewAddress();
        return 2;
      } else return 1;
    } else return 0;
}

uint8_t HoneyNode::publish(uint8_t channel, byte *payload) {
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
        return write(payload.c_str(), channel + 64, payload.length() + 1);
    else return 3;
}
