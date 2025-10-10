#pragma once

#include <SDL3/SDL.h>
struct SDLState;
struct GameData;
struct Resources;

void settingsUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void charSelectUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleMousePointerCharSelect(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleMousePointerSettings(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void charSelectInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleCharSelectKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent event, bool keyDown, float deltaTime);
void handleCharSelectClick(const SDLState &state, GameData &gd, Resources &res,
                     float deltaTime) ;
void settingsInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleSettingsClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime);