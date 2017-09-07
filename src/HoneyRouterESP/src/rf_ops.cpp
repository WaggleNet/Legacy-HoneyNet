#include "rf_ops.h"

RF24 radio(RF_PIN_CE, RF_PIN_CS);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

#define DEBUG 1

uint8_t registry[32];
uint8_t channel_counter = 0;
uint16_t trfc_counter = 0;

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
    if (registry[channel] == 0) channel_counter ++;
    registry[channel] = size;
    print_radio_info();
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
        byte *payload;
        payload = new byte[data_size];
        network.read(header, payload, data_size);
        if (header.type == 64) {
            // Handle channel registration
            channel_t *ch = (channel_t *) payload;
            Serial.print("Handling REG ch#");
            Serial.print(ch -> ch_id);
            Serial.print(" with size ");
            Serial.println(ch -> size);
            registerChannel(ch -> ch_id, ch -> size);
        } else {
            if (!isStrChannel(header.type)) {
                if (getChannelSize(header.type) == 0) {
                    Serial.print("UNDEF - ch#");
                    Serial.print(header.type);
                    Serial.print(" is now size ");
                    Serial.println(data_size);
                    registerChannel(header.type, data_size);
                }
                // Check consistency
                else if (getChannelSize(header.type) != data_size) {
                    Serial.print("WARNING - Size inconsistent: Expected ");
                    Serial.print(getChannelSize(header.type));
                    Serial.print(", got ");
                    Serial.println(data_size);
                    registerChannel(header.type, data_size);
                }
            }
            auto topic = generate_topic(mesh.getNodeID(header.from_node), header.type, "status");
            mqclient.publish(topic.c_str(), payload, data_size);
        }
        trfc_counter ++;
        delete[] payload;
	}
}

void print_radio_info() {
    display_clear_line(4, 5);
    display.setTextColor(WHITE, BLACK);
    display.println("CH NODE TRFC  ");
    display.setTextColor(BLACK, WHITE);
    display.print(channel_counter);
    display.setCursor(18, 40);
    display.print(mesh.addrListTop);
    display.setCursor(48, 40);
    display.print(trfc_counter);
    trfc_counter = 0;
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
