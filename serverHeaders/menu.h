#pragma once

#include <SDL3/SDL.h>
#include "../ext/glm/glm.hpp"
#include "animation.h"
#include "globals.h"
#include "object.h"

class charIconObject : public Object { // obj with anims
    public:
        int spriteFrame;
        std::vector<Animation> animations;
        int curAnimation;    
        float dir;
        charIconObject() : Object() { // default
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
        }
        charIconObject(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
        }

        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime, int newState);
        void draw(const SDLState &state, GameData &gd, float width, float height);
};