#include <SDL3/SDL.h>
#include "../serverHeaders/player.h"
#include "../serverHeaders/playerState.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/helper.h"


void Player::handleState(PlayerState* &pState, GameData &gd) {
    if (pState != nullptr) {
        state_->exit(gd, (*this)); // exit fn for old state
        printf("delete player state\n");
        delete state_;
        state_ = pState;
        state_->enter(gd, (*this)); // enter fn for new state
    }
}

void Player::handleInput(const SDLState &state, GameData &gd, int keyID, int keyDown, float deltaTime) {
    PlayerState* pState = state_->handleInput(state, gd, (*this), keyID, keyDown);
    this->handleState(pState, gd);
}

void Player::update(const SDLState &state, GameData &gd, float deltaTime, int keyID, int keyDown) {
    
    PlayerState* pState = state_->update(state, gd, (*this), deltaTime, keyID, keyDown);
    this->handleState(pState, gd);

    if (this->pickingItem) { // update item roulette
        gd.itemStorage_.update(state, gd, (*this), deltaTime);
    }

    this->hook.update(state, gd, deltaTime); // update this player's hook and handle its collision
    this->hook.checkCollision(state, gd, (*this), deltaTime);
    
    for (auto i = this->items_.begin(); i != this->items_.end(); ) { // update active items
        Item* item = *i;
        item->update(state, gd, (*this), deltaTime); 
        item->checkCollision(state, gd, (*this), deltaTime);
        if (!item->active) {
            printf("delete player item in update\n");
            delete item;
            i = this->items_.erase(i);
        } else {
            i++;
        }
        //printf("vector size: %d\n", this->items_.size());
    }

    //printf("currentDirection: %d\n", this->currentDirection);
    this->vel += static_cast<float>(this->currentDirection) * this->acc * deltaTime; // update player velocity based on acceleration and direction
    this->pos += updatePos((*this), deltaTime); // update pos
    
    if (std::abs(this->vel.x) > this->maxSpeedX) {
        if (!(this->vel.x * this->dir < 0)) { // if not sliding slow down
            this->vel.x -= 1.5 * this->acc.x * deltaTime * this->currentDirection;
        }
    }
    

    // collision
    this->grounded = false;
    //collisionCheckAndResponse(state,gd,res,(*this),deltaTime);
    this->checkCollision(state, gd, deltaTime);

    // check if player has fallen off of map
    handleOutOfMap(gd, (*this));

}
