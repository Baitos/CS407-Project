#pragma once

#include <SDL3/SDL.h>
#include "../ext/glm/glm.hpp"
#include "animation.h"
#include "globals.h"
#include <vector>
#include "resources.h"

struct SDLState;
struct GameData;
class PlayerState;

enum ObjectType{
    ANIMATED,
    LEVEL,
    BACKGROUND,
    PORTAL,
    LASER
};

class Object { // generic obj type    
    public:
        glm::vec2 pos, vel, acc;
        SDL_Texture *texture;
        SDL_FRect collider; // rectangle for collision
        int type;
        Object() {           
            pos = vel = acc = glm::vec2(0);
            collider = {
                .x = 0,
                .y = 0,
                .w = (float)TILE_SIZE,
                .h = (float)TILE_SIZE
            };
        }
        Object(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) {
            pos = pos_;       
            texture = tex;
            collider = colliderRect;
            vel = acc = glm::vec2(0);
        }
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void drawDebug(const SDLState &state, GameData &gd, float width, float height);
        virtual ~Object() {}
};

class AnimatedObject : public Object { // obj with anims
    public:
        int spriteFrame;
        std::vector<Animation> animations;
        int curAnimation;    
        float dir;
        float flip; // anti gravity
        bool visible; 
        AnimatedObject() : Object() { // default
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
            visible = true;
        }
        AnimatedObject(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
            visible = true;
        }

        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void draw(const SDLState &state, GameData &gd, float width, float height);
};

class BackgroundObject : public Object { // bg tiles
    public:
        BackgroundObject() : Object() { // default 
            type = BACKGROUND;
        }
        BackgroundObject(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            type = BACKGROUND;
        }
        void draw(const SDLState &state, GameData &gd, float width, float height);
};

class Level : public Object { // the level type!
    public:
        Level() : Object() { // default

        }
        Level(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            type = LEVEL;
        }
};

class Portal : public AnimatedObject { // portals
    public:
        int _id;
        bool isEntrance;
    Portal() : AnimatedObject() { // default 
        _id = std::floor(portalID);
        isEntrance = false;
        portalID += 0.5;
        type = PORTAL;
    }  
    Portal(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : 
    AnimatedObject(pos_, colliderRect, tex) { // generic obj constructor
        _id = std::floor(portalID);
        isEntrance = false;
        portalID += 0.5;
        type = PORTAL;
    }
};

class Laser : public Object { // obstacle
    public:
        bool laserActive;
        Timer laserTimer;
        Laser() : Object(), laserTimer(2.1f)
        {
            laserActive = true; 
            type = LASER;
        }
        Laser(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
        Object(pos_, colliderRect, tex), laserTimer(2.1f) {
            laserActive = true;
            type = LASER;
        }
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
};

class Hook : public AnimatedObject { // grappling hook projectile
    public:
        Hook() : AnimatedObject() { // default 
        }  
        Hook(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : 
        AnimatedObject(pos_, colliderRect, tex) { // generic obj constructor

        }
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
};