#pragma once

#include "../headers/object.h"
#include <vector>

class Minimap : public AnimatedObject {
    public:
        std::vector<Object> playerDots; // each player gets their own dot
        Minimap() : AnimatedObject() { 
            debug = false;
        }

        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void draw(const SDLState &state, GameData &gd, float width, float height);
};