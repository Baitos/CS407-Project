#pragma once

#include <SDL3/SDL.h>
struct SDLState;
struct GameData;
struct Resources;
class Player;

void levelUpdate(const SDLState &state, GameData &gd, float deltaTime);
void handleKeyInput(const SDLState &state, GameData &gd,
                    SDL_Event event, bool keyDown, float deltaTime); 
void handleCrosshair(const SDLState &state, GameData &gd, float deltaTime);
void levelInputs(SDLState &state, GameData &gd, float deltaTime);
void handleLevelClick(SDLState &state, GameData &gd, Player &p, float deltaTime, SDL_Event event, bool buttonDown);
void placement(const SDLState &state, GameData &gd, float deltaTime);