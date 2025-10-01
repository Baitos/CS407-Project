#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <SDL3_image/SDL_image.h>
#include "../ext/glm/glm.hpp"

/*struct SDLState;
struct GameData;
struct Resources;
struct GameObject;

bool intersectAABB(const SDL_FRect &a, const SDL_FRect &b, glm::vec2 &overlap);
void checkCollision(const SDLState &state, GameData &gd, Resources &res, GameObject &a, GameObject &b, float deltaTime);
void collisionResponse(const SDLState &state, GameData &gd, Resources &res,
                       const SDL_FRect &rectA, const SDL_FRect &rectB, const glm::vec2 &overlap,
                       GameObject &a, GameObject &b, float deltaTime);*/