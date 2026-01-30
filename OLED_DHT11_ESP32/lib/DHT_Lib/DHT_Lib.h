#ifndef DHT_LIB_H
#define DHT_LIB_H

#include <Arduino.h>
#include <DHT.h>

class DHT_Lib {
private:
    DHT dht;

public:
    DHT_Lib(uint8_t pin, uint8_t type);
    void begin();
    float readTemperature();
    float readHumidity();
};

#endif
