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
    ITEMBOX,
    SIGN,
    WATER,
    LAVA,
    WIRE,
    ICE_BLOCK,
    REVOLVER,
    CACTUS,
};

class Object { // generic obj type    
    public:
        glm::vec2 pos, vel, acc;
        SDL_Texture *texture;
        SDL_FRect collider; // rectangle for collision
        float width, height; // size for drawing
        bool debug; // should draw debug?
        bool visible; 
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
        Object(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) {
            pos = pos_;       
            texture = tex;
            collider = colliderRect;
            vel = acc = glm::vec2(0);
        }
        virtual ~Object() {}
        virtual void draw(const SDLState &state, GameData &gd, float width, float height);
        void drawDebug(const SDLState &state, GameData &gd, float width, float height);
        void drawDebugNearby(const SDLState &state, GameData &gd, float width, float height);  
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
        bool persistent; // will be drawn even if "offscreen"
        AnimatedObject() : Object() { // default
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
            visible = true;
            debug = true;
            persistent = false;
        }
        AnimatedObject(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            spriteFrame = 1;
            curAnimation = -1;
            dir = 1;
            flip = 1.0f;
            type = ANIMATED;
            visible = true;
            debug = true;
            persistent = false;
        }
        //virtual ~AnimatedObject() {}
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
};

class BackgroundObject : public Object { // bg tiles
    public:
        BackgroundObject() : Object() { // default 
            type = BACKGROUND;
        }
        BackgroundObject(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            type = BACKGROUND;
            debug = false;
        }
};

class Level : public Object { // the level type!
    public:
        Level() : Object() { // default

        }
        Level(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
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
            debug = true;
        }
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
};

class Sign:  public Object {
    public:
        Sign() : Object() { // default 
            type = SIGN;
            debug = false;
        }
        Sign(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            type = SIGN;
            debug = false;
        }
};

class Wire:  public Object {
    public:
        Wire() : Object() { // default 
            type = WIRE;
            debug = false;
        }
        Wire(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            type = WIRE;
            debug = false;
        }
        void draw(const SDLState &state, GameData &gd, float width, float height);
};

class Water: public Object {
    public:
        Water() : Object() { // default 
            type = WATER;
            debug = false;
        }
        Water(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
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
        Lava(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Object(pos_, colliderRect, tex) {
            type = LAVA;
            debug = false;
        }
};

class IceBlock: public Level {
    public:
        IceBlock() : Level() { // default 
            type = ICE_BLOCK;
            debug = false;
        }
        IceBlock(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Level(pos_, colliderRect, tex) {
            type = ICE_BLOCK;
            debug = false;
        }
};

class Revolver : public AnimatedObject {
    public:
        Timer useTimer; // how long until usable again
        bool inUse; // is someone using the revolver
        glm::vec2 launchVel; // how much velocity to give the player using it
        int orientation; // where is the revolver pointed
        // 0 = R,  1 = DR, 2 = D
        // 3 = DL, 4 = L,  5 = LU
        // 6 = U,  7 = UR
        int cycle; // used for determining when rotation should stop
        bool spinning; // should it spin?
        int rotation; // for spinning the revolver
        Player *player; // the player being held currently
        Revolver() : AnimatedObject(), useTimer(0.5f) { // default 
            inUse = false;
            launchVel = glm::vec2(25.0f, -1000.0f);
            orientation = 0;
            rotation = 0; 
            cycle = 0;
            type = REVOLVER;
            visible = true;
            debug = true;
            spinning = false;
            player = nullptr;
        }  
        Revolver(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : 
        AnimatedObject(pos_, colliderRect, tex), useTimer(0.5f) { // generic obj constructor
            inUse = false;
            launchVel = glm::vec2(25.0f, -1000.0f);
            orientation = 0;
            rotation = 0; 
            cycle = 0;
            type = REVOLVER;
            visible = true;
            debug = true;
            spinning = false;
            player = nullptr;
        }
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
};

class Cactus: public Level {
    public:
        Cactus() : Level() { // default 
            type = CACTUS;
            debug = false;
        }
        Cactus(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        Level(pos_, colliderRect, tex) {
            type = CACTUS;
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

class Hook : public AnimatedObject { // grappling hook projectile
    public:
        bool collided; // has hook hit something
        Hook() : AnimatedObject() { // default 
            collided = false;
            visible = false;
        }  
        Hook(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : 
        AnimatedObject(pos_, colliderRect, tex) { // generic obj constructor
            collided = false;
            visible = false;
            debug = true;
        }
        void draw(const SDLState &state, GameData &gd, Player &p, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
};
