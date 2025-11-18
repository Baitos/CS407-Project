#pragma once

struct SDLState;
struct GameData;
struct Resources;

void createGrid(const SDLState &state, GameData &gd, int rows, int cols);
void createMinimap(const SDLState &state, GameData &gd, const Resources &res, int map, int scale);

void createTilesSpaceship(const SDLState &state, GameData &gd, const Resources &res);
void createTilesGrassland(const SDLState &state, GameData &gd, const Resources &res);
void createTilesSnow(const SDLState &state, GameData &gd, const Resources &res);

void initCharSelect(const SDLState &state, GameData &gd, const Resources &res);
void initSettings(const SDLState &state, GameData &gd, const Resources &res);
void initGameplaySettings(const SDLState &state, GameData &gd, const Resources &res);
void initTitle(const SDLState &state, GameData &gd, const Resources &res);
void initResults(const SDLState &state, GameData &gd, const Resources &res);
void initEndResults(const SDLState &state, GameData &gd, const Resources &res);