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




void drawObject(const SDLState &state, GameState &gs, GameObject &obj, float width, float height, float deltaTime);
void drawPlayer(const SDLState &state, GameState &gs, Player player, float width, float height, float deltaTime);