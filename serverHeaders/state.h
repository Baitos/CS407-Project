#pragma once
#include <stdio.h>
#include <SDL3/SDL.h>
//include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>

#include "../serverHeaders/updateMenu.h"
#include "../serverHeaders/updateLevel.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/resources.h"
#include "../serverHeaders/playerState.h"



enum gameStates {
    TITLE,
    SETTINGS,
    HOST,
    JOIN,
    CHAR_SELECT,
    SPACESHIP,
    RESULTS,
    CREDITS
};

class GameState{
    public:
        int prevStateVal;
        int currStateVal;
        int nextStateVal;
        GameState * prevState;
        GameState * nextState; //May not need
        void (*input)(SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void (*update)(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void (*render)(const SDLState &state, GameData &gd, Resources res, float deltaTime);
        void (*init)(const SDLState &state, GameData &gd, const Resources &res);
       
        void unloadGameState(GameData &gd){
            gd.mapTiles_.clear();
            //gd.bgTiles_.clear();
            //gd.previews_.clear();
            gd.lasers_.clear();
            gd.portals_.clear();
            //TO-DO Add clearing players
        }


};

class TitleState : public GameState{

};

class SettingsState : public GameState{

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
        characterType character = SWORD;
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


GameState * changeState(GameState * tempState, GameData &gd);
