#include "DHT_Lib.h"

DHT_Lib::DHT_Lib(uint8_t pin, uint8_t type)
    : dht(pin, type) {}

void DHT_Lib::begin() {
    dht.begin();
}

float DHT_Lib::readTemperature() {
    return dht.readTemperature();
}

float DHT_Lib::readHumidity() {
    return dht.readHumidity();
}
