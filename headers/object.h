#pragma once

#include <SDL3/SDL.h>
#include "../ext/glm/glm.hpp"
#include "animation.h"
#include "globals.h"
#include <vector>

struct SDLState;
struct GameData;
struct Resources;
class PlayerState;

enum ObjectType{
    ANIMATED,
    LEVEL,
    BACKGROUND,
    PORTAL,
    LASER,
    ITEMBOX
};

class Object {   // generic obj type    
    public:
        glm::vec2 pos, vel, acc;
        SDL_Texture *texture;
        SDL_FRect collider; // rectangle for collision
        int type;
        Object() {           
            pos = vel = acc = glm::vec2(0);
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
        AnimatedObject() : Object() { // default
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
        }
        AnimatedObject(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
        }

        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void draw(const SDLState &state, GameData &gd, float width, float height);
};

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

class Level : public Object { // the level type!
    public:
        Level() : Object() { // default

        }
        Level(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            type = LEVEL;
        }
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

class ItemBox : public Object {
    public:
        bool itemBoxActive;
        Timer itemBoxTimer;
        float cooldownLength = 3.0f;
        ItemBox() : Object(), itemBoxTimer(cooldownLength) {
            itemBoxActive = true;
            type = ITEMBOX;
        }
        ItemBox(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
            Object(pos_, colliderRect, tex), itemBoxTimer(cooldownLength) {
                itemBoxActive = true;
                type = ITEMBOX;
            }
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
};


