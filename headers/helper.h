#pragma once

#include "../ext/glm/glm.hpp"

struct SDLState;
struct GameState;
struct Resources;
struct GameObject;

bool isOnscreen(const SDLState &state, GameState &gs, GameObject &obj);
float changeVel(float vel, GameObject &obj);
bool isSliding(GameObject &obj);
glm::vec2 findCenterOfSprite(GameObject &obj);