#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/player.h"

class IdleState : public PlayerState {
    public:
        virtual void enter(Player& player)
        {
            //player.data.curAnimation = res.ANIM_PLAYER_IDLE;
        }
};