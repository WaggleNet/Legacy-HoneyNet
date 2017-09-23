#include "spiffs_params.h"

namespace fs {

    String read_str(String filename) {
        if (SPIFFS.exists(filename)) {
            auto f = SPIFFS.open(filename, "r");
            auto s = f.readString();
            f.close();
            return s;
        } else return String("");
    }

    void write_str(String filename, String content) {
        auto f = SPIFFS.open(filename, "w");
        f.print(content);
        f.close();
    }

}

namespace param {

    String get_wifi_ssid() {
        return fs::read_str("/wifi/ssid");
    }

    String get_wifi_password() {
        return fs::read_str("/wifi/password");
    }

    void set_wifi_ssid(String s) {
        return fs::write_str("/wifi/ssid", s);
    }

    void set_wifi_password(String s) {
        return fs::write_str("/wifi/password", s);
    }

}
