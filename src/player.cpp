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
