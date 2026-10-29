#include "BLERadio.h"

QueueHandle_t BLERadio::xQueueBLEIncoming = nullptr;

void BLERadio::begin(const char* deviceName) {
    // This class should be initialized only once by SmartMeshApp.
    if (xQueueBLEIncoming == nullptr) {
        xQueueBLEIncoming = xQueueCreate(10, 140 * sizeof(char));
    }
    if (xQueueBLEIncoming == nullptr) {
        Serial.println("[BLE] ERROR: Failed to create incoming-message queue.");
        return;
    }

    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    if (pServer == nullptr) {
        Serial.println("[BLE] ERROR: Failed to create BLE server.");
        return;
    }
    pServer->setCallbacks(this);

    BLEService* pService = pServer->createService(SERVICE_UUID);
    if (pService == nullptr) {
        Serial.println("[BLE] ERROR: Failed to create Nordic UART service.");
        return;
    }

    // TX: ESP32 -> phone. The phone must enable notifications on this characteristic.
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxCharacteristic->addDescriptor(new BLE2902());

    // RX: phone -> ESP32.
    BLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );
    pRxCharacteristic->setCallbacks(this);

    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.print("[BLE] Advertising as: ");
    Serial.println(deviceName);
    Serial.println("[BLE] RX UUID: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
    Serial.println("[BLE] TX UUID: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
}

void BLERadio::onConnect(BLEServer* server) {
    deviceConnected = true;
    Serial.println("[BLE] Phone connected. Select/enable TX notifications in the phone app.");
}

void BLERadio::onDisconnect(BLEServer* server) {
    deviceConnected = false;
    Serial.println("[BLE] Phone disconnected. Restarting advertising...");
    if (server != nullptr) {
        server->startAdvertising();
    } else {
        BLEDevice::startAdvertising();
    }
}

void BLERadio::onWrite(BLECharacteristic* characteristic) {
    if (characteristic == nullptr) {
        return;
    }

    std::string rxValue = characteristic->getValue();
    Serial.print("[BLE RX] onWrite; bytes=");
    Serial.println(static_cast<unsigned int>(rxValue.length()));

    if (rxValue.empty() || xQueueBLEIncoming == nullptr) {
        return;
    }

    char buffer[140] = {};
    size_t len = (rxValue.length() < sizeof(buffer) - 1)
                     ? rxValue.length()
                     : sizeof(buffer) - 1;
    memcpy(buffer, rxValue.data(), len);
    // Phone terminal apps often append CR/LF. Strip line endings before the text
    // is stored or forwarded, so the OLED Inbox receives a clean message.
    while (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n')) {
        --len;
    }
    buffer[len] = '\0';
    if (len == 0) {
        return;
    }

    // BLE callbacks run in task context, so use the normal FreeRTOS queue API.
    if (xQueueSend(xQueueBLEIncoming, buffer, 0) == pdTRUE) {
        Serial.print("[BLE RX] queued: ");
        Serial.println(buffer);
    } else {
        Serial.println("[BLE RX] ERROR: incoming queue full; data dropped.");
    }
}

void BLERadio::sendToPhone(const char* message) {
    if (message == nullptr || !deviceConnected || pTxCharacteristic == nullptr) {
        Serial.println("[BLE TX] Not sent: no active BLE connection/characteristic.");
        return;
    }

    const size_t len = strlen(message);
    if (len == 0) {
        return;
    }

    // Default ATT MTU is often 23 bytes, leaving 20 bytes for a notification.
    // Fragment longer messages rather than trying to send all bytes in one notify.
    constexpr size_t MAX_NOTIFY_PAYLOAD = 20;
    for (size_t offset = 0; offset < len; offset += MAX_NOTIFY_PAYLOAD) {
        size_t chunkLen = len - offset;
        if (chunkLen > MAX_NOTIFY_PAYLOAD) {
            chunkLen = MAX_NOTIFY_PAYLOAD;
        }

        pTxCharacteristic->setValue(
            reinterpret_cast<uint8_t*>(const_cast<char*>(message + offset)),
            chunkLen
        );
        pTxCharacteristic->notify();
        delay(10); // Give the BLE stack time to queue each notification.
    }

    // Line-oriented terminal apps commonly display a message only after a line ending.
    const bool hasLineEnding = message[len - 1] == '\n' || message[len - 1] == '\r';
    if (!hasLineEnding) {
        uint8_t newline[2] = {'\r', '\n'};
        pTxCharacteristic->setValue(newline, sizeof(newline));
        pTxCharacteristic->notify();
        delay(10);
    }

    Serial.print("[BLE TX] notified ");
    Serial.print(static_cast<unsigned int>(len));
    Serial.println(" text bytes.");
}
