#pragma once

#include <string>
#include <SDL3/SDL.h>
extern int TILE_SIZE;
extern int HOOK_SIZE;
extern int MINIMAP_DOT_SIZE;
extern bool running;
extern float portalID;
extern int selectedItem;
extern const float EPSILON;
extern bool waitingForKey;
extern bool hasNewKey;
extern float masterVolume;
extern float musicVolume;
extern float sfxVolume;
extern Uint64 lastLobbyQuery;
extern const Uint64 UPDATE_INTERVAL_MS;
extern std::string username;