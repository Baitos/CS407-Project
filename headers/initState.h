#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file

struct SDLState
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height, logW, logH;
    float xRatio, yRatio;
    const bool *keys;
    SDLState() : keys(SDL_GetKeyboardState(nullptr)) {

    }
};
bool initialize(SDLState &state);
void cleanup(SDLState &state);