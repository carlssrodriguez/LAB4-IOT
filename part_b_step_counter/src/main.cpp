#include <Arduino.h>
#include <Wire.h>
#include <NimBLEDevice.h>

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

int steps = 0;
bool stepDetected = false;
unsigned long lastStepTime = 0;
const float threshold = 1.2;
const int stepCooldown = 300;

NimBLECharacteristic* pStepChar = nullptr;
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  delay(100);

  writeRegister(CTRL1_XL, 0x40);
  delay(100);

  NimBLEDevice::init("SDSUCS");
  NimBLEServer* pServer = NimBLEDevice::createServer();
  NimBLEService* pService = pServer->createService(SERVICE_UUID);

  pStepChar = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
  );

  pStepChar->setValue("0");
  pService->start();
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("BLE server ready. Start walking to count steps.");
}

void loop() {
  int16_t az_raw = read16bitRegister(OUTX_L_A + 4);
  float az = az_raw * 0.000061;
  unsigned long now = millis();

  if (az > threshold && !stepDetected && (now - lastStepTime > stepCooldown)) {
    steps++;
    stepDetected = true;
    lastStepTime = now;

    char stepStr[10];
    sprintf(stepStr, "%d", steps);
    pStepChar->setValue(stepStr);
    pStepChar->notify();

    Serial.printf("Step %d | Z: %.2f g\n", steps, az);
  }

  if (az < 1.0) {
    stepDetected = false;
  }

  delay(20);
}
