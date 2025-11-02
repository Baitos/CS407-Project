#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <SDL3_image/SDL_image.h>
#include "../ext/glm/glm.hpp"

struct SDLState;
struct GameData;
struct Resources;
struct Player;
struct Object;

bool intersectAABB(const SDL_FRect &a, const SDL_FRect &b, glm::vec2 &overlap);
void playerCheckCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
void playerCollisionResponse(const SDLState &state, GameData &gd, Resources &res,
	Player &p, Object &o, SDL_FRect &rectA, SDL_FRect &rectB, glm::vec2 &resolution, float deltaTime);

void collisionCheckAndResponse(const SDLState &state, GameData &gd, Resources &res,
  	Player &player, float deltaTime);