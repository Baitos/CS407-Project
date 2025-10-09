#include "../headers/createTiles.h"
#include "../headers/drawLevel.h"
#include "../headers/updateMenu.h"
#include "../headers/updateLevel.h"
#include "../headers/state.h"
#include "../headers/collision.h"

//Function called to change the state
//NOTE: When a state is changed, call currState->init() immediately after

//Init functions should be located in createTiles.cpp
//Input and Update functions should be located in update.cpp
//Render functions should be located in draw.cpp

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
