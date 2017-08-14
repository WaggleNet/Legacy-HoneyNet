#include "wifi_ops.h"
#include "rf_ops.h"

struct sensors_t {
    uint8_t temperature;
    uint8_t humidity;
    uint16_t lightness_1;
    uint16_t lightness_2;
    uint32_t timer;
};

struct commands_t {
    int16_t lamp;
    int8_t indicators;
};

#define CH_SENSORS 5 // Combined Data at 69=5+64
#define CH_COMMANDS 6 // Commands at 70=6+64
#define CH_TIMER 13

uint32_t displayTimer = 0;

void setup() {
	Serial.begin(115200);
	Serial.println();
	radio_init();
	wifi_init();

	registerChannel(CH_TIMER, sizeof(uint32_t));
	registerChannel(CH_SENSORS, sizeof(sensors_t));
	registerChannel(CH_COMMANDS, sizeof(commands_t));
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
