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
void handleKeyInput(const SDLState &state, GameState &gs, GameObject &obj,
                    SDL_Scancode key, bool keyDown);