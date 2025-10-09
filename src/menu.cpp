#include <SDL3/SDL.h>
#include <cstdio>
#include "../headers/menu.h"
#include "../headers/playerState.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/helper.h"

void charIconObject::draw(const SDLState &state, GameData &gd, float width, float height) {

    float srcX = ((*this).spriteFrame - 1) * width;
    
    SDL_FRect src {
        .x = srcX,
        .y = 0,
        .w = width,
        .h = height
    };

    SDL_FRect dst {
        .x = (*this).pos.x,
        .y = (*this).pos.y,
        .w = width,
        .h = height
    };
    SDL_FlipMode flipMode; // = obj.dir == -1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    flipMode = SDL_FLIP_NONE;
    SDL_RenderTextureRotated(state.renderer, (*this).texture, &src, &dst, 0, nullptr, flipMode); // src is for sprite stripping, dest is for where sprite should be drawn
    (*this).drawDebug(state, gd, width, height);
}

void charIconObject::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime, int newState) { // just step the anims

}