#pragma once
#include "../headers/object.h"
#include "../headers/player.h"
enum class itemType {
    BOOMBOX,
    MISSILE,
    SUGAR,
    PIE,
    BOMB
};
class Item : public AnimatedObject {
    public:
    itemType type;
    SDL_FRect collider;
    Item() : AnimatedObject(){}
    Item (glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    AnimatedObject(pos_, colliderRect, tex) {}
    virtual void useItem(const SDLState &state, GameData &gd, float width, float height);
    virtual void onCollision();
};

class Bomb : Item {
    void useItem(const SDLState &state, GameData &gd, float width, float height) {
        
        this->draw(state, gd, width, height)
    }
}