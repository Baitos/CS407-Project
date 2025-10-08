#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <SDL3_image/SDL_image.h>
#include "../ext/glm/glm.hpp"

struct SDLState;
struct GameData;
struct Resources;
struct GameObject;
struct Player;

bool intersectAABB(const SDL_FRect &a, const SDL_FRect &b, glm::vec2 &overlap);
//void checkCollision(const SDLState &state, GameData &gd, Resources &res, Object &a, Object &b, float deltaTime);
void collisionCheckAndResponse(const SDLState &state, GameData &gd, Resources &res,
 	Player &a, float deltaTime);