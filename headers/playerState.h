#pragma once

#include <stdio.h>
#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "init.h"

class Player;

class PlayerState {
    public:
        virtual ~PlayerState() {}
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key){}
        virtual void update(Player& player){}
        virtual void enter(Player& player){}
    
};

class IdleState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key){}
        virtual void update(Player& player){}
        virtual void enter(Player& player){}
    
};

class WalkState : public PlayerState {

};