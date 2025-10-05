#include <SDL3/SDL.h>
#include <cstdio>
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/helper.h"

void Object::draw(const SDLState &state, GameData &gd, float width, float height) {
    if (!isOnscreen(state, gd, (*this))) {
        return;
    }
    SDL_FRect dst {
        .x = (*this).pos.x - gd.mapViewport.x,
        .y = (*this).pos.y - gd.mapViewport.y,
        .w = width,
        .h = height
    };
    SDL_RenderTexture(state.renderer, (*this).texture, nullptr, &dst);
    (*this).drawDebug(state, gd, width, height);
}

void Object::drawDebug(const SDLState &state, GameData &gd, float width, float height) {
    if (gd.debugMode) {
        SDL_FRect rectA {
            .x = (*this).pos.x + (*this).collider.x - gd.mapViewport.x, 
            .y = (*this).pos.y + (*this).collider.y - gd.mapViewport.y,
            .w = (*this).collider.w, 
            .h = (*this).collider.h
        };
        SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 150);
        SDL_RenderFillRect(state.renderer, &rectA);
        SDL_FRect sensor{  
            .x = (*this).pos.x + (*this).collider.x - gd.mapViewport.x,
            .y = (*this).pos.y + (*this).collider.y + (*this).collider.h - gd.mapViewport.y,
            .w = (*this).collider.w, 
            .h = 1
        };
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 150);
        SDL_RenderFillRect(state.renderer, &sensor);

        SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_NONE);
    }
}

void AnimatedObject::draw(const SDLState &state, GameData &gd, float width, float height) {
    if (!isOnscreen(state, gd, (*this))) {
        return;
    }
    float srcX = (*this).curAnimation != -1 ? (*this).animations[(*this).curAnimation].currentFrame() * width : ((*this).spriteFrame - 1) * width;
    SDL_FRect src {
        .x = srcX,
        .y = 0,
        .w = width,
        .h = height
    };

    SDL_FRect dst {
        .x = (*this).pos.x - gd.mapViewport.x,
        .y = (*this).pos.y - gd.mapViewport.y,
        .w = width,
        .h = height
    };
    SDL_FlipMode flipMode; // = obj.dir == -1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    if ((*this).dir == -1) {            
        if ((*this).flip == -1) {
            flipMode = (SDL_FlipMode)3; // FLIP_VERTICAL_AND_HORIZONTAL
        } else {
            flipMode = SDL_FLIP_HORIZONTAL;
        }
    } else {
        if ((*this).flip == -1) {
            flipMode = SDL_FLIP_VERTICAL;
        } else {
            flipMode = SDL_FLIP_NONE;
        }
    }
    SDL_RenderTextureRotated(state.renderer, (*this).texture, &src, &dst, 0, nullptr, flipMode); // src is for sprite stripping, dest is for where sprite should be drawn
    (*this).drawDebug(state, gd, width, height);
}

void AnimatedObject::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) { // just step the anims
    if ((*this).curAnimation != -1) {
        (*this).animations[(*this).curAnimation].step(deltaTime);
    }
}

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

void BackgroundObject::draw(const SDLState &state, GameData &gd, float width, float height) { // same as Object.draw, just no debug option (maybe we can optimize this?)
    if (!isOnscreen(state, gd, (*this))) {
        return;
    }
    SDL_FRect dst {
        .x = (*this).pos.x - gd.mapViewport.x,
        .y = (*this).pos.y - gd.mapViewport.y,
        .w = width,
        .h = height
    };
    SDL_RenderTexture(state.renderer, (*this).texture, nullptr, &dst);
}

void Laser::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) { // update laser timer every frame
    Timer &laserTimer = (*this).laserTimer;
    laserTimer.step(deltaTime);
    if (laserTimer.isTimeOut()){
        //Resets the timer and switches tHe LaSeR
        laserTimer.reset();
        (*this).laserActive = !(*this).laserActive;
    }
}