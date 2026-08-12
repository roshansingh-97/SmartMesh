#include <Arduino.h>
#include "Config.h"
#include "Radio/MeshRadio.h"
#include "Input/KeypadHandler.h"
#include "UI/DisplayManager.h"
#include "Power/BatteryMonitor.h"
#include "Storage/Storage.h"

MeshRadio radio;
KeypadHandler keypad;
DisplayManager display;

SystemState currentState = STATE_MENU;
uint8_t menuSelection = 0;
uint8_t inboxIndex = 0; // Current message index being viewed

void handleIncomingPackets() {
    SmartMeshPacket packet;

    if (xQueueReceive(MeshRadio::xQueueIncoming, &packet, 0) == pdTRUE) {
        if (packet.receiverID == NODE_ID && packet.packetType != PACKET_TYPE_ACK) {
            radio.sendACK(packet.msgID, packet.senderID);
        }

        if (packet.packetType == PACKET_TYPE_TEXT) {
            // Auto-save incoming message to persistent NVS storage
            Storage.saveMessage(packet);
            
            // Switch view to display newest received message
            inboxIndex = (Storage.getSavedCount() > 0) ? Storage.getSavedCount() - 1 : 0;
            currentState = STATE_INBOX;
        }
    }
}

void setup() {
    Serial.begin(115200);

    Storage.begin();
    Battery.begin();
    display.begin();
    keypad.begin();
    radio.begin();

    display.renderMenu(menuSelection);
}

void loop() {
    keypad.update();
    handleIncomingPackets();

    char key = keypad.scanKeypad();

    switch (currentState) {
        case STATE_MENU:
            if (key == '2' || key == 'B') {
                menuSelection = (menuSelection + 1) % 4;
                display.renderMenu(menuSelection);
            } 
            else if (key == '8' || key == 'A') {
                menuSelection = (menuSelection == 0) ? 3 : menuSelection - 1;
                display.renderMenu(menuSelection);
            } 
            else if (key == '#' || key == 'D') {
                if (menuSelection == 0) {
                    keypad.clearMessage();
                    currentState = STATE_COMPOSE;
                    display.renderComposeScreen(keypad.getMessage());
                } else if (menuSelection == 1) {
                    currentState = STATE_INBOX;
                    uint8_t count = Storage.getSavedCount();
                    if (count > 0) {
                        inboxIndex = count - 1; // View most recent
                        SmartMeshPacket p;
                        if (Storage.readMessage(inboxIndex, p)) {
                            display.renderInboxScreen(p.senderID, p.payload);
                        }
                    } else {
                        display.renderInboxScreen(0, "No saved messages.");
                    }
                }
            }
            break;

        case STATE_COMPOSE:
            display.renderComposeScreen(keypad.getMessage());

            if (key == 'B') {
                currentState = STATE_MENU;
                display.renderMenu(menuSelection);
            } 
            else if (key == '#') {
                String msg = keypad.getMessage();
                if (msg.length() > 0) {
                    display.renderStatusScreen("Transmitting...", "Waiting for ACK");

                    SmartMeshPacket packet;
                    packet.senderID = NODE_ID;
                    packet.receiverID = 0xFF;
                    packet.msgID = millis() & 0xFFFF;
                    packet.packetType = PACKET_TYPE_TEXT;
                    strncpy(packet.payload, msg.c_str(), sizeof(packet.payload));

                    if (radio.sendWithRetry(packet)) {
                        Storage.saveMessage(packet); // Persist sent message
                        display.renderStatusScreen("Status: Sent!", "ACK Received");
                    } else {
                        display.renderStatusScreen("Status: Failed", "No ACK Received");
                    }

                    vTaskDelay(pdMS_TO_TICKS(1200));
                    keypad.clearMessage();
                    currentState = STATE_MENU;
                    display.renderMenu(menuSelection);
                }
            }
            break;

        case STATE_INBOX:
            // Scroll through inbox history using 2 (Down/Older) and 8 (Up/Newer)
            if (key == '2' || key == 'B') { 
                if (inboxIndex > 0) {
                    inboxIndex--;
                    SmartMeshPacket p;
                    if (Storage.readMessage(inboxIndex, p)) {
                        display.renderInboxScreen(p.senderID, p.payload);
                    }
                }
            } 
            else if (key == '8' || key == 'A') {
                if (inboxIndex + 1 < Storage.getSavedCount()) {
                    inboxIndex++;
                    SmartMeshPacket p;
                    if (Storage.readMessage(inboxIndex, p)) {
                        display.renderInboxScreen(p.senderID, p.payload);
                    }
                }
            } 
            else if (key == '*') { // Exit Inbox
                currentState = STATE_MENU;
                display.renderMenu(menuSelection);
            }
            break;
    }

    vTaskDelay(pdMS_TO_TICKS(15));
}