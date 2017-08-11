#include "rf_ops.h"

RF24 radio(4,5);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

uint8_t registry[32];

void radio_init(uint8_t node_id) {
    radio.setPALevel(RF24_PA_MAX);
    mesh.setNodeID(node_id);
    Serial.println("[Radio] Configuring mesh network...");
	mesh.begin(63, RF24_2MBPS);
    Serial.println("[Radio] Mesh network configuration complete");
}

String generate_topic(int16_t from_node, uint8_t header_type, char *msg_type) {
    String topic = base_topic + from_node;
    topic += "/";
    topic += header_type;
    topic += "/";
    topic += msg_type;
    return topic;
}

void registerChannel(uint8_t channel, uint8_t size) {
    if (channel >=64) channel -= 64;
    registry[channel] = size;
}

uint8_t getChannelSize(uint8_t channel) {
    if (channel >= 64) channel -= 64;
    return registry[channel];
}

uint8_t isStrChannel(uint8_t channel) {
    if (channel >= 64) channel -= 64;
    return (channel >= 32 && channel <= 63);
}

void radio_update() {
    mesh.update();
	mesh.DHCP();
    // Check for incoming data from the sensors
    if(network.available()){
		RF24NetworkHeader header;
        size_t data_size = network.peek(header);
        #ifdef DEBUG
    		Serial.print("[Radio] Rx Pkt from ");
            Serial.print(header.from_node);
            Serial.print(", node ");
            Serial.print(mesh.getNodeID(header.from_node));
            Serial.print(", size ");
            Serial.println(data_size);
        #endif
        // Assemble memory for MQTT forwarding
        auto topic = generate_topic(mesh.getNodeID(header.from_node), header.type, "status");
        byte *payload;
        payload = new byte[data_size];
        network.read(header, payload, data_size);
        if (!isStrChannel(header.type)) {
            // Check consistency
            if (getChannelSize(header.type) != data_size)
                Serial.print("WARNING: Size inconsistent.");
        }
        mqclient.publish(topic.c_str(), payload, data_size);
        delete[] payload;
	}
}

void print_assigned_addresses() {
    Serial.println(" ");
    Serial.println(F("[Radio] ********Assigned Addresses********"));
    for(int i=0; i<mesh.addrListTop; i++){
        Serial.print("[Radio] NodeID: ");
        Serial.print(mesh.addrList[i].nodeID);
        Serial.print(" RF24Network Address: 0");
        Serial.println(mesh.addrList[i].address,OCT);
    }
    Serial.println(F("[Radio] **********************************"));
}
