#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

enum SyncState {
    NO_SYNC,
    SYNC_SEARCH,        // discover devices to sync with
    SYNC_NETWORK        // device network established
};

extern SyncState currentSyncState;

void startSyncManager();
void toggleSyncMode();
void handleSyncMessages();
void sendModeUpdate(int mode);
void discoverSyncPeers();

#endif