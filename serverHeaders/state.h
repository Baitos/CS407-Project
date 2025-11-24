#pragma once
#include <stdio.h>
#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include "../serverHeaders/updateLevel.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/playerState.h"



enum gameStates {
    TITLE,
    SETTINGS,
    HOST,
    JOIN,
    CHAR_SELECT,
    SPACESHIP,
    GRASSLANDS,
    SNOW,
    RESULTS,
    CREDITS,
    GAMEPLAY_SETTINGS
};

class GameState{
    public:
        int prevStateVal;
        int currStateVal;
        int nextStateVal;
        GameState * prevState;
        GameState * nextState; //May not need
        void (*input)(SDLState &state, GameData &gd, float deltaTime);
        void (*update)(const SDLState &state, GameData &gd, float deltaTime, int keyID, int keyDown, int playerID);
        void (*init)(const SDLState &state, GameData &gd);
       
        void unloadGameState(GameData &gd);
};

class TitleState : public GameState{

};

class SettingsState : public GameState{
    public:
    std::vector<std::string> controlStrings = std::vector<std::string>(7);
    float xStart = 600;
    float yStart = 115;
    float yOffset = 26;
};

class HostState : public GameState {

};

class JoinState : public GameState {

};

class CharSelectState : public GameState{
    public:
        characterType character = SWORD;
};

class LevelState : public GameState{
    public:
        characterType character = SHOTGUN;
};

/*class SpaceshipState : public LevelState {
     void (*createTiles)();
};

Add a copy for each additional stage
class LevelX : public LevelState {
     void (*createTiles)();
};
*/

class ResultsState : public GameState {

};

class CreditsState : public GameState {

};

class GameplaySettingsState : public GameState{

};


GameState * changeState(GameState * tempState, GameData &gd);
