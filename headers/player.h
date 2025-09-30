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
#include "playerState.h"

    // PlayerState state;
    // Timer weaponTimer;
    // Timer deathTimer;
    // Timer sprintTimer; // time 
    // int healthPoints;
    // float maxWalkX; // walking
    // float maxRunX; // running 
    // float maxSprintX; // sprinting
    // bool fastfalling;
    // bool canDoubleJump; // can double jump?

    //glm::vec2 pos, vel, acc;
    // float dir;

    // float maxSpeedX; // maximum (sprinting)
    // //float maxSpeedY;
    // std::vector<Animation> animations;
    // int curAnimation;
    // SDL_Texture *texture;
    // bool dynamic;
    // bool grounded;
    // float gravityScale; // how fast they fall
    // float flip; // anti gravity
    // SDL_FRect collider; // rectangle for collision
    // Timer flashTimer;
    // bool shouldFlash;
    // int spriteFrame;

struct PlayerData {
    glm::vec2 pos, vel, acc;
    float dir;
    bool grounded;
    float gravityScale;
    float flip; // anti gravity
    SDL_FRect collider; // rectangle for collision
    int spriteFrame;
    float maxSpeedX; // maximum (sprinting)
    std::vector<Animation> animations;
    int curAnimation;
    SDL_Texture *texture;
};

class Player {
    public:
        PlayerData data;
        virtual void handleInput(SDL_KeyboardEvent& key){}
        virtual void update(){}
        Player(float x, float y, std::vector<Animation> animations, int curAnimation, SDL_FRect collider, SDL_Texture *texture) {
            data.pos = glm::vec2(x, y);
            data.acc = glm::vec2(300, 0);
            data.animations = animations;
            data.curAnimation = curAnimation;
            data.collider = collider;
            data.maxSpeedX = 250; // walking speed for now     
            data.texture = texture;
        }
        Player() {
            data.pos = data.acc = glm::vec2(0,0);
        }
    private:
        PlayerState* state_;
};
