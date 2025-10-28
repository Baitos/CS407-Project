#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
//#include <SDL3_image/SDL_image.h>

struct SDLState;
struct GameData;
struct Resources;

void drawLevel(const SDLState &state, GameData &gd, Resources res, float deltaTime);