#pragma once

#include "../ext/glm/glm.hpp"
#include "animation.h"
#include <vector>
#include "object.h"

struct SDLState;
struct GameState;
struct Resources;
class PlayerState;

class Player : public AnimatedObject { // player
    public:
        bool grounded; 
        bool sprinting;
        float gravityScale; 
        float maxSpeedX; // will change depending on state

        virtual void handleInput(SDL_KeyboardEvent& key);
        virtual void update();
        void draw(const SDLState &state, GameState &gs, float width, float height);
        
        Player(float x, float y, std::vector<Animation> anims, SDL_FRect colliderRect, SDL_Texture *tex, float maxSpeedX_) {
            pos = glm::vec2(x, y);
            vel = glm::vec2(0);
            acc = glm::vec2(300, 0); // default for now
            texture = tex;
            collider = colliderRect;
            
            spriteFrame = 1;
            animations = anims;
            curAnimation = -1;
            dir = 1;
            flip = 1.0f;

            grounded = false;
            sprinting = false;
            maxSpeedX = maxSpeedX_; // should be walk speed
            gravityScale = 1.0f;
            
        }
        Player() : AnimatedObject() {
            grounded = false;
            gravityScale = 1.0f;
            maxSpeedX = 250; // walk speed default
        }
    private:
        PlayerState* state_;
};
