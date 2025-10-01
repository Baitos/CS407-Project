#pragma once

#include "../ext/glm/glm.hpp"
#include "animation.h"
#include <vector>
#include "object.h"

struct SDLState;
struct GameData;
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
        //void draw(const SDLState &state, GameData &gs, float width, float height);
        
        Player(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex, std::vector<Animation> anims, int curAnim, float maxSpeedX_) :
        AnimatedObject(pos_, colliderRect, tex) {
            acc = glm::vec2(300, 0); // default for now
            animations = anims;
            curAnimation = curAnim;

            grounded = false;
            sprinting = false;
            maxSpeedX = maxSpeedX_; // should be walk speed
            gravityScale = 1.0f;
            
        }
        Player(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) : // generic obj constructor
        AnimatedObject(pos_, colliderRect, tex) {
            grounded = false;
            sprinting = false;
            gravityScale = 1.0f;
            maxSpeedX = 250; // walk speed default
        }

        Player() : AnimatedObject() {
            grounded = false;
            gravityScale = 1.0f;
            maxSpeedX = 250; // walk speed default
        }
    private:
        PlayerState* state_;
};
