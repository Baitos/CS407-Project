#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/gameobject.h"
#include "../headers/init.h"
#include "../headers/createTiles.h"
#include "../headers/draw.h"
#include "../headers/update.h"

using namespace std;

bool running = true;

int main(int argc, char** argv) { // SDL needs to hijack main to do stuff; include argv/argc
    SDLState state;
    state.width = 1600;
    state.height = 900;
    state.logW = 640;
    state.logH = 480;

    // go to result when you die, should probably change!!!!
    //main_loop: absolutely do not use this holy shit my computer almost crashed. fork bomb!
    bool l = false;
    if (argc > 1 && !strcmp(argv[1], "l")) {
        l = true;
    }
    if (!initialize(state)) {
        return 1;
    }
    // load game assets
    Resources res;
    res.load(state, l);

    // setup game data
    GameState gs(state);
    createTiles(state, gs, res);
    uint64_t prevTime = SDL_GetTicks();

    // start game loop
    while (running) {
        uint64_t nowTime = SDL_GetTicks(); // take time from previous frame to calculate delta
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
                    handleKeyInput(state, gs, gs.player(), event.key.scancode, true);
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    handleKeyInput(state, gs, gs.player(), event.key.scancode, false);
                    if (event.key.scancode == SDL_SCANCODE_F12) {
                        gs.debugMode = !gs.debugMode;
                    }
                    if (event.key.scancode == SDL_SCANCODE_F11) {
                        gs.player().pos = gs.EntrancePortal;
                         gs.player().pos.x -= 32;
                    }
                    break;
                }
            }
        }

        // update objs
        for (auto &layer : gs.layers) {
            for (GameObject &obj : layer) {
                update(state, gs, res, obj, deltaTime);
            }
        }

        // update lasers
        for (GameObject &laser : gs.lasers) {
            update(state, gs, res, laser, deltaTime);
        }

        // update bullets
        for (GameObject &bullet : gs.bullets) {
            update(state, gs, res, bullet, deltaTime);
        }
        // used for camera system
        gs.mapViewport.x = (gs.player().pos.x + TILE_SIZE / 2) - (gs.mapViewport.w / 2); 
        gs.mapViewport.y = (gs.player().pos.y + TILE_SIZE / 2) - (gs.mapViewport.h / 2); 
        //draw stuff
        SDL_SetRenderDrawColor(state.renderer, 37, 37, 84, 255);
        SDL_RenderClear(state.renderer);

        // draw background
        /*SDL_FRect src = {
            gs.mapViewport.x,  // camera x
            gs.mapViewport.y,  // camera y
            gs.mapViewport.w,  // camera width (usually = screen width)
            gs.mapViewport.h   // camera height (usually = screen height)
        };
        SDL_FRect dst = {
            0, 0,              // always draw at (0,0) on the screen
            gs.mapViewport.w,  // draw to the screen width
            gs.mapViewport.h   // draw to the screen height
        };*/

        //SDL_RenderTexture(state.renderer, res.texBg1, &src, &dst);
        //drawParallaxBackground(state.renderer, res.texBg1, gs.player().vel.x, gs.bg4Scroll, 0.0375f, deltaTime);
        //drawParallaxBackground(state.renderer, res.texBg4, gs.player().vel.x, gs.bg4Scroll, 0.075f, deltaTime);
        //drawParallaxBackground(state.renderer, res.texBg3, gs.player().vel.x, gs.bg3Scroll, 0.15f, deltaTime);
        //drawParallaxBackground(state.renderer, res.texBg2, gs.player().vel.x, gs.bg2Scroll, 0.3f, deltaTime);

        // draw bg tiles
        for (GameObject &obj : gs.bgTiles) {
            SDL_FRect dst {
                .x = obj.pos.x - gs.mapViewport.x,
                .y = obj.pos.y- gs.mapViewport.y,
                .w = static_cast<float>(obj.texture->w),
                .h = static_cast<float>(obj.texture->h)
            };
            SDL_RenderTexture(state.renderer, obj.texture, nullptr, &dst);
        }

        // draw objs
        for (auto &layer : gs.layers) {
            for (GameObject &obj : layer) {
                if (obj.data.level.state == LevelState::portal && obj.type == ObjectType::level){
                    drawObject(state, gs, obj, 32, 64, deltaTime); 
                } else{
                    drawObject(state, gs, obj, TILE_SIZE, TILE_SIZE, deltaTime); 
                }            
            }
        }

        // Draw Lasers
        for(GameObject &laser : gs.lasers){
            if(laser.data.obstacle.laserActive){
                drawObject(state, gs, laser, TILE_SIZE, TILE_SIZE, deltaTime);
            }
        }

        // draw bullets
        for (GameObject &bullet : gs.bullets) {
            if (bullet.data.bullet.state != BulletState::inactive) {
                drawObject(state, gs, bullet, bullet.collider.w, bullet.collider.h, deltaTime);
            }
        }

        // draw fg tiles
        for (GameObject &obj : gs.fgTiles) {
            SDL_FRect dst {
                .x = obj.pos.x - gs.mapViewport.x,
                .y = obj.pos.y - gs.mapViewport.y,
                .w = static_cast<float>(obj.texture->w), 
                .h = static_cast<float>(obj.texture->h)
            };
            SDL_RenderTexture(state.renderer, obj.texture, nullptr, &dst);
        }

        if (gs.debugMode) {
        // debug info
            SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
            SDL_RenderDebugText(state.renderer, 5, 5,
                            std::format("State: {}, Bullet: {}, Grounded: {}", 
                            static_cast<int>(gs.player().data.player.state), gs.bullets.size(), gs.player().grounded).c_str());
        }
        //swap buffers and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
        /*if (dead) {
            goto main_loop;
            dead = false;
        }*/
    }

    res.unload();
    cleanup(state);
    return 0;
}