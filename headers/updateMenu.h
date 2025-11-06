#pragma once

#include <SDL3/SDL.h>
struct SDLState;
struct GameData;
struct Resources;
// Title

void titleUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) ;
void handleMousePointerTitle(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void titleInput(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleTitleClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime);

// Settings
void settingsUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleMousePointerSettings(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void settingsInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleSettingsClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime);

// CharSelect

void charSelectUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleMousePointerCharSelect(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void charSelectInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleCharSelectKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent event, bool keyDown, float deltaTime);
void handleCharSelectClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) ;

// HostLobby
void hostLobbyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleMousePointerHostLobby(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void hostLobbyInput(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleHostLobbyClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime);

// JoinLobby
void joinLobbyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleMousePointerJoinLobby(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void joinLobbyInput(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleJoinLobbyClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
                    
// Gameplay Settings

void gameplaySettingsUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) ;
void handleMousePointerGameplaySettings(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void gameplaySettingsInput(SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleGameplaySettingsClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
