#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "00000002-0000-0000-FDFD-FDFDFDFDFDFD"
#define CHARACTERISTIC_UUID_TEMPERATURE "00002A1C-0000-1000-8000-00805F9B34FB" // Temperature Measurement
#define CHARACTERISTIC_UUID_HUMIDITY "00002A6F-0000-1000-8000-00805F9B34FB" // Humidity

BLECharacteristic *pTemperatureCharacteristic;
BLECharacteristic *pHumidityCharacteristic;

float getTemperature() {
  // Mimic temperature sensor reading
  return 25.0 + (rand() % 100) / 10.0; // Return a value between 25.0 and 35.0
}

float getHumidity() {
  // Mimic humidity sensor reading
  return 50.0 + (rand() % 100) / 10.0; // Return a value between 50.0 and 60.0
}

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32_BLE_Server");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics
  pTemperatureCharacteristic = pService->createCharacteristic(
                                 CHARACTERISTIC_UUID_TEMPERATURE,
                                 BLECharacteristic::PROPERTY_READ |
                                 BLECharacteristic::PROPERTY_NOTIFY
                               );
  pTemperatureCharacteristic->addDescriptor(new BLE2902());

  pHumidityCharacteristic = pService->createCharacteristic(
                              CHARACTERISTIC_UUID_HUMIDITY,
                              BLECharacteristic::PROPERTY_READ |
                              BLECharacteristic::PROPERTY_NOTIFY
                            );
  pHumidityCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Waiting for a client connection to notify...");
}

void loop() {
  // Update temperature and humidity readings
  float temperature = getTemperature();
  float humidity = getHumidity();

  pTemperatureCharacteristic->setValue(temperature);
  pHumidityCharacteristic->setValue(humidity);

  pTemperatureCharacteristic->notify();
  pHumidityCharacteristic->notify();

  Serial.printf("Temperature: %.2f, Humidity: %.2f\n", temperature, humidity);

  delay(2000); // Delay for 2 seconds before next update
}