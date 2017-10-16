#include "mqtt_ops.h"

PubSubClient mqclient;

uint8_t mqtt_on = 0;
String mqtt_broker_address = "";
String mqtt_username = "";
String mqtt_password = "";
uint8_t mqtt_broker_enable = 0;

String esp_chip_id(ESP.getChipId(), HEX);
String base_topic = String("esp-mesh/")+esp_chip_id+"/";
String node_name = String("esp-") + esp_chip_id;

boolean process_connection() {
    if (mqtt_username.length())
        return mqclient.connect(
            node_name.c_str(),
            mqtt_username.c_str(),
            mqtt_password.c_str()
        );
    else return mqclient.connect(node_name.c_str());
}

void mqtt_init() {
    mqtt_broker_address = param::get_mqtt_address();
    mqtt_username = param::get_mqtt_username();
    mqtt_password = param::get_mqtt_password();
    if (mqtt_broker_address.length() > 0) mqtt_broker_enable = 1;
}

void mqtt_refresh_state() {
    if (mqtt_broker_enable && !mqtt_on) {
        mqtt_on = 1;
        mqclient = PubSubClient(mqtt_broker_address.c_str(), 1883, mqtt_callback, wclient);
        if (process_connection()) {
            Serial.print(F("[MQTT] Connected to "));
            Serial.println(mqtt_broker_address);
            Serial.print("Base topic: ");
            Serial.println(base_topic);
            String topic = base_topic + "router/mqtt/status";
            String message = "{\"msg\": \"connected\"}";
            mqclient.publish(topic.c_str(), message.c_str());
            mqclient.subscribe((base_topic + "*/*/command").c_str());
        } else {
            Serial.println(F("[MQTT] Connection failed"));
            mqtt_on = 0;
        }
    } else if (!mqtt_broker_enable && mqtt_on) {
        mqclient.disconnect();
    }
    if (mqtt_on && !mqclient.connected()) mqtt_on = 0;
}

void print_mqtt_info() {
    display_clear_line(2, 3);
    display.setTextWrap(false);
    display.setTextColor(WHITE, BLACK);
    display.print("MQTT>");
    display.setTextColor(BLACK, WHITE);
    if (mqtt_broker_enable) {
        display.print("On,");
        display.println(mqtt_on ? "OK" : "Wait");
        display.println(mqtt_broker_address);
    } else display.println("Off");
}

void mqtt_loop() {
    if (mqtt_on)
        if (!mqclient.loop()) mqtt_on = 0;
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    Serial.print(F("[MQTT] Received message on ch#"));
    String t(topic);
    // Parse the topic String
    int endslash = t.lastIndexOf('/');
    int startslash = t.lastIndexOf('/', endslash - 1);
    int ch = t.substring(startslash + 1, endslash).toInt();
    endslash = startslash;
    startslash = t.lastIndexOf('/', endslash - 1);
    int nodeID = t.substring(startslash + 1, endslash).toInt();
    Serial.print(ch);
    Serial.print(F(", node#"));
    Serial.print(nodeID);
    Serial.print(", size ");
    Serial.println(length);
    if (!getChannelSize(ch)) registerChannel(ch, length);
    mesh.write(payload, ch, length, nodeID);
}
