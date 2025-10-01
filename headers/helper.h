#pragma once

#include "../ext/glm/glm.hpp"
#include "object.h"
#include "player.h"

struct SDLState;
struct GameState;
struct Resources;
struct GameObject;

bool isOnscreen(const SDLState &state, GameState &gs, Object &obj);
float changeVel(float vel, Player &p);
bool isSliding(Player &p);
glm::vec2 findCenterOfSprite(Object &obj);