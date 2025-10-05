#include "../headers/createTiles.h"
#include "../headers/draw.h"
#include "../headers/update.h"
#include "../headers/state.h"
#include "../headers/collision.h"

//Function called to change the state
//NOTE: When a state is changed, call currState->init() immediately after

//Init functions should be located in createTiles.cpp
//Input and Update functions should be located in update.cpp
//Render functions should be located in draw.cpp

GameState * changeState(GameState * tempState){
    GameState * newState;
    //Initialize nextState
    switch (tempState->nextStateVal){
        case TITLE:
        {

        }
        case SETTINGS:
        {

        }
        case HOST:
        {

        }
        case JOIN:
        {

        }
        case CHAR_SELECT:
        {
            
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

        }
    }
    newState->prevStateVal = tempState->currStateVal;
    newState->currStateVal = tempState->nextStateVal;
    newState->prevState = tempState;
    return newState;
}