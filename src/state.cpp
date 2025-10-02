/*#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>

#include "../headers/*.h"
#include "../headers/createTiles.h"
#include "../headers/draw.h"
#include "../headers/update.h"
#include "../headers/state.h"
#include "../headers/collision.h"

GameState * changeState(GameState * currState){
    GameState * newState;
    //Initialize nextState
    switch (currState->nextStateVal){
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
            newState->input = handleKeyInput;
            newState->update = update;
            newState->render = drawObject;
            ((LevelState *)newState)->init = createTiles;
            ((LevelState *)newState)->checkCollision = checkCollision;
            ((LevelState *)newState)->collisionResponse = collisionResponse;
            ((LevelState *)newState)->aabbIntersection = intersectAABB;
            ((LevelState *)newState)->isOnScreen = isOnscreen;
            ((LevelState *)newState)->changeVelocity = changeVel;
            
            break;
        }
        case RESULTS:
        {

        }
    }
    newState->prevStateVal = currState->currStateVal;
    newState->currStateVal = currState->nextStateVal;
    newState->prevState = currState;

    return newState;
}*/