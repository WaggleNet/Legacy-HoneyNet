#pragma once

#include "PubSubClient.h"

#include "rf_ops.h"
#include "wifi_ops.h"

extern PubSubClient mqclient;
extern String base_topic;

void mqtt_refresh_state();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void mqtt_loop();
