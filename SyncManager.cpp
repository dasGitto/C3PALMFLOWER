#include "SyncManager.h"
#include "Effects.h"  // Needed to update LED modes
#include <WiFi.h>
#include <WiFiUdp.h>
#include "ButtonHandler.h"

#define CON_ATTEMPTS_TIMEOUT 30000

WiFiUDP udp;
const int udpPort = 12345;
const char* broadcastIP = "255.255.255.255";  // Send to all devices on network

SyncState currentSyncState = NO_SYNC;
std::vector<String> syncPeers;
String deviceID;

void startSyncManager() {
    Serial.println("Initializing Sync Manager...");

    // Ensure Wi-Fi is connected before doing anything sync-related
    // TODO: change placeholder credentials
    WiFi.begin("YourSSID", "YourPassword");
    unsigned long con_start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - con_start < CON_ATTEMPTS_TIMEOUT) {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi Connected!");
    } else {
        Serial.println("\nWi-Fi Connection Unsuccessful");
    }

    deviceID = WiFi.macAddress();
    udp.begin(udpPort);
}

void toggleSyncMode() {
    if (currentSyncState == NO_SYNC) {
        currentSyncState = SYNC_SEARCH;
        syncPeers.clear();
        Serial.println("Sync Mode Enabled - Searching for peers...");
        discoverSyncPeers();
    } else {
        // TODO: maybe remove? We might not want this behaviour
        currentSyncState = NO_SYNC;
        Serial.println("Sync Mode Disabled");
    }
}

void discoverSyncPeers() {
    if (currentSyncState != SYNC_SEARCH) return;
    String message = "DISCOVER:" + deviceID;
    udp.beginPacket(broadcastIP, udpPort);
    udp.write((uint8_t*)message.c_str(), message.length());
    udp.endPacket();
}

void sendModeUpdate(int mode) {
    if (currentSyncState != SYNC_SEARCH) return;

    String message = "MODE:" + String(mode);
    udp.beginPacket(broadcastIP, udpPort);
    udp.write((uint8_t*)message.c_str(), message.length());
    udp.endPacket();
    Serial.println("Broadcasting Mode: " + String(mode));

    currentSyncState = SYNC_NETWORK; // Lock sync network after first mode change
}

void handleSyncMessages() {
    if (currentSyncState == NO_SYNC) return;

    int packetSize = udp.parsePacket();
    if (packetSize) {
        char incomingPacket[255];
        udp.read(incomingPacket, 255);
        String receivedMsg = String(incomingPacket);

        if (currentSyncState == SYNC_SEARCH && receivedMsg.startsWith("DISCOVER:")) {
            String senderID = receivedMsg.substring(9);
            if (senderID != deviceID && std::find(syncPeers.begin(), syncPeers.end(), senderID) == syncPeers.end()) {
                syncPeers.push_back(senderID);
                Serial.println("Discovered Sync Peer: " + senderID);
            }
        }
        else if (currentSyncState == SYNC_NETWORK && receivedMsg.startsWith("MODE:")) {
            int newMode = receivedMsg.substring(5).toInt();
            setCurrentMode(newMode);
            updateLEDStrip();
        }
    }
}
