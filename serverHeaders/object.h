#pragma once

#include <SDL3/SDL.h>
#include <stdio.h>
#include "../ext/glm/glm.hpp"
#include "timer.h"
#include "globals.h"
#include <vector>

struct SDLState;
struct GameData;
class Player;
class PlayerState;

enum ObjectType{
    ANIMATED,
    LEVEL,
    BACKGROUND,
    PORTAL,
    LASER,
    ITEMBOX,
    SIGN,
    WATER,
    LAVA
};

class Object { // generic obj type    
    public:
        glm::vec2 pos, vel, acc;
        SDL_FRect collider; // rectangle for collision
        float width, height; // size for drawing
        bool debug; // should draw debug?
        int type;
        Object() {           
            pos = vel = acc = glm::vec2(0);
            collider = {
                .x = 0,
                .y = 0,
                .w = (float)TILE_SIZE,
                .h = (float)TILE_SIZE
            };
            width = height = TILE_SIZE;
            debug = true;
        }
        Object(glm::vec2 pos_, SDL_FRect colliderRect) {
            pos = pos_;       
            collider = colliderRect;
            vel = acc = glm::vec2(0);
        }
        virtual ~Object() {}
};

class AnimatedObject : public Object { // obj with anims
    public: 
        float width = TILE_SIZE;
        float height = TILE_SIZE; 
        float dir;
        float flip; // anti gravity
        bool visible; 
        bool persistent; // will be drawn even if "offscreen"
        AnimatedObject() : Object() { // default
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
            visible = true;
            debug = true;
            persistent = false;
        }
        AnimatedObject(glm::vec2 pos_, SDL_FRect colliderRect) : // generic obj constructor
        Object(pos_, colliderRect) {
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
            visible = true;
            debug = true;
            persistent = false;
        }
        //virtual ~AnimatedObject() {}
        void update(const SDLState &state, GameData &gd, float deltaTime);
};

class BackgroundObject : public Object { // bg tiles
    public:
        BackgroundObject() : Object() { // default 
            type = BACKGROUND;
        }
        BackgroundObject(glm::vec2 pos_, SDL_FRect colliderRect) : // generic obj constructor
        Object(pos_, colliderRect) {
            type = BACKGROUND;
            debug = false;
        }
};

class Level : public Object { // the level type!
    public:
        Level() : Object() { // default

        }
        Level(glm::vec2 pos_, SDL_FRect colliderRect) : // generic obj constructor
        Object(pos_, colliderRect) {
            type = LEVEL;
            debug = true;
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
    Portal(glm::vec2 pos_, SDL_FRect colliderRect) : 
    AnimatedObject(pos_, colliderRect) { // generic obj constructor
        _id = std::floor(portalID);
        isEntrance = false;
        portalID += 0.5;
        type = PORTAL;
        width = TILE_SIZE;
        height = TILE_SIZE * 2;
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
        Laser(glm::vec2 pos_, SDL_FRect colliderRect) :
        Object(pos_, colliderRect), laserTimer(2.1f) {
            laserActive = true;
            type = LASER;
            debug = true;
        }
        void update(const SDLState &state, GameData &gd, float deltaTime);
};

class Sign:  public Object {
    public:
        Sign() : Object() { // default 
            type = SIGN;
            debug = false;
        }
        Sign(glm::vec2 pos_, SDL_FRect colliderRect) : // generic obj constructor
        Object(pos_, colliderRect) {
            type = SIGN;
            debug = false;
        }
};

class Water: public Object {
    public:
        Water() : Object() { // default 
            type = WATER;
            debug = false;
        }
        Water(glm::vec2 pos_, SDL_FRect colliderRect) : // generic obj constructor
        Object(pos_, colliderRect) {
            type = WATER;
            debug = false;
        }
};

class Lava: public Object {
    public:
        Lava() : Object() { // default 
            type = LAVA;
            debug = false;
        }
        Lava(glm::vec2 pos_, SDL_FRect colliderRect) : // generic obj constructor
        Object(pos_, colliderRect) {
            type = LAVA;
            debug = false;
        }
};

class ItemBox : public Object {
    public:
        bool itemBoxActive;
        Timer itemBoxTimer;
        ItemBox() : Object(), itemBoxTimer(3.0f) {
            itemBoxActive = true;
            type = ITEMBOX;
        }
        ItemBox(glm::vec2 pos_, SDL_FRect colliderRect) :
            Object(pos_, colliderRect), itemBoxTimer(3.0f) {
                itemBoxActive = true;
                type = ITEMBOX;
            }
        void update(const SDLState &state, GameData &gd, float deltaTime);
        void generateItem(Player &player, GameData &gd);
};

// Stun for animated objects
void angledStun(AnimatedObject &obj, GameData &gd, Player &player);

class Hook : public AnimatedObject { // grappling hook projectile
    public:
        bool collided; // has hook hit something
        Hook() : AnimatedObject() { // default 
            collided = false;
            visible = false;
        }  
        Hook(glm::vec2 pos_, SDL_FRect colliderRect) : 
        AnimatedObject(pos_, colliderRect) { // generic obj constructor
            collided = false;
            visible = false;
            debug = true;
        }
        // void draw(const SDLState &state, GameData &gd, Player &p, float width, float height);
        void update(const SDLState &state, GameData &gd, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Player &p, float deltaTime);
};
