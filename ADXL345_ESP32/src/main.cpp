#include <SPI.h>
#include <Arduino.h>

#define ADXL_CS   5
#define ADXL_SCK  18
#define ADXL_MISO 19
#define ADXL_MOSI 23

// Thanh ghi
#define REG_DEVID       0x00
#define REG_POWER_CTL   0x2D
#define REG_DATA_FORMAT 0x31
#define REG_DATAX0      0x32

// Tốc độ SPI thấp (100kHz) để loại trừ nhiễu dây dẫn
SPISettings settingsA(100000, MSBFIRST, SPI_MODE3);

void writeRegister(uint8_t reg, uint8_t value) {
    SPI.beginTransaction(settingsA);
    digitalWrite(ADXL_CS, LOW);
    SPI.transfer(reg & 0x7F); // Bit 7 = 0 để Ghi
    SPI.transfer(value);
    digitalWrite(ADXL_CS, HIGH);
    SPI.endTransaction();
    delay(5); // Chờ chip xử lý
}

uint8_t readRegister(uint8_t reg) {
    uint8_t result;
    SPI.beginTransaction(settingsA);
    digitalWrite(ADXL_CS, LOW);
    SPI.transfer(0x80 | reg); // Bit 7 = 1 để Đọc
    result = SPI.transfer(0x00);
    digitalWrite(ADXL_CS, HIGH);
    SPI.endTransaction();
    return result;
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(ADXL_CS, OUTPUT);
    digitalWrite(ADXL_CS, HIGH);
    
    // Khởi tạo chân SPI
    SPI.begin(ADXL_SCK, ADXL_MISO, ADXL_MOSI, ADXL_CS);

    Serial.println("\n=== KHOI PHUC ADXL345 SPI (DEFENSIVE MODE) ===");

    // 1. Kiểm tra kết nối (MISO)
    uint8_t id = readRegister(REG_DEVID);
    Serial.printf("1. DEVID: 0x%02X ", id);
    if (id == 0xE5) Serial.println("[OK]");
    else {
        Serial.println("[LOI] Kiem tra lai day MISO/SCK");
        while(1);
    }

    // 2. Ép chế độ 4-wire và Full Resolution (Ghi 0x0B)
    // Nếu vẫn ra 0xCB, nghĩa là bit 7 bị nhiễu từ đường Read sang
    writeRegister(REG_DATA_FORMAT, 0x0B); 
    uint8_t format = readRegister(REG_DATA_FORMAT);
    Serial.printf("2. DATA_FORMAT: 0x%02X ", format);
    if (format == 0x0B) Serial.println("[OK]");
    else Serial.println("[CANH BAO] Ghi du lieu bi loi, kiem tra day MOSI");

    // 3. Kích hoạt chế độ đo
    writeRegister(REG_POWER_CTL, 0x08);
    uint8_t pwr = readRegister(REG_POWER_CTL);
    Serial.printf("3. POWER_CTL: 0x%02X ", pwr);
    if (pwr == 0x08) Serial.println("[OK - CHIP DA BAT]");
    else Serial.println("[LOI] Chip khong chiu tinh giac");

    Serial.println("---------------------------------------");
}

void loop() {
    SPI.beginTransaction(settingsA);
    digitalWrite(ADXL_CS, LOW);
    
    // Đọc 6 byte: Bit 7(Read)=1, Bit 6(MB)=1
    SPI.transfer(0x80 | 0x40 | REG_DATAX0); 
    
    uint8_t x_l = SPI.transfer(0x00);
    uint8_t x_h = SPI.transfer(0x00);
    uint8_t y_l = SPI.transfer(0x00);
    uint8_t y_h = SPI.transfer(0x00);
    uint8_t z_l = SPI.transfer(0x00);
    uint8_t z_h = SPI.transfer(0x00);
    
    digitalWrite(ADXL_CS, HIGH);
    SPI.endTransaction();

    // Ghép dữ liệu 16-bit
    int16_t x = (int16_t)((x_h << 8) | x_l);
    int16_t y = (int16_t)((y_h << 8) | y_l);
    int16_t z = (int16_t)((z_h << 8) | z_l);

    // Tính toán m/s2 (Full Resolution: 3.9mg/LSB)
    float accX = x * 0.0039 * 9.81;
    float accY = y * 0.0039 * 9.81;
    float accZ = z * 0.0039 * 9.81;

    Serial.printf("X: %6.2f | Y: %6.2f | Z: %6.2f m/s2\n", accX, accY, accZ);

    delay(200);
}