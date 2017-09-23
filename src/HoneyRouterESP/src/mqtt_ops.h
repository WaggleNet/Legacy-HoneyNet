#pragma once

#include "PubSubClient.h"

#include "rf_ops.h"
#include "wifi_ops.h"
#include "config.h"
#include "lcd_ops.h"

extern PubSubClient mqclient;
extern String base_topic;
extern uint8_t mqtt_on;

void mqtt_refresh_state();
void print_mqtt_info();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void mqtt_loop();
