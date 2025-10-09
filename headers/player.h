#pragma once

#include "../ext/glm/glm.hpp"
#include "animation.h"
#include <vector>
#include "object.h"
#include "timer.h"
#include "item.h"

struct SDLState;
struct GameData;
struct Resources;
class PlayerState;

class Player : public AnimatedObject { // player
    public:
        PlayerState* state_;
        Item item;
        Item nextItem;
        bool grounded; 
        bool isDead = false;
        bool hasItem = false;
        bool pickingItem = false;
        bool sprinting = false;
        bool fastFalling = false;
        bool canDoubleJump = true;
        bool usingSugar = false;

        float gravityScale; 
        float maxSpeedX; // will change depending on state
        float maxSpeedY = 550;
        float maxWalkX = 250; // walking
        float maxRunX = 650; // running 
        float maxSprintX = 850; // sprinting
        int currentDirection;
        int position = 8;
        Timer sprintTimer;
        Timer jetpackTimer;
        Timer cooldownTimer;

        AnimatedObject* blast;

        virtual void handleInput(SDL_KeyboardEvent& key, GameData &gd, Resources &res, float deltaTime);
        virtual void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        //void draw(const SDLState &state, GameData &gs, float width, float height);
        
        Player(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex, std::vector<Animation> anims, int curAnim, float maxSpeedX_) :
        AnimatedObject(pos_, colliderRect, tex), sprintTimer(1.5f), jetpackTimer(1.0f), cooldownTimer(5.0f) {
            acc = glm::vec2(300, 0); // default for now
            animations = anims;
            curAnimation = curAnim;
            currentDirection = 0;
            grounded = false;
            sprinting = false;
            maxSpeedX = maxSpeedX_; // should be walk speed
            gravityScale = 1.0f;
            
            
        }
        Player(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        AnimatedObject(pos_, colliderRect, tex), sprintTimer(1.5f), jetpackTimer(1.0f), cooldownTimer(5.0f) {
            grounded = false;
            sprinting = false;
            currentDirection = 0;
            gravityScale = 1.0f;
            maxSpeedX = 250; // walk speed default
        }

        Player() : AnimatedObject(), sprintTimer(1.5f), jetpackTimer(1.0f), cooldownTimer(5.0f) {
            grounded = false;
            gravityScale = 1.0f;
            currentDirection = 0;
            maxSpeedX = 250; // walk speed default
        }
        
};
