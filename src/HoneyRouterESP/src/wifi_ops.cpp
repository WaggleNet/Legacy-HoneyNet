#include "wifi_ops.h"

String mqtt_broker_address = "";
uint8_t mqtt_broker_enable = 0;

ESP8266WebServer server(80);
WiFiClient wclient;

#define ssid "esp_mesh_master_demo"
#define password "12345678"

void mode_ap_begin() {
	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password);
	auto myIP = WiFi.softAPIP();
	// Finally, print everything out...
	print_wifi_info();
}

void mode_sta_begin() {
	WiFi.mode(WIFI_STA);
	// Finally, print everything out...
	print_wifi_info();
}

void route_root() {
	String message = "<html><head></head><body>";
	message += "<h4>Assigned Addresses</h4><ul>";
	for (int i = 0; i < mesh.addrListTop; i++) {
		message += "<li><b>ID: ";
		message += String(mesh.addrList[i].nodeID) + "</b>";
		message += " Address: ";
		message += String(mesh.addrList[i].address);
		message += "</li>";
	}
	message += "</ul></body></html>";
	server.send(200, "text/html", message);
}

void route_enable_mqtt() {
	if (server.hasArg("address")) {
		mqtt_broker_address = String(server.arg("address"));
		mqtt_broker_enable = 1;
		print_mqtt_info();
		server.send(200, "application/json", "{\"status\": \"success\"}");
	} else {
		server.send(200, "application/json", "{\"status\": \"error\"}");
	}
}

void route_disable_mqtt() {
	mqtt_broker_enable = 0;
	Serial.println(F("[MQTT] Broker disabled via REST."));
	server.send(200, "application/json", "{\"status\": \"success\"}");
}

void setup_routes() {
	server.on("/", route_root);
	server.on("/mqtt/enable", route_enable_mqtt);
	server.on("/mqtt/disable", route_disable_mqtt);
}

void wifi_init() {
	Serial.println(F("[Wifi] Configuring access point..."));
	uint8_t stored_mode = EEPROM.read(EE_WIFI_MODE);
	switch (stored_mode) {
		case WIFI_MODE_AP:
			mode_ap_begin();
			break;
		case WIFI_MODE_STA:
			mode_sta_begin();
			break;
		default:
			EEPROM.write(EE_WIFI_MODE, WIFI_MODE_AP);
			EEPROM.commit();
			mode_ap_begin();
	}
	setup_routes();
	server.begin();
}

void wifi_update() {
	server.handleClient();
}

void print_wifi_info() {
	display_clear_line(0, 1);
	display.setTextWrap(false);
	Serial.println(F("[Wifi] ********WiFi Information********"));
	display.print(F("WIFI:"));
	if (WiFi.getMode() == WIFI_AP) {
		display.println(F("AP Mode"));
		display.println(WiFi.softAPIP());
		Serial.print(F("[Wifi] SoftAP, IP: "));
	}
	else {
		display.println(WiFi.SSID());
		if (WiFi.status() == WL_CONNECTED) {
			Serial.print(F("[Wifi] Status: Connected as "));
			Serial.println(WiFi.localIP());
			display.println(WiFi.localIP());
		} else {
			Serial.println(F("[Wifi] Status: Disconnected"));
			display.println("!Disconnected!");
		}
	}
	Serial.println(F("[Wifi] ********************************"));
	display.display();
}
