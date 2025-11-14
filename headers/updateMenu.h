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

void gameplaySettingsUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) ;
void handleMousePointerGameplaySettings(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void gameplaySettingsInput(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleGameplaySettingsClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime);

void titleUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) ;
void handleMousePointerTitle(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void titleInput(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleTitleClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime);

void handleMousePointerResults(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleMousePointerEndResults(const SDLState &state, GameData &gd, Resources &res, float deltaTime);