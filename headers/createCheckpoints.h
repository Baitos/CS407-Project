#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

struct SDLState;
struct GameData;
struct Resources;

struct Checkpoint {
    SDL_FRect collider;
    int index;
};

extern std::vector<Checkpoint> courseCheckpoints;

void createCheckpointsGrassland(const SDLState &state, GameData &gd, const Resources &res);

void createCheckpointsSpaceship(const SDLState &state, GameData &gd, const Resources &res);

void createCheckpointsSnow(const SDLState &state, GameData &gd, const Resources &res);


void renderCheckpoints(SDL_Renderer* renderer, const std::vector<Checkpoint>& checkpoints, GameData &gd);