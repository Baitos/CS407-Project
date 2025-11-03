#pragma once

#include "../ext/glm/glm.hpp"
#include "animation.h"
#include "object.h"
#include "timer.h"
#include "item.h"

#include <vector>
#include <memory>

struct SDLState;
struct GameData;
struct Resources;
class PlayerState;

enum characterType {
    SHOTGUN,
    SWORD,
    JETPACK
};

class Player : public AnimatedObject { // player
    public:
        characterType character; // who are you  
        PlayerState* state_;

        std::string username;
        
        Hook hook;
        Item* heldItem; // item currently active
        bool hasItem = false;
        bool pickingItem = false;

        std::vector<Item*> items_; // store all of this player's active item objs here

        bool grounded; 
        bool isDead = false;
        
        bool sprinting = false;

        bool canDoubleJump = true;
        bool usingSugar = false;

        float gravityScale; 
        float maxSpeedX; // will change depending on state
        float maxWalkX = 300; // walking
        float maxRunX = 700; // running 
        float maxSprintX = 900; // sprinting
        int currentDirection;
        
        Timer jetpackTimer;
        Timer cooldownTimer;
        Timer respawnTimer;

        int lapsCompleted = 0;
        int lastCheckpoint = 0;

        int position = 4;
        int index;

        void draw(const SDLState &state, GameData &gd, float width, float height);
        virtual void handleInput(const SDLState &state, GameData &gd, Resources &res, SDL_KeyboardEvent& key, float deltaTime);
        virtual void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);

        void checkCollision(const SDLState &state, GameData &gd, Resources &res,
 	                        float deltaTime);
        void collisionResponse(const SDLState &state, GameData &gd, Resources &res,
 	                           Object &o, SDL_FRect &rectA, SDL_FRect &rectB, glm::vec2 &resolution, float deltaTime);
        void groundedCheck(Object &o, SDL_FRect &rectB);

        void handleState(PlayerState* &pState, GameData &gd, Resources &res);
        //void draw(const SDLState &state, GameData &gs, float width, float height);
        
        Player(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex, std::vector<Animation> anims, int curAnim, float maxSpeedX_) :
        AnimatedObject(pos_, colliderRect, tex), jetpackTimer(1.0f), cooldownTimer(5.0f), respawnTimer(2.0f) {
            acc = glm::vec2(330, 0); // default for now
            animations = anims;
            curAnimation = curAnim;
            currentDirection = 0;
            grounded = false;
            sprinting = false;
            maxSpeedX = maxSpeedX_; // should be walk speed
            gravityScale = 1.0f;
            character = SHOTGUN;   
            heldItem = nullptr;         
        }
        Player(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        AnimatedObject(pos_, colliderRect, tex), jetpackTimer(1.0f), cooldownTimer(5.0f), respawnTimer(2.0f) {
            grounded = false;
            sprinting = false;
            currentDirection = 0;
            gravityScale = 1.0f;
            maxSpeedX = 300; // walk speed default
            character = SHOTGUN; 
            heldItem = nullptr; 
        }

        Player() : AnimatedObject(), jetpackTimer(1.0f), cooldownTimer(5.0f), respawnTimer(2.0f) {
            grounded = false;
            gravityScale = 1.0f;
            currentDirection = 0;
            maxSpeedX = 300; // walk speed default
            character = SHOTGUN; 
            heldItem = nullptr; 
        } 
};
