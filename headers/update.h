#pragma once

#include <stdio.h>
#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "init.h"
#include "gameobject.h"

void update(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime);
float updatePlayer(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateBullet(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateEnemy(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateObstacle(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
void handleKeyInput(const SDLState &state, GameState &gs, Resources &res, GameObject &obj,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime);