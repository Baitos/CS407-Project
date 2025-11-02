#include "../headers/createTiles.h"
#include "../headers/drawLevel.h"
#include "../headers/updateMenu.h"
#include "../headers/updateLevel.h"
#include "../headers/state.h"
#include "../headers/collision.h"
#include "../headers/drawMenu.h"

//Function called to change the state
//NOTE: When a state is changed, call currState->init() immediately after

//Init functions should be located in createTiles.cpp
//Input and Update functions should be located in updateLevel/Menu.cpp
//Render functions should be located in drawLevel/Menu.cpp

GameState * changeState(GameState * tempState, GameData &gd){
    GameState * newState;
    //Initialize nextState
    switch (tempState->nextStateVal){
        case TITLE:
        {
            newState = new TitleState();
            newState->init = initTitle;
            newState->update = titleUpdate;
            newState->render = drawTitle;
            newState->input = titleInput;
            break;
        }
        case SETTINGS:
        {
            newState = new SettingsState();
            newState->init = initSettings;
            newState->update = settingsUpdate;
            newState->render = drawSettings;
            newState->input = settingsInputs;
            break;
        }
        case HOST:
        {
            //newState = new HostState();
            printf("HOST\n");
            
            break;
        }
        case JOIN:
        {
            printf("JOIN\n");
            // newState = new JoinState();
            // newState->init = initSettings;
            // newState->update = settingsUpdate;
            // newState->render = drawSettings;
            // newState->input = settingsInputs;
            break;
        }
        case CHAR_SELECT:
        {
            newState = new CharSelectState();
            newState->init = initCharSelect;
            newState->update = charSelectUpdate;
            newState->render = drawCharSelect;
            newState->input = charSelectInputs;
            break;           
        }
        case SPACESHIP:
        {
            //Creating LevelState with init of Spaceship
            newState = new LevelState();
            newState->input = levelInputs;
            newState->update = levelUpdate;
            newState->render = drawLevel;
            newState->init = createTilesSpaceship;    
            break;
        }
        case GRASSLANDS:
        {
            //Creating LevelState with init of Spaceship
            newState = new LevelState();
            newState->input = levelInputs;
            newState->update = levelUpdate;
            newState->render = drawLevel;
            newState->init = createTilesGrassland;    
            break;
        }
        case GAMEPLAY_SETTINGS:
        {
             //Creating LevelState with init of Spaceship
            newState = new GameplaySettingsState();
            newState->input = gameplaySettingsInput;
            newState->update = gameplaySettingsUpdate;
            newState->render = drawGameplaySettings;
            newState->init = initGameplaySettings;    
            break;
        }

        case RESULTS:
        {
            break;
        }
    }
    tempState->unloadGameState(gd);
    newState->prevStateVal = tempState->currStateVal;
    newState->currStateVal = tempState->nextStateVal;
    newState->prevState = tempState;

    //To be removed when new screens are added
    newState->nextStateVal = SPACESHIP;
    return newState;
}

template<typename T>
void freeInVector(std::vector<T*>& vec) { // i love manual memory management.
    for (size_t i = 0; i < vec.size(); ++i) {
        delete vec[i]; 
        vec[i] = nullptr; 
    }
}

void GameState::unloadGameState(GameData &gd) {
    freeInVector(gd.mapTiles_);
    gd.mapTiles_.clear();

    freeInVector(gd.bgTiles_);
    gd.bgTiles_.clear();

    freeInVector(gd.lasers_);
    gd.lasers_.clear();

    freeInVector(gd.portals_);
    gd.portals_.clear();

    freeInVector(gd.lava_);
    gd.lava_.clear();

    freeInVector(gd.water_);
    gd.water_.clear();

    freeInVector(gd.signs_);
    gd.signs_.clear();

    gd.grid_.clear();

    gd.checkpoints_.clear();
    gd.md.previews_.clear();
    gd.md.arrows_.clear();
    //TTF_CloseFont(gd.font);
    //TO-DO Add clearing players
    // ^ eventually we will but to be honest, being able to add multiple players is very useful lol
}
