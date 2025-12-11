#pragma once

struct SDLState;
struct GameData;
//struct Resources;
//void createPlayer(GameData &gd, glm::vec2 pos);
void createGrid(const SDLState &state, GameData &gd, int rows, int cols);
//void createItemBox(GameData &gd, glm::vec2 pos, SDL_FRect collider);

void createTilesSpaceship(const SDLState &state, GameData &gd);
void createTilesGrassland(const SDLState &state, GameData &gd);

void createTilesSnow(const SDLState &state, GameData &gd);
void createTilesDesert(const SDLState &state, GameData &gd);
void customizeRevolvers(const SDLState &state, GameData &gd);