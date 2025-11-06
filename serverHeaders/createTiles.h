#pragma once

struct SDLState;
struct GameData;
//struct Resources;

void createGrid(const SDLState &state, GameData &gd, int rows, int cols);

void createTilesSpaceship(const SDLState &state, GameData &gd);
void createTilesGrassland(const SDLState &state, GameData &gd);

