#include <SDL3/SDL.h>
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"

void Object::draw(const SDLState &state, GameState &gs, float width, float height) {
    SDL_FRect src {
        .x = 0,
        .y = 0,
        .w = width,
        .h = height
    };

    SDL_FRect dst {
        .x = (*this).pos.x - gs.mapViewport.x,
        .y = (*this).pos.y - gs.mapViewport.y,
        .w = width,
        .h = height
    };
    SDL_RenderTextureRotated(state.renderer, (*this).texture, &src, &dst, 0, nullptr, SDL_FLIP_NONE);
}

void Laser::update(const SDLState &state, GameState &gs, Resources &res, float deltaTime) {
    Timer &laserTimer = (*this).laserTimer;
    laserTimer.step(deltaTime);
    if (laserTimer.isTimeOut()){
        //Resets the timer and switches tHe LaSeR
        laserTimer.reset();
        (*this).laserActive = !(*this).laserActive;
    }
}