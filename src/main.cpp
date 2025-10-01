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

    //gd.mapViewport.x = 0;//(gd.player().pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    //gd.mapViewport.y = 0;

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

        // update tiles, we don't need to currently
        /*for (GameObject &tile : gd.mapTiles) {
            update(state, gd, res, tile, deltaTime);
        }*/
        // update chars
        /*for (GameObject &chars : gd.characters) {
            update(state, gd, res, chars, deltaTime);
        }*/

        // update lasers
        /*for (GameObject &laser : gd.lasers) {
            update(state, gd, res, laser, deltaTime);
        }*/

        // update portals
        for (Portal &portal : gd.portals_) {
            portal.update(state, gd, res, deltaTime);
        }

        // update lasers
        for (Laser &laser : gd.lasers_) {
            laser.update(state, gd, res, deltaTime);
        }

        // update bullets
        /*for (GameObject &bullet : gd.bullets) {
            update(state, gd, res, bullet, deltaTime);
        }*/

        // used for camera system
        gd.mapViewport.x = (gd.player.pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
        gd.mapViewport.y = (gd.player.pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 
        //draw bg
        SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
        SDL_RenderClear(state.renderer);

        // draw background
        /*SDL_FRect src = {
            gd.mapViewport.x,  // camera x
            gd.mapViewport.y,  // camera y
            gd.mapViewport.w,  // camera width (usually = screen width)
            gd.mapViewport.h   // camera height (usually = screen height)
        };
        SDL_FRect dst = {
            0, 0,              // always draw at (0,0) on the screen
            gd.mapViewport.w,  // draw to the screen width
            gd.mapViewport.h   // draw to the screen height
        };*/

        //SDL_RenderTexture(state.renderer, res.texBg1, &src, &dst);
        //drawParallaxBackground(state.renderer, res.texBg1, gd.player().vel.x, gd.bg4Scroll, 0.0375f, deltaTime);
        //drawParallaxBackground(state.renderer, res.texBg4, gd.player().vel.x, gd.bg4Scroll, 0.075f, deltaTime);
        //drawParallaxBackground(state.renderer, res.texBg3, gd.player().vel.x, gd.bg3Scroll, 0.15f, deltaTime);
        //drawParallaxBackground(state.renderer, res.texBg2, gd.player().vel.x, gd.bg2Scroll, 0.3f, deltaTime);

        // draw bg tiles
        /*for (GameObject &obj : gd.bgTiles) {
            SDL_FRect dst {
                .x = obj.pos.x - gd.mapViewport.x,
                .y = obj.pos.y - gd.mapViewport.y,
                .w = static_cast<float>(obj.texture->w),
                .h = static_cast<float>(obj.texture->h)
                
            }; 
            SDL_RenderTexture(state.renderer, obj.texture, nullptr, &dst);
        }*/

        // draw bg tiles
        for (BackgroundObject &bg : gd.bgTiles_) {
           bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
        }

        // draw level tiles
        /*for(GameObject &level : gd.mapTiles){
            if (level.data.level.state == LevelState::portal){
                drawObject(state, gd, level, 32, 64, deltaTime); 
            } else{
                drawObject(state, gd, level, TILE_SIZE, TILE_SIZE, deltaTime); 
            } 
        }*/

        // draw level tiles
        for(Level &level : gd.mapTiles_) {
            level.draw(state, gd, TILE_SIZE, TILE_SIZE);
            //drawLevel(state, gd, level, TILE_SIZE, TILE_SIZE, deltaTime); 
        }

        // draw portal tiles
        for(Portal &portal : gd.portals_) {
            portal.draw(state, gd, TILE_SIZE, TILE_SIZE * 2);
        }

        // draw chars
        /*for(GameObject &chars : gd.characters){
            if (isOnscreen(state, gd, chars)) {
                drawObject(state, gd, chars, TILE_SIZE, TILE_SIZE, deltaTime);
            }
        }*/
        /*for (auto &layer : gd.layers) {
            for (GameObject &obj : layer) {
                if (isOnscreen(state, gd, obj)) {
                    if (obj.data.level.state == LevelState::portal && obj.type == ObjectType::level){
                        drawObject(state, gd, obj, 32, 64, deltaTime); 
                    } else{
                        drawObject(state, gd, obj, TILE_SIZE, TILE_SIZE, deltaTime); 
                    }    
                }        
            }
        }*/

        //drawPlayer(state, gd, gd.player, TILE_SIZE, TILE_SIZE, deltaTime); 

        // draw player
        gd.player.draw(state, gd, TILE_SIZE, TILE_SIZE); // draw player class


        // draw lasers
        for(Laser &laser : gd.lasers_) {
            if (laser.laserActive) {
                laser.draw(state, gd, TILE_SIZE, TILE_SIZE);
            }
            //drawLevel(state, gd, level, TILE_SIZE, TILE_SIZE, deltaTime); 
        }

        // Draw Lasers
        /*for(GameObject &laser : gd.lasers){
            if (isOnscreen(state, gd, laser) && laser.data.obstacle.laserActive) {
                drawObject(state, gd, laser, TILE_SIZE, TILE_SIZE, deltaTime);
            }
        }*/
        

        // draw bullets
        /*for (GameObject &bullet : gd.bullets) {
            if (bullet.data.bullet.state != BulletState::inactive) {
                drawObject(state, gd, bullet, bullet.collider.w, bullet.collider.h, deltaTime);
            }
        }*/

        // draw fg tiles
        /*for (GameObject &obj : gd.fgTiles) {
            SDL_FRect dst {
                .x = obj.pos.x - gd.mapViewport.x,
                .y = obj.pos.y - gd.mapViewport.y,
                .w = static_cast<float>(obj.texture->w), 
                .h = static_cast<float>(obj.texture->h)
            };
            SDL_RenderTexture(state.renderer, obj.texture, nullptr, &dst);
        }*/ // turned off for now we dont have any

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
        /*if (dead) {
            goto main_loop;
            dead = false;
        }*/
    }

    res.unload();
    cleanup(state);
    return 0;
}