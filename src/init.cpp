#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/init.h"

using namespace std;

/*const size_t LAYER_IDX_LEVEL = 0;
const size_t LAYER_IDX_CHARACTERS = 1;

const int MAP_ROWS = 40;
const int MAP_COLS = 140;
const int TILE_SIZE = 32;*/

bool initialize(SDLState &state) {
    bool initSuccess = true;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Initializing SDL3", nullptr);
        initSuccess = false;
    } 
    state.window = SDL_CreateWindow("SDL3 Demo", state.width, state.height, SDL_WINDOW_RESIZABLE);
    if (!state.window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Creating Window", nullptr);
        cleanup(state);
        initSuccess = false;
    }

    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Creating Renderer", nullptr);
        cleanup(state);
        initSuccess = false;
    }

    //SDL_SetRenderVSync(state.renderer, 1); // turn this SHIT off

    SDL_SetWindowRelativeMouseMode(state.window, true); // mouse 
    //SDL_SetWindowMouseGrab(state.window, true); // dont let mouse go past

    // configure presentation
    SDL_SetRenderLogicalPresentation(state.renderer, state.logW, state.logH, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return initSuccess;
}


void cleanup(SDLState &state) {
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

bool isOnscreen(const SDLState &state, GameData &gd, GameObject &obj) { // checks if obj is onscreen (with some leeway)
    bool onscreen = true;
    if (obj.pos.x - gd.mapViewport.x + 2 * TILE_SIZE < 0 || // left side
        obj.pos.x - gd.mapViewport.x - 2 * TILE_SIZE > state.logW || // right side
        obj.pos.y - gd.mapViewport.y + 2 * TILE_SIZE < 0 || // up
        obj.pos.y - gd.mapViewport.y - 2 * TILE_SIZE > state.logH) // down
    {
        onscreen = false;
    }
    return onscreen;
}

float changeVel(float vel, GameObject &obj) { // this is for ease of accounting for obj_flip without having to have it every time, use when you change/need vel/pos
    return vel * obj.flip; 
}

bool isSliding(GameObject &obj) { // checks if an obj is sliding
    if (obj.vel.x * obj.dir < 0) {
       return true;     
    }
    return false;
}

glm::vec2 findCenterOfSprite(GameObject &obj) { // finds center of sprite by collider
    return glm::vec2((float)obj.collider.w / 2, (float)obj.collider.h / 2);
}
