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

void checkCollision(const SDLState &state, GameState &gs, const Resources &res, GameObject &a, GameObject &b, float deltaTime);
void collisionResponse(const SDLState &state, GameState &gs, const Resources &res, 
                       const SDL_FRect &rectA, const SDL_FRect &rectB, 
                       const SDL_FRect &rectC, GameObject &a, GameObject &b, float deltaTime);
void groundedCheck(const SDLState &state, GameState &gs, const Resources &res, GameObject &a, GameObject &b, float deltaTime);