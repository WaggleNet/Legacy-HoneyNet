#pragma once

#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "rf_ops.h"
#include "lcd_ops.h"
#include "config.h"
#include "spiffs_params.h"

extern ESP8266WebServer server;
extern WiFiClient wclient;

extern String mqtt_broker_address;
extern uint8_t mqtt_broker_enable;

void mode_ap_begin(boolean);
void mode_sta_begin(boolean);

void route_root();
void setup_routes();
void wifi_init();
void wifi_update();
void print_wifi_info();
