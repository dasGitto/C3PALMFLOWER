#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

enum SyncState {
    NO_SYNC,
    SYNC_SEARCH,        // discover devices to sync with
    SYNC_NETWORK        // device network established
};

enum SyncAction { NO_ACTION, SET_MODE };

extern SyncState currentSyncState;

void startSyncManager();
void toggleSyncMode();
SyncAction readNetworkInput();
void sendModeUpdate(int mode);
void discoverSyncPeers();

#endif