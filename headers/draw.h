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




void drawParallaxBackground(SDL_Renderer *renderer, SDL_Texture *texture, float xVelocity, float &scrollPos, float scrollFactor, float deltaTime);
void drawObject(const SDLState &state, GameData &gd, GameObject &obj, float width, float height, float deltaTime);