#ifndef OLED_LIB_H
#define OLED_LIB_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class OLED_Lib {
private:
    Adafruit_SSD1306 display;

public:
    OLED_Lib();
    void begin();
    void showData(float temp, float hum);
};

#endif
