#pragma once
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "config.h"

extern Adafruit_PCD8544 display;

void display_init();
void display_clear_line(uint8_t, uint8_t);
