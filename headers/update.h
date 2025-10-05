#pragma once

#include <SDL3/SDL.h>
struct SDLState;
struct GameData;
struct Resources;

/*void update(const SDLState &state, GameData &gd, Resources &res, GameObject &obj, float deltaTime);
float updatePlayer(const SDLState &state, GameData &gd, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateBullet(const SDLState &state, GameData &gd, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateEnemy(const SDLState &state, GameData &gd, Resources &res, GameObject &obj, float deltaTime, float currentDirection);
float updateObstacle(const SDLState &state, GameData &gd, Resources &res, GameObject &obj, float deltaTime, float currentDirection);*/
void levelUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void handleKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime); 
void handleCrosshair(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void levelInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime);