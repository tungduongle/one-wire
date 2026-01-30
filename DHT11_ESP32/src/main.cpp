#include <Arduino.h>
#include "DHT11.h"

// Khởi tạo DHT11 sử dụng GPIO4
DHT11 dht11(4);

void setup() {
  // Khởi tạo Serial với baud rate 115200
  Serial.begin(9600);
  delay(2000);  // Chờ Serial stabilize
  
  Serial.println("\n\n=== DHT11 Sensor Reading ===");
  Serial.println("Reading temperature and humidity from DHT11 on GPIO4");
  Serial.println("Waiting 2 seconds for sensor to initialize...");
  delay(2000);
}

void loop() {
  // Đọc dữ liệu từ cảm biến
  DHT11::Data data = dht11.read();
  
  // Kiểm tra xem dữ liệu có hợp lệ không
  if (data.valid) {
    Serial.print("✓ Temperature: ");
    Serial.print(data.temperature);
    Serial.print(" °C | Humidity: ");
    Serial.print(data.humidity);
    Serial.println(" %");
  } else {
    Serial.println("✗ Failed to read data from DHT11 - Check wiring and pull-up resistor");
  }
  
  // Đọc lại sau 3 giây (DHT11 cần ít nhất 2 giây giữa các lần đọc)
  delay(3000);
}