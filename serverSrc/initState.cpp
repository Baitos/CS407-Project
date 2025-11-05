#include <stdio.h>
#include "../serverHeaders/initState.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/gameData.h"

bool initialize(SDLState &state) {
    bool initSuccess = true;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Initializing SDL3", nullptr);
        printf("Error Initializing SDL3\n");
        initSuccess = false;
    } 
    state.window = SDL_CreateWindow("Retro Runners", state.width, state.height, SDL_WINDOW_RESIZABLE);
    if (!state.window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Creating Window", nullptr);
        printf("Error Creating Window\n");
        cleanup(state);
        initSuccess = false;
    }

    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Creating Renderer", nullptr);
        printf("Error Creating Renderer\n");
        cleanup(state);
        initSuccess = false;
    }
    state.keys = SDL_GetKeyboardState(nullptr);

    //SDL_SetRenderVSync(state.renderer, 1); // turn this SHIT off

    SDL_SetWindowRelativeMouseMode(state.window, true); // mouse 

    // configure presentation
    SDL_SetRenderLogicalPresentation(state.renderer, state.logW, state.logH, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return initSuccess;
}


void cleanup(SDLState &state) {
    SDL_Quit();
}

