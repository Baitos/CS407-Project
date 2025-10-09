#pragma once

#include "../ext/glm/glm.hpp"
#include "object.h"
#include "player.h"

struct SDLState;
struct GameData;
struct Resources;

bool isOnscreen(const SDLState &state, GameData &gd, Object &obj);
float changeVel(float vel, Player &p);
bool isSliding(Player &p);
glm::vec2 findCenterOfSprite(Object &obj);
std::vector<float> distanceForm(GameData &gd, Object &a, Object &b); // returns xDist, yDist, dist = sqrt[(b.x-a.x)^2 + (b.y-a.y)^2], ah, oh as a vector
glm::vec2 updatePos(Object &o, float deltaTime);
