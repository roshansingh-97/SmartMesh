#include "SmartMeshApp.h"
#include "../Storage/Storage.h"

// Set to 1 only for a temporary phone <-> BLE echo test. Set back to 0 for mesh use.
#define BLE_ECHO_TEST 0

SmartMeshApp::SmartMeshApp() {}

void SmartMeshApp::begin() {
    Storage.begin();
    displayManager.begin();
    uiManager.begin();

    // Initialize ESP-NOW and BLE once. The BLE object owned here is also used to send.
    radio.begin();
    bleRadio.begin("SmartMesh Terminal");

    homeScreen.initialize();
    composeScreen.initialize();
    composeScreen.setAppOwner(this);
    inboxScreen.initialize();
    sentScreen.initialize();
    settingsScreen.initialize();
    aboutScreen.initialize();
    contactsScreen.initialize();

    uiManager.setHomeScreen(&homeScreen);
    uiManager.setComposeScreen(&composeScreen);
    uiManager.setInboxScreen(&inboxScreen);
    uiManager.setSentScreen(&sentScreen);
    uiManager.setSettingsScreen(&settingsScreen);
    uiManager.setAboutScreen(&aboutScreen);
    uiManager.setContactsScreen(&contactsScreen);

    uiManager.navigateTo(&homeScreen);
}

void SmartMeshApp::update() {
    uiManager.update();

    // Receive ESP-NOW messages. Guard queues because radio initialization can fail.
    SmartMeshPacket rxPacket = {};
    if (MeshRadio::xQueueIncoming != nullptr &&
        xQueueReceive(MeshRadio::xQueueIncoming, &rxPacket, 0) == pdTRUE) {

        if (rxPacket.mode == MODE_1_PHONE_TO_PHONE || rxPacket.mode == MODE_4_SCT_TO_PHONE) {
            if (bleRadio.isConnected()) {
                bleRadio.sendToPhone(rxPacket.payload);
            } else {
                Storage.saveInboxMessage(rxPacket.senderID, rxPacket.payload);
            }
            radio.sendACK(rxPacket.msgID, rxPacket.senderID);
        } else if (rxPacket.mode == MODE_2_SCT_TO_SCT || rxPacket.mode == MODE_3_PHONE_TO_SCT) {
            Storage.saveInboxMessage(rxPacket.senderID, rxPacket.payload);
            radio.sendACK(rxPacket.msgID, rxPacket.senderID);
        }
    }

    // This is the ONLY place that consumes BLE input. main.cpp must not read this queue.
    char bleBuffer[140] = {};
    if (BLERadio::xQueueBLEIncoming != nullptr &&
        xQueueReceive(BLERadio::xQueueBLEIncoming, bleBuffer, 0) == pdTRUE) {
        Serial.print("[BLE->APP] Received from phone: ");
        Serial.println(bleBuffer);

        // Keep a local copy so the message appears in this device's Inbox even when
        // no other ESP-NOW node is powered on or acknowledges the mesh transmission.
        if (Storage.saveInboxMessage(0x434C, bleBuffer)) {
            Serial.println("[BLE->INBOX] Saved phone message to local Inbox.");
        } else {
            Serial.println("[BLE->INBOX] ERROR: Could not save message to local Inbox.");
        }

#if BLE_ECHO_TEST
        // Temporary test: verify both BLE directions without needing a second mesh node.
        bleRadio.sendToPhone(bleBuffer);
#else
        // 0x00FF is the broadcast node ID accepted by MeshRadio::onDataRecv().
        const bool sent = sendMessage(0x00FF, bleBuffer, MODE_1_PHONE_TO_PHONE);
        Serial.println(sent
            ? "[BLE->MESH] Message sent and ACK received."
            : "[BLE->MESH] No ACK. Check that another SmartMesh node is powered on and on the same channel.");
#endif
    }
}

void SmartMeshApp::handleKey(char key) {
    uiManager.handleInput(key);
}

bool SmartMeshApp::sendComposedMessage(const char* text) {
    if (text == nullptr || text[0] == '\0') {
        Serial.println("[KEYPAD->BLE] Not sent: message is empty.");
        return false;
    }

    // Store the outgoing message locally for the Sent screen.
    Storage.saveSentMessage(0x434C, text);

    if (!bleRadio.isConnected()) {
        Serial.println("[KEYPAD->BLE] Not sent: phone is not connected over BLE.");
        return false;
    }

    bleRadio.sendToPhone(text);
    Serial.println("[KEYPAD->BLE] Composed message sent to connected phone.");
    return true;
}

bool SmartMeshApp::sendMessage(uint16_t targetNodeId, const char* text, uint8_t mode) {
    if (text == nullptr) {
        return false;
    }

    SmartMeshPacket packet = {};
    packet.mode = mode;
    packet.packetType = PACKET_TYPE_TEXT;
    packet.msgID = static_cast<uint16_t>(millis() & 0xFFFF);
    packet.senderID = Storage.getSettings().nodeId;
    packet.receiverID = targetNodeId;
    strncpy(packet.payload, text, MAX_PAYLOAD_LEN - 1);
    packet.payload[MAX_PAYLOAD_LEN - 1] = '\0';

    Storage.saveSentMessage(targetNodeId, text);

    if (Storage.getSettings().loopbackMode) {
        Storage.saveInboxMessage(packet.senderID, text);
        return true;
    }

    return radio.sendWithRetry(packet, 3);
}
