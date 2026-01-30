#include <Arduino.h>
#include "DHT_Lib.h"
#include "OLED_Lib.h"

#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT_Lib dht(DHT_PIN, DHT_TYPE);
OLED_Lib oled;

void setup() {
    Serial.begin(9600);

    dht.begin();
    oled.begin();
}

void loop() {
    float temperature = dht.readTemperature();
    float humidity    = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
        oled.showData(temperature, humidity);
    } else {
        Serial.println("Loi doc DHT11");
    }

    delay(2000);
}
