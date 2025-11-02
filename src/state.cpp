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
    //tempState->nextStateVal = CHAR_SELECT;
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
    //newState->nextStateVal = SPACESHIP;
    return newState;
}

void GameState::unloadGameState(GameData &gd) {
    gd.mapTiles_.clear();
    gd.bgTiles_.clear();
    gd.lasers_.clear();
    gd.portals_.clear();
    gd.lava_.clear();
    gd.water_.clear();
    gd.signs_.clear();
    gd.itemBoxes_.clear();

    gd.checkpoints_.clear();
    
    gd.grid_.clear();

    gd.md.previews_.clear();
    gd.md.arrows_.clear();

    //TTF_CloseFont(gd.font);
    //TO-DO Add clearing players
}