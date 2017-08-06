#include "wifi_ops.h"
#include "rf_ops.h"

#define CH_TIMER 13

uint32_t displayTimer = 0;

void setup() {
	Serial.begin(115200);
	Serial.println();
	radio_init();
	wifi_init();

	registerChannel(CH_TIMER, sizeof(uint32_t));
}


void loop() {
	radio_update();
	wifi_update();
	if(millis() - displayTimer > 20000){
		displayTimer = millis();
		print_assigned_addresses();
		print_wifi_info();
		mqtt_refresh_state();
	}
	mqtt_loop();
}
