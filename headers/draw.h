#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <SDL3_image/SDL_image.h>

struct SDLState;
struct GameState;
struct Resources;
struct GameObject;
class Player;
class Level;

void drawObject(const SDLState &state, GameState &gs, GameObject &obj, float width, float height, float deltaTime);
void drawPlayer(const SDLState &state, GameState &gs, Player &player, float width, float height, float deltaTime);
void drawLevel(const SDLState &state, GameState &gs, Level &level, float width, float height, float deltaTime);