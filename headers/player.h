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

enum characterType {
    SHOTGUN,
    SWORD,
    JETPACK
};

class Player : public AnimatedObject { // player
    public:
        characterType character; // who are you  
        PlayerState* state_;
        
        Hook hook;
        Item item;
        Item nextItem;
        bool hasItem = false;
        bool pickingItem = false;

        bool grounded; 
        bool isDead = false;

        bool isStunned = false;
        bool sprinting = false;

        bool canDoubleJump = true;
        bool usingSugar = false;

        int currentDirection;
        int position = 4;
        // TODO maybe keep consistent across clients
        int index; // Currently for keeping track of item's owner 
        float gravityScale; 
        float maxSpeedX; // will change depending on state
        float maxWalkX = 300; // walking
        float maxRunX = 700; // running 
        float maxSprintX = 900; // sprinting
        
        Timer jetpackTimer;
        Timer cooldownTimer;

        void draw(const SDLState &state, GameData &gd, float width, float height);
        virtual void handleInput(const SDLState &state, GameData &gd, Resources &res, SDL_Event &event, float deltaTime);
        virtual void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void handleState(PlayerState* &pState, GameData &gd, Resources &res);
        //void draw(const SDLState &state, GameData &gs, float width, float height);
        
        Player(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex, std::vector<Animation> anims, int curAnim, float maxSpeedX_) :
        AnimatedObject(pos_, colliderRect, tex), jetpackTimer(1.0f), cooldownTimer(5.0f) {
            acc = glm::vec2(330, 0); // default for now
            animations = anims;
            curAnimation = curAnim;
            currentDirection = 0;
            grounded = false;
            sprinting = false;
            maxSpeedX = maxSpeedX_; // should be walk speed
            gravityScale = 1.0f;
            character = SHOTGUN;            
        }
        Player(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        AnimatedObject(pos_, colliderRect, tex), jetpackTimer(1.0f), cooldownTimer(5.0f) {
            grounded = false;
            sprinting = false;
            currentDirection = 0;
            gravityScale = 1.0f;
            maxSpeedX = 250; // walk speed default
            character = SHOTGUN; 
        }

        Player() : AnimatedObject(), jetpackTimer(1.0f), cooldownTimer(5.0f) {
            grounded = false;
            gravityScale = 1.0f;
            currentDirection = 0;
            maxSpeedX = 250; // walk speed default
            character = SHOTGUN; 
        } 
};
