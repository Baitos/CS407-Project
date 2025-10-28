#include "../serverHeaders/createTiles.h"
#include "../serverHeaders/drawLevel.h"
#include "../serverHeaders/updateMenu.h"
#include "../serverHeaders/updateLevel.h"
#include "../serverHeaders/state.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/drawMenu.h"

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
            printf("TITLE");
            break;
        }
        case SETTINGS:
        {
            break;
        }
        case HOST:
        {
            break;
        }
        case JOIN:
        {
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
            newState->init = createTiles;    
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
