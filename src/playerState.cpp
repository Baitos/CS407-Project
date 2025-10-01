#include <SDL3/SDL.h>

#include "../headers/player.h"
#include "../headers/playerState.h"
#include <vector>

PlayerState* PlayerState::handleInput(Player& player, SDL_KeyboardEvent key) {
    return NULL;
};
void PlayerState::update(Player& player) {
};
void PlayerState::enter(Player& player) {
};
// for generic playerState, do nothing. This solely exists to be overwritten.

PlayerState* IdleState::handleInput(Player& player, SDL_KeyboardEvent key) { 
    return NULL;
};
void IdleState::update(Player& player) {
};
void IdleState::enter(Player& player) {
};