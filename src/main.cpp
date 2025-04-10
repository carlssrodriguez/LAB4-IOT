#include <Arduino.h>
#include <Wire.h>

#define LSM6DSO_ADDR 0x6B
#define CTRL1_XL     0x10
#define OUTX_L_A     0x28

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(LSM6DSO_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

int16_t read16bitRegister(uint8_t regL) {
  Wire.beginTransmission(LSM6DSO_ADDR);
  Wire.write(regL);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM6DSO_ADDR, 2);

  if (Wire.available() == 2) {
    uint8_t low = Wire.read();
    uint8_t high = Wire.read();
    return (int16_t)(high << 8 | low);
  }
  return 0;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  delay(100);

  Serial.println("Initializing LSM6DSO...");

  // Accelerometer: 104 Hz, ±2g
  writeRegister(CTRL1_XL, 0x40);

  delay(100);
  Serial.println("Sensor ready. Reading acceleration...");
}

void loop() {
  int16_t ax_raw = read16bitRegister(OUTX_L_A);
  int16_t ay_raw = read16bitRegister(OUTX_L_A + 2);
  int16_t az_raw = read16bitRegister(OUTX_L_A + 4);

  float scale = 0.000061; // ±2g scale
  float ax = ax_raw * scale;
  float ay = ay_raw * scale;
  float az = az_raw * scale;

  Serial.print("X: ");
  Serial.print(ax, 3);
  Serial.print(" g | Y: ");
  Serial.print(ay, 3);
  Serial.print(" g | Z: ");
  Serial.print(az, 3);
  Serial.println(" g");

  delay(500);
}
