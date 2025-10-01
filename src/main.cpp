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
#include "../headers/update.h"
#include "../headers/helper.h"
#include "../headers/draw.h"

using namespace std;

int main(int argc, char** argv) { // SDL needs to hijack main to do stuff; include argv/argc
    SDLState state;
    state.width = 1600;
    state.height = 900;
    state.logW = 640;
    state.logH = 480;

    if (!initialize(state)) {
        return 1;
    }
    // load game assets
    Resources res;
    res.load(state);
    

    // setup game data
    GameData gd(state);
    createTiles(state, gd, res);
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
        SDL_Event event { 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: 
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    //printf("Width = %d, Height = %d", state.width, state.height);
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    handleKeyInput(state, gd, res, event.key, true, deltaTime);
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    handleKeyInput(state, gd, res, event.key, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    //handleClick(state, gd, res, gd.player(), deltaTime);
                    break;
                }
            }
        }

        // update portals
        for (Portal &portal : gd.portals_) {
            portal.update(state, gd, res, deltaTime);
        }

        // update lasers
        for (Laser &laser : gd.lasers_) {
            laser.update(state, gd, res, deltaTime);
        }

        // used for camera system
        gd.mapViewport.x = (gd.player.pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
        gd.mapViewport.y = (gd.player.pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 
        //draw bg
        SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
        SDL_RenderClear(state.renderer);

        // draw bg tiles
        for (BackgroundObject &bg : gd.bgTiles_) {
           bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
        }

        // draw level tiles
        for(Level &level : gd.mapTiles_) {
            level.draw(state, gd, TILE_SIZE, TILE_SIZE);
        }

        // draw portal tiles
        for(Portal &portal : gd.portals_) {
            portal.draw(state, gd, TILE_SIZE, TILE_SIZE * 2);
        }

        // draw player
        gd.player.draw(state, gd, TILE_SIZE, TILE_SIZE); // draw player class


        // draw lasers
        for(Laser &laser : gd.lasers_) {
            if (laser.laserActive) {
                laser.draw(state, gd, TILE_SIZE, TILE_SIZE);
            }
        }

        if (gd.debugMode) {
        // debug info
            SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
            SDL_RenderDebugText(state.renderer, 5, 5,
                            std::format("FPS: {}, State: {}, Grounded: {}, X: {}, Y: {}", 
                            static_cast<int>(FPS), static_cast<int>(0), false, gd.mapViewport.x, gd.mapViewport.y).c_str());
        }

        // handle the crosshair
        handleCrosshair(state, gd, res, deltaTime);

        //swap buffers and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
    }

    res.unload();
    cleanup(state);
    return 0;
}