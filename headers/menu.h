#pragma once

#include <SDL3/SDL.h>
#include "../ext/glm/glm.hpp"
#include "animation.h"
#include "globals.h"
#include "object.h"

class charIconObject : public AnimatedObject { // obj with anims
    public:
        charIconObject() : AnimatedObject() { // default

        }
        charIconObject(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        AnimatedObject(pos_, colliderRect, tex) {

        }

        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime, int newState);
        void draw(const SDLState &state, GameData &gd, float width, float height);
};