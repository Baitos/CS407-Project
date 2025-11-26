#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "enet.h"
struct SDLState;
struct GameData;
struct Resources;
class Player;

void levelUpdate(const SDLState &state, GameData &gd, float deltaTime, int keyID, int keyDown, int playerID);
void handleKeyInput(const SDLState &state, GameData &gd,
                    int playerID, int keyID, int keyDown, float deltaTime, ENetHost* lobbyServer, std::vector<ENetPeer *> clients); 
void handleCrosshair(const SDLState &state, GameData &gd, float deltaTime);
void levelInputs(SDLState &state, GameData &gd, float deltaTime);
void handleLevelClick(SDLState &state, GameData &gd, int playerID, float deltaTime, int key, bool buttonDown, float aH, float oH,  ENetHost* lobbyServer, std::vector<ENetPeer *> clients);
void placement(const SDLState &state, GameData &gd, float deltaTime);