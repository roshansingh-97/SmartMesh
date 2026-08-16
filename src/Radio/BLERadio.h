#ifndef BLE_RADIO_H
#define BLE_RADIO_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Config.h"

// Standard Nordic UART Service UUIDs
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class BLERadio : public BLEServerCallbacks, public BLECharacteristicCallbacks {
private:
    BLEServer* pServer = nullptr;
    BLECharacteristic* pTxCharacteristic = nullptr;
    bool deviceConnected = false;
    bool oldDeviceConnected = false;

public:
    static QueueHandle_t xQueueBLEIncoming; // Incoming text from Phone

    void begin(const char* deviceName = "SmartMesh Terminal");
    void sendToPhone(const char* message);
    bool isConnected() const { return deviceConnected; }

    // BLE Callbacks
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
    void onWrite(BLECharacteristic* pCharacteristic) override;
};

#endif // BLE_RADIO_H