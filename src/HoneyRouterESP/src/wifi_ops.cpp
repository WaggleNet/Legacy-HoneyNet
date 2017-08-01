#include "wifi_ops.h"

const char *ssid = "esp_mesh_master_demo";
const char *password = "19960223";

String mqtt_broker_address = "";
uint8_t mqtt_broker_enable = 0;

ESP8266WebServer server(80);
WiFiClient wclient;

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
		Serial.println(F("[MQTT] Broker enabled via REST."));
		Serial.print(F("[MQTT] Address set to "));
		Serial.println(mqtt_broker_address);
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
	WiFi.mode(WIFI_AP_STA);
	WiFi.begin("wlan_15_502", "929917223");
	WiFi.softAP(ssid, password);
	auto myIP = WiFi.softAPIP();
	Serial.print(F("[Wifi] AP IP address: "));
	Serial.println(myIP);
	setup_routes();
	server.begin();
}

void wifi_update() {
	server.handleClient();
}

void print_wifi_info() {
	Serial.println(F("[Wifi] ********WiFi Information********"));
	if (WiFi.status() == WL_CONNECTED) {
		Serial.println(F("[Wifi] Status: Connected"));
	} else {
		Serial.println(F("[Wifi] Status: Disconnected"));
	}
	Serial.print(F("[Wifi] Connected as:"));
	Serial.println(WiFi.localIP());
	Serial.println(F("[Wifi] ********************************"));
}
