#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/player.h"
#include "../headers/globals.h"
#include "../headers/createTiles.h"
#include "../headers/updateMenu.h"
#include "../headers/updateLevel.h"
#include "../headers/helper.h"
#include "../headers/drawMenu.h"
#include "../headers/drawLevel.h"
#include "../headers/state.h"
#include "../headers/playerState.h"
#include "../headers/menu.h"

GameState * currState;
using namespace std;

int main(int argc, char** argv) { // SDL needs to hijack main to do stuff; include argv/argc
    SDLState state;
    state.width = 1600;
    state.height = 900;
    state.logW = 800;
    state.logH = 450;

    if (!initialize(state)) {
        return 1;
    }
    // load game assets
    Resources res;
    res.load(state);
    

    //Initial Game State
    //CHANGE if testing a different screen and you want it up on start

    currState = new SettingsState();
    currState->currStateVal =  SETTINGS;
    currState->nextStateVal = CHAR_SELECT;
    currState->init = initSettings;
    currState->update = settingsUpdate;
    currState->render = drawSettings;
    currState->input = settingsInputs;
    
    // currState = new CharSelectState();
    // currState->currStateVal =  CHAR_SELECT;
    // currState->nextStateVal = SPACESHIP;
    // currState->init = initCharSelect;
    // currState->update = charSelectUpdate;
    // currState->render = drawCharSelect;
    // currState->input = charSelectInputs;
    
    // currState = new LevelState();
    // currState->nextStateVal = SPACESHIP;
    // currState->init = createTiles;
    // currState->update = levelUpdate;
    // currState->render = drawLevel;
    // currState->input = levelInputs;

    // setup game data
    GameData gd(state);
    currState->init(state,gd,res);
    uint64_t prevTime = SDL_GetTicks();

    uint64_t frames = 0;
    uint64_t FPS = 0;
    uint64_t lastTime = 0;

    

    // start game loop
    while (running) {
        uint64_t nowTime = SDL_GetTicks(); // take time from previous frame to calculate delta
        frames++;
        if (nowTime > lastTime + 1000) { // fps counter
            lastTime = nowTime;
            FPS = frames;           
            frames = 0;
        }
        float deltaTime = (nowTime - prevTime) / 1000.0f; // convert to seconds from ms

        //Calls functions related to the current GameState
        //printf("new main loop");
        currState->input(state, gd, res, deltaTime);
        //printf("input done\n");
        currState->update(state, gd, res, deltaTime);
        //printf("update done\n");
        currState->render(state, gd, res, deltaTime);
        //printf("draw done\n");
        if (gd.debugMode) {
        // debug info
            SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
            SDL_RenderDebugText(state.renderer, 5, 5,
                            std::format("FPS: {}, State: {}, Grounded: {}, X: {}, Y: {}", 
                            static_cast<int>(FPS), static_cast<int>(gd.player.state_->currStateVal), gd.player.grounded, gd.mapViewport.x, gd.mapViewport.y).c_str());
        }
        //swap buffers and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
        
    }

    delete currState;
    res.unload();
    cleanup(state);
    return 0;
}