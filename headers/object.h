#pragma once

#include <SDL3/SDL.h>
#include <stdio.h>
#include "../ext/glm/glm.hpp"
#include "animation.h"
#include "globals.h"
#include <vector>
#include "resources.h"

struct SDLState;
struct GameData;
struct Resources;
class Player;
class PlayerState;

enum ObjectType{
    ANIMATED,
    LEVEL,
    BACKGROUND,
    PORTAL,
    LASER,
    ITEMBOX
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
        std::vector<Animation> animations;
        int spriteFrame;
        int curAnimation;  
        float width = TILE_SIZE;
        float height = TILE_SIZE; 
        float dir;
        float flip; // anti gravity
        bool visible; 
        float knockbackMultiplier;
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
        AnimatedObject(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex, float w, float h) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
            width = w;
            height = h;
        }
        void (*onCollision)(AnimatedObject &obj, GameData &gd, Resources &res, Player &player);
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
        width = TILE_SIZE;
        height = TILE_SIZE * 2;
    }
};

class Effect : public AnimatedObject{
    public:
    bool  followsPlayer; // i.e. boombox
    Effect() : AnimatedObject(){}
    Effect(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex, float w, float h) :
    AnimatedObject(pos_, colliderRect, tex, w, h) {}
    void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
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
        ItemBox() : Object(), itemBoxTimer(3.0f) {
            itemBoxActive = true;
            type = ITEMBOX;
        }
        ItemBox(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
            Object(pos_, colliderRect, tex), itemBoxTimer(3.0f) {
                itemBoxActive = true;
                type = ITEMBOX;
            }
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void generateItem(Player &player, GameData &gd, Resources &res);
};

// Stun for animated objects
void angledStun(AnimatedObject &obj, GameData &gd, Resources &res, Player &player);
void effectExplosion(GameData &gd, Resources &res, AnimatedObject obj);


class Hook : public AnimatedObject { // grappling hook projectile
    public:
        bool collided; // has hook hit something
        Hook() : AnimatedObject() { // default 
            collided = false;
        }  
        Hook(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : 
        AnimatedObject(pos_, colliderRect, tex) { // generic obj constructor
            collided = false;
        }
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
};
