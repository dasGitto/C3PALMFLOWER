#include "SyncManager.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "ButtonHandler.h"
#include "Settings.h"
#include <vector>

#define CON_ATTEMPTS_TIMEOUT 30000

WiFiUDP udp;

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
    udp.begin(UDP_PORT);
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
    udp.beginPacket(BROADCAST_IP, UDP_PORT);
    udp.write((uint8_t*)message.c_str(), message.length());
    udp.endPacket();
}

void sendModeUpdate(int mode) {
    if (currentSyncState != SYNC_SEARCH) return;

    String message = "MODE:" + String(mode);
    udp.beginPacket(BROADCAST_IP, UDP_PORT);
    udp.write((uint8_t*)message.c_str(), message.length());
    udp.endPacket();
    Serial.println("Broadcasting Mode: " + String(mode));

    currentSyncState = SYNC_NETWORK; // Lock sync network after first mode change
}

SyncAction readNetworkInput() {
    SyncAction action = NO_ACTION;

    if (currentSyncState == NO_SYNC) return action;

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
            action = SET_MODE;
        }
    }

    return NO_ACTION;
}
