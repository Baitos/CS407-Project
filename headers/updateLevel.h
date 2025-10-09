#pragma once

#include <SDL3/SDL.h>
struct SDLState;
struct GameData;
struct Resources;

void levelUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime); 
void handleCrosshair(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void levelInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleLevelClick(SDLState &state, GameData &gd, Resources &res, float deltaTime, SDL_Event event);