#include "../serverHeaders/createTiles.h"
#include "../serverHeaders/updateLevel.h"
#include "../serverHeaders/state.h"
#include "../serverHeaders/collision.h"


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
           
            break;
        }
        case SETTINGS:
        {
            newState = new SettingsState();
           
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
          
            break;           
        }
        case SPACESHIP:
        {
            printf("space\n");
            //Creating LevelState with init of Spaceship
            newState = new LevelState();
            newState->input = levelInputs;
            newState->update = levelUpdate;
            newState->init = createTilesSpaceship;    
            break;
        }
        case GRASSLANDS:
        {
            //Creating LevelState with init of Spaceship
            newState = new LevelState();
            newState->input = levelInputs;
            newState->update = levelUpdate;
            newState->init = createTilesGrassland;    
            break;
        }
        case SNOW :
        {
            newState = new LevelState();
            newState->input = levelInputs;
            newState->update = levelUpdate;
            newState->init = createTilesSnow;   
            break;
        }
        case GAMEPLAY_SETTINGS:
        {
             //Creating LevelState with init of Spaceship
            newState = new GameplaySettingsState();
          
            break;
        }

        case RESULTS:
        {
            newState = new ResultsState();
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
    printf("start unload\n");
    gd.mapTiles_.clear();
    gd.bgTiles_.clear();
    gd.lasers_.clear();
    gd.portals_.clear();
    gd.lava_.clear();
    gd.water_.clear();
    gd.signs_.clear();
    gd.itemBoxes_.clear();

    gd.checkpoints_.clear();
    gd.players_.clear();
    
    gd.grid_.clear();
    printf("end unload\n");


    //TTF_CloseFont(gd.font);
    //TO-DO Add clearing players
}