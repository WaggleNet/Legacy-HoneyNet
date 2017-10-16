#pragma once

#include "PubSubClient.h"

#include "rf_ops.h"
#include "wifi_ops.h"
#include "config.h"
#include "lcd_ops.h"
#include "spiffs_params.h"

extern PubSubClient mqclient;
extern String base_topic;
extern uint8_t mqtt_on;

extern String mqtt_broker_address;
extern String mqtt_username;
extern String mqtt_password;
extern uint8_t mqtt_broker_enable;

void mqtt_refresh_state();
void print_mqtt_info();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void mqtt_init();
void mqtt_loop();
