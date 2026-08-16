#include "BLERadio.h"

QueueHandle_t BLERadio::xQueueBLEIncoming = NULL;

void BLERadio::begin(const char* deviceName) {
    xQueueBLEIncoming = xQueueCreate(10, 140 * sizeof(char));

    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(this);

    BLEService* pService = pServer->createService(SERVICE_UUID);

    // TX Characteristic (ESP32 -> Phone)
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxCharacteristic->addDescriptor(new BLE2902());

    // RX Characteristic (Phone -> ESP32)
    BLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
    );
    pRxCharacteristic->setCallbacks(this);

    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void BLERadio::onConnect(BLEServer* pServer) {
    deviceConnected = true;
}

void BLERadio::onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    pServer->startAdvertising(); // Restart advertising so phone can reconnect
}

void BLERadio::onWrite(BLECharacteristic* pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0 && xQueueBLEIncoming != NULL) {
        char buffer[140];
        size_t len = min(rxValue.length(), sizeof(buffer) - 1);
        memcpy(buffer, rxValue.c_str(), len);
        buffer[len] = '\0';

        xQueueSend(xQueueBLEIncoming, buffer, portMAX_DELAY);
    }
}

void BLERadio::sendToPhone(const char* message) {
    if (deviceConnected && pTxCharacteristic != nullptr) {
        pTxCharacteristic->setValue((uint8_t*)message, strlen(message));
        pTxCharacteristic->notify();
    }
}