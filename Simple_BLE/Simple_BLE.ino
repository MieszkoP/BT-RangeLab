#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
//#include "esp_gap_ble_api.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
// https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf

//esp_err_t esp_ble_gap_set_path_loss_reporting_enable(uint16_t conn_handle, bool enable);

#define LUT_tx(i) (_LUT_tx[(i) + 12])

#define SERVICE_UUID        "0000185A-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_TX "00002A07-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_RSSI "e2969b90-7759-44dc-b7db-a477c2e3122e"
#define CHARACTERISTIC_LINK_LOSS "e2969b90-7759-44dc-b7db-a477c2e3122f"

uint8_t _LUT_tx[22] = {0};


//Tabela tlumaczy Db na odpowiedni bajt. Np. 9dB -> 0x07. -6dB -> 0x02 itd. 
void setupLUT_tx()
{
  uint8_t i = 12;
  _LUT_tx[i-12] = 0;
  _LUT_tx[i-9] = 1;
  _LUT_tx[i-6] = 2;
  _LUT_tx[i-3] = 3;
  _LUT_tx[i-0] = 4;
  _LUT_tx[i+3] = 5;
  _LUT_tx[i+6] = 6;
  _LUT_tx[i+9] = 7;
}

int8_t power_level;
int8_t rssi_level;
int8_t estimated_LL_level;
BLECharacteristic *pCharacteristic_tx;
BLECharacteristic *pCharacteristic_rssi;
BLECharacteristic *pCharacteristic_estimated_power_loss;

void setup() {
  setupLUT_tx();
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  BLEDevice::init("Przykladowa Nazwa");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  pCharacteristic_tx = pService->createCharacteristic(CHARACTERISTIC_TX, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic_tx->setValue((uint8_t*)&power_level, sizeof(power_level));

  pCharacteristic_rssi = pService->createCharacteristic(CHARACTERISTIC_RSSI, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic_rssi->setValue((uint8_t*)&rssi_level, sizeof(rssi_level));

  pCharacteristic_estimated_power_loss = pService->createCharacteristic(CHARACTERISTIC_LINK_LOSS, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic_estimated_power_loss->setValue((uint8_t*)(&estimated_LL_level), sizeof(estimated_LL_level)); //Zapisujemy zawsze jako uint8_t, intepretacji dokonuje aplikacja!

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

}


void loop()
{
   String val = pCharacteristic_tx->getValue();
  int8_t power_dBm;
  if (val.length() > 0) {
    power_dBm = (int8_t)val[0];
    esp_power_level_t power = (esp_power_level_t)LUT_tx(power_dBm);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL0, power);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL1, power);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL2, power);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL3, power);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL4, power);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL5, power);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL6, power);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL7, power);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL8, power);
    Serial.print("Ustawiono moc: ");
    Serial.println(power);
  }
  rssi_level = *(int8_t*)pCharacteristic_rssi->getData();
  Serial.print("Power Loss: ");
  estimated_LL_level = power_dBm - rssi_level + 3 + 2;
  Serial.println(estimated_LL_level);
  pCharacteristic_estimated_power_loss->setValue((uint8_t*)(&estimated_LL_level), sizeof(estimated_LL_level));
  pCharacteristic_estimated_power_loss->notify();
  delay(2000);
}
