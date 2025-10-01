#include <SDL3/SDL.h>
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"

void Player::handleInput(SDL_KeyboardEvent& key){
    PlayerState* state = state_->handleInput(*this, key);
    if (state != NULL) {
        delete state_;
        state_ = state;
        // do enter action on new state
        state_->enter(*this);
    }
}

void Player::update() {
    state_->update(*this);
}

void Player::draw(const SDLState &state, GameState &gs, float width, float height) {
    float srcX = (*this).curAnimation != -1 ? (*this).animations[(*this).curAnimation].currentFrame() * width : ((*this).spriteFrame - 1) * width;
    SDL_FRect src {
        .x = srcX,
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
}
