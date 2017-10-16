#pragma once

#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "ESPTemplateProcessor.h"

#include "rf_ops.h"
#include "lcd_ops.h"
#include "config.h"
#include "spiffs_params.h"

extern ESP8266WebServer server;
extern WiFiClient wclient;

void mode_ap_begin(boolean);
void mode_sta_begin(boolean);

void route_root();
String root_processor(const String& key);
bool handleFileRead(String);
void setup_routes();
void wifi_init();
void wifi_update();
void print_wifi_info();
