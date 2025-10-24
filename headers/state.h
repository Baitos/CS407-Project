#pragma once
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>

#include "../headers/updateMenu.h"
#include "../headers/updateLevel.h"
#include "../headers/collision.h"
#include "../headers/gameData.h"
#include "../headers/initState.h"
#include "../headers/helper.h"
#include "../headers/globals.h"
#include "../headers/resources.h"
#include "../headers/playerState.h"



enum gameStates {
    TITLE,
    SETTINGS,
    HOST,
    JOIN,
    CHAR_SELECT,
    SPACESHIP,
    GRASSLANDS,
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
        void (*input)(SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void (*update)(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void (*render)(const SDLState &state, GameData &gd, Resources res, float deltaTime);
        void (*init)(const SDLState &state, GameData &gd, const Resources &res);
       
        void unloadGameState(GameData &gd){
            gd.mapTiles_.clear();
            gd.bgTiles_.clear();
            gd.previews_.clear();
            gd.lasers_.clear();
            gd.portals_.clear();
            gd.arrows_.clear();
            gd.lava_.clear();
            gd.water_.clear();
            gd.signs_.clear();
            gd.checkpoints_.clear();
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
