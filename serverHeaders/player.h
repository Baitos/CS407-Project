#pragma once

#include "../ext/glm/glm.hpp"
#include "../serverHeaders/object.h"
#include "../serverHeaders/timer.h"
#include "../serverHeaders/item.h"

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

        bool isStunned = false;
        bool sprinting = false;

        bool canDoubleJump = true;
        bool usingSugar = false;

        int currentDirection;
        int position = -1;
        int index;
        // TODO maybe keep consistent across clients
        float gravityScale; 
        float maxSpeedX; // will change depending on state
        float maxWalkX = 300; // walking
        float maxRunX = 700; // running 
        float maxSprintX = 900; // sprinting
        
        Timer jetpackTimer;
        Timer cooldownTimer;
        Timer respawnTimer;

        int lapsCompleted = 0;
        int lastCheckpoint = 0;

        bool hasFog = false;
    
        virtual void handleInput(const SDLState &state, GameData &gd, int keyID, int keyDown, float deltaTime);
        virtual void update(const SDLState &state, GameData &gd, float deltaTime, int keyID, int keyDown);

        void checkCollision(const SDLState &state, GameData &gd, 
 	                        float deltaTime);
        void collisionResponse(const SDLState &state, GameData &gd, 
 	                           Object &o, SDL_FRect &rectA, SDL_FRect &rectB, glm::vec2 &resolution, float deltaTime);
        void groundedCheck(Object &o, SDL_FRect &rectB);

        void handleState(PlayerState* &pState, GameData &gd);
        //void draw(const SDLState &state, GameData &gs, float width, float height);
        
        Player(glm::vec2 pos_, SDL_FRect colliderRect, float maxSpeedX_) :
        AnimatedObject(pos_, colliderRect), jetpackTimer(1.0f), cooldownTimer(5.0f), respawnTimer(2.0f) {
            acc = glm::vec2(330, 0); // default for now
            currentDirection = 0;
            grounded = false;
            sprinting = false;
            maxSpeedX = maxSpeedX_; // should be walk speed
            gravityScale = 1.0f;
            character = SHOTGUN;   
            heldItem = nullptr;         
        }
        Player(glm::vec2 pos_, SDL_FRect colliderRect) : // generic obj constructor
        AnimatedObject(pos_, colliderRect), jetpackTimer(1.0f), cooldownTimer(5.0f), respawnTimer(2.0f) {
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
