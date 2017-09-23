#include "lcd_ops.h"

Adafruit_PCD8544 display = Adafruit_PCD8544(LCD_PIN_DC, LCD_PIN_CE, LCD_PIN_RST);

void display_init() {
    display.begin();
    display.setContrast(60);
    display.setTextSize(1);
	display.setTextColor(BLACK, WHITE);
	display.setRotation(2);
    display.display();
    delay(500);
    display.clearDisplay();
    display.display();
}

void display_clear_line(uint8_t line_start, uint8_t line_end) {
    display.setTextSize(1);
    display.fillRect(0, line_start*8, 84, (line_end-line_start+1)*8, WHITE);
    display.setCursor(0, line_start*8);
}
