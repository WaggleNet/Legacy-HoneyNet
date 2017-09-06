#include "lcd_ops.h"
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

void ota_init() {
    ArduinoOTA.onStart([]() {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("OTA Update");
        display.display();
    });
    ArduinoOTA.onEnd([]() {
        display_clear_line(3, 4);
        display.println("Complete!");
        display.display();
        delay(1000);
        ESP.reset();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        display_clear_line(1, 2);
        display.println("Progress:");
        display.print(progress);
        display.print('/');
        display.println(total);
        display.display();
    });
    ArduinoOTA.onError([](ota_error_t error) {
        display_clear_line(3, 4);
        display.print("ERROR ");
        display.println(error);
        if (error == OTA_AUTH_ERROR) display.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) display.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) display.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) display.println("Receive Failed");
        else if (error == OTA_END_ERROR) display.println("End Failed");
        display.display();
        delay(2000);
        ESP.reset();
    });
    ArduinoOTA.begin();
}

void ota_update() {
    ArduinoOTA.handle();
}
