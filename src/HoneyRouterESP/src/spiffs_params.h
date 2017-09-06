#include <FS.h>
#include "Arduino.h"

namespace fs {
    String read_str(String);
    void write_str(String, String);
}

namespace param {
    String get_wifi_ssid();
    String get_wifi_password();
    void set_wifi_ssid(String);
    void set_wifi_password(String);
}
