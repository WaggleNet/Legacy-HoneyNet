#pragma once
#include "Arduino.h"

#include "wemos_pins.h"

// EEPROM Locations

#define EE_WIFI_MODE 1

// Pins

#define LCD_PIN_DC PIN_D8
#define LCD_PIN_CE PIN_D0
#define LCD_PIN_RST -1

#define SWITCH_PIN PIN_D4

#define RF_PIN_CE 4
#define RF_PIN_CS 5

// Set-up

#define RF_CHANNEL 123

// Constants

#define WIFI_MODE_AP 0
#define WIFI_MODE_STA 1

String getContentType(String);
