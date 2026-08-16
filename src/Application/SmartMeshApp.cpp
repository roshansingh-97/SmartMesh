#include "SmartMeshApp.h"
#include "../Storage/Storage.h"

SmartMeshApp::SmartMeshApp() {}

void SmartMeshApp::begin() {
    Storage.begin();
    displayManager.begin();
    uiManager.begin();

    // 1. Initialize ESP-NOW Mesh Radio
    radio.begin();

    // 2. Initialize BLE Radio for Smartphone pairing
    bleRadio.begin("SmartMesh Terminal");

    // 3. Initialize UI screens
    homeScreen.initialize();
    composeScreen.initialize();
    inboxScreen.initialize();
    sentScreen.initialize();
    settingsScreen.initialize();
    aboutScreen.initialize();
    contactsScreen.initialize();

    // 4. Register screens
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
    // 1. Render UI frames
    uiManager.update();

    // 2. PROCESS ESP-NOW INCOMING PACKETS (Received from another ESP32)
    SmartMeshPacket rxPacket;
    if (xQueueReceive(MeshRadio::xQueueIncoming, &rxPacket, 0) == pdTRUE) {
        
        // --- MODE 1 & MODE 4: Destination is Phone B over BLE ---
        if (rxPacket.mode == MODE_1_PHONE_TO_PHONE || rxPacket.mode == MODE_4_SCT_TO_PHONE) {
            if (bleRadio.isConnected()) {
                bleRadio.sendToPhone(rxPacket.payload); // Relay to Smartphone
            } else {
                // Fallback: Save to Inbox if phone is disconnected
                Storage.saveInboxMessage(rxPacket.senderID, rxPacket.payload);
            }
            radio.sendACK(rxPacket.msgID, rxPacket.senderID);
        }
        
        // --- MODE 2 & MODE 3: Destination is Local ESP32 Screen/Storage ---
        else if (rxPacket.mode == MODE_2_SCT_TO_SCT || rxPacket.mode == MODE_3_PHONE_TO_SCT) {
            Storage.saveInboxMessage(rxPacket.senderID, rxPacket.payload);
            radio.sendACK(rxPacket.msgID, rxPacket.senderID);
        }
    }

    // 3. PROCESS INCOMING BLE MESSAGES (Received from Smartphone A)
    char bleBuffer[140];
    if (xQueueReceive(BLERadio::xQueueBLEIncoming, bleBuffer, 0) == pdTRUE) {
        // Automatically transmit message over ESP-NOW mesh (Mode 1: Phone to Phone)
        sendMessage(0xFFFF, bleBuffer, MODE_1_PHONE_TO_PHONE);
    }
}

void SmartMeshApp::handleKey(char key) {
    uiManager.handleInput(key);
}

bool SmartMeshApp::sendMessage(uint16_t targetNodeId, const char* text, uint8_t mode) {
    SmartMeshPacket packet;
    packet.mode = mode;
    packet.packetType = PACKET_TYPE_TEXT;
    packet.msgID = millis() & 0xFFFF;
    packet.senderID = Storage.getSettings().nodeId;
    packet.receiverID = targetNodeId;
    
    strncpy(packet.payload, text, MAX_PAYLOAD_LEN - 1);
    packet.payload[MAX_PAYLOAD_LEN - 1] = '\0';

    // 1. Save local copy to Sent Screen
    Storage.saveSentMessage(targetNodeId, text);

    // 2. Loopback Check
    if (Storage.getSettings().loopbackMode) {
        Storage.saveInboxMessage(packet.senderID, text);
        return true;
    }

    // 3. Transmit over ESP-NOW Mesh with ACK retries
    return radio.sendWithRetry(packet, 3);
}