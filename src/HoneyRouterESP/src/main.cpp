#include "wifi_ops.h"
#include "rf_ops.h"
#include "lcd_ops.h"
#include <EEPROM.h>
#include <FS.h>
#include "config.h"
#include "ota_ops.h"

uint32_t displayTimer = 0;

void setup() {
	Serial.begin(115200);
	Serial.println();
    EEPROM.begin(128);
    SPIFFS.begin();
    display_init();
	radio_init();
	wifi_init();
    print_mqtt_info();
    ota_init();
	mqtt_init();
}


void loop() {
	radio_update();
	wifi_update();
	if(millis() - displayTimer > 5000){
		displayTimer = millis();
		print_assigned_addresses();
		print_wifi_info();
        print_mqtt_info();
		mqtt_refresh_state();
		print_radio_info();
	}
	mqtt_loop();
    ota_update();
}
