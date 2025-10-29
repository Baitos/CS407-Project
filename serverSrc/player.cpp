#include <SDL3/SDL.h>
#include "../serverHeaders/player.h"
#include "../serverHeaders/playerState.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/helper.h"

void Player::draw(const SDLState &state, GameData &gd, float width, float height) {
    //(*this).hook.draw(state, gd, (*this), HOOK_SIZE, HOOK_SIZE); // draw hook under player
    
    //AnimatedObject::draw(state, gd, width, height); // do generic object draw for player
}

void Player::handleState(PlayerState* &pState, GameData &gd, Resources &res) {
    if (pState != nullptr) {
        state_->exit(gd, res, (*this)); // exit fn for old state
        delete state_;
        state_ = pState;
        state_->enter(gd, res, (*this)); // enter fn for new state
    }
}

void Player::handleInput(const SDLState &state, GameData &gd, Resources &res, SDL_KeyboardEvent& key, float deltaTime) {
    PlayerState* pState = state_->handleInput(state, gd, res, (*this), key);
    (*this).handleState(pState, gd, res);
}

void Player::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    PlayerState* pState = state_->update(state, gd, res, (*this), deltaTime);
    (*this).handleState(pState, gd, res);

    if ((*this).pickingItem) { // update item roulette
        gd.itemStorage_.update(state, gd, res, (*this), deltaTime);
    }

    (*this).hook.update(state, gd, res, deltaTime); // update this player's hook and handle its collision
    (*this).hook.checkCollision(state, gd, res, (*this), deltaTime);
    
    //printf("currentDirection: %d\n", (*this).currentDirection);
    (*this).vel += static_cast<float>((*this).currentDirection) * (*this).acc * deltaTime; // update player velocity based on acceleration and direction
    (*this).pos += updatePos((*this), deltaTime); // update pos
    
    if (std::abs((*this).vel.x) > (*this).maxSpeedX) {
        if (!isSliding((*this))) { // if not sliding slow down
            (*this).vel.x -= 1.5 * (*this).acc.x * deltaTime * (*this).currentDirection;
        }
    }

    // collision
    (*this).grounded = false;
    collisionCheckAndResponse(state,gd,res,(*this),deltaTime);
}
