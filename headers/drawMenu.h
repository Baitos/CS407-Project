#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <SDL3_image/SDL_image.h>

struct SDLState;
struct GameData;
struct Resources;

void drawCharSelect(const SDLState &state, GameData &gd, Resources res, float deltaTime);
void drawSettings(const SDLState &state, GameData &gd, Resources res, float deltaTime);
void drawGameplaySettings(const SDLState &state, GameData &gd, Resources res, float deltaTime);
void drawTitle(const SDLState &state, GameData &gd, Resources res, float deltaTime);
void drawJoinLobby(const SDLState &state, GameData &gd, Resources res, float deltaTime);
void drawHostLobby(const SDLState &state, GameData &gd, Resources res, float deltaTime);