#pragma once

struct SDLState;
struct GameData;
struct Resources;

void createTilesSpaceship(const SDLState &state, GameData &gd, const Resources &res);
void createTilesGrassland(const SDLState &state, GameData &gd, const Resources &res);
void initCharSelect(const SDLState &state, GameData &gd, const Resources &res);
void initSettings(const SDLState &state, GameData &gd, const Resources &res);
void initGameplaySettings(const SDLState &state, GameData &gd, const Resources &res);
void initTitle(const SDLState &state, GameData &gd, const Resources &res);