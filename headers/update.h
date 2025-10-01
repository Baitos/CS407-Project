#pragma once

#include <SDL3/SDL.h>
struct SDLState;
struct GameState;
struct Resources;
struct GameObject;

/*void update(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime);
float updatePlayer(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateBullet(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateEnemy(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateObstacle(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection);*/
void handleKeyInput(const SDLState &state, GameState &gs, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime); 
void handleCrosshair(const SDLState &state, GameState &gs, Resources &res, float deltaTime);