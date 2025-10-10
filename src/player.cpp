#include <SDL3/SDL.h>
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"

void Player::handleInput(SDL_Event &event, GameData &gd, Resources &res, float deltaTime){
    state_->handleInput(gd, res, event);
}

void Player::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    state_->update(state,gd, res, deltaTime);
}
