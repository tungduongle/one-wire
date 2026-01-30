#include "OLED_Lib.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1
#define OLED_ADDR    0x3C

OLED_Lib::OLED_Lib()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void OLED_Lib::begin() {
    // BẮT BUỘC init I2C trước
    Wire.begin(21, 22);
    Wire.setClock(100000); // 100kHz cho ổn định (chống nhiễu)

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED loi!");
        while (1);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void OLED_Lib::showData(float temp, float hum) {
    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("Nhiet do: ");
    display.print(temp);
    display.println(" C");

    display.setCursor(0, 20);
    display.print("Do am: ");
    display.print(hum);
    display.println(" %");

    display.display();
}
