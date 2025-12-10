#include "../headers/globals.h"
int TILE_SIZE = 32;
int HOOK_SIZE = 16;
int MINIMAP_DOT_SIZE = 3;
bool running = true;
float portalID = 0;
int selectedItem = 0;
bool waitingForKey = false;
bool hasNewKey = false;
float masterVolume = 1.0f;
float musicVolume = 1.0f;
float sfxVolume = 1.0f;
const float EPSILON = 0.01f;
Uint64 lastLobbyQuery = 0;
const Uint64 UPDATE_INTERVAL_MS = 3000; // Update lobbies every 3 seconds
std::string username = "";
