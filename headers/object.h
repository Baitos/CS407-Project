#pragma once

#include <SDL3/SDL.h>
#include "../ext/glm/glm.hpp"
#include "../headers/animation.h"
#include <vector>

struct SDLState;
struct GameState;
struct Resources;
class PlayerState;

class Object {   // generic obj type    
    public:
        glm::vec2 pos, vel, acc;
        SDL_Texture *texture;
        SDL_FRect collider; // rectangle for collision
        Object() {           
            pos = vel = acc = glm::vec2(0,0);
        }
        void draw(const SDLState &state, GameState &gs, float width, float height);
};

class AnimatedObject : public Object { // obj with anims
    public:
        int spriteFrame;
        std::vector<Animation> animations;
        int curAnimation;    
        float dir;
        float flip; // anti gravity
        AnimatedObject() : Object() {
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
        }
        void update(const SDLState &state, GameState &gs, Resources &res, float deltaTime);
        void draw(const SDLState &state, GameState &gs, float width, float height);
};

class Level : public Object { // the level type!
    public:
        Level() : Object() {

        }
};

class Laser : public Object { // obstacle
    public:
        bool laserActive;
        Timer laserTimer;
        Laser() : laserTimer(2.1f)
        {
            laserActive = true; 
        }
        void update(const SDLState &state, GameState &gs, Resources &res, float deltaTime);
};