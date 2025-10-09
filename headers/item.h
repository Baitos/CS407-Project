#pragma once
#include "../headers/object.h"
#include "../headers/player.h"

enum class itemType {
    BOMB,
    BOOMBOX,
    BOUNCYBALL,
    FOG,
    ICE,
    MISSILE,
    SUGAR,
    PIE
};
void useBomb(const SDLState &state, GameData &gd, Resources &res);
void useBoombox(const SDLState &state, GameData &gd, Resources &res);
void useBouncyBall(const SDLState &state, GameData &gd, Resources &res);
void useFog(const SDLState &state, GameData &gd, Resources &res);
void useIce(const SDLState &state, GameData &gd, Resources &res);
void useMissile(const SDLState &state, GameData &gd, Resources &res);
void useSugar(const SDLState &state, GameData &gd, Resources &res);
void usePie(const SDLState &state, GameData &gd, Resources &res);

void clearItem(const SDLState &state, GameData &gd, Resources &res);
void setItemPicked(GameData &gd, Resources &res);
class Item : public AnimatedObject {
    public:
    int index = 0;
    itemType type;
    Timer sugarTimer;
    Item() : AnimatedObject(), sugarTimer(3.0f) {}
    Item(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    AnimatedObject(pos_, colliderRect, tex), sugarTimer(4.0f) {}
    void (*useItem)(const SDLState &state, GameData &gd, Resources &res);
    //virtual void onCollision();
    
};

class Bomb : public Item {
    public:
    Bomb(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    Item(pos_, colliderRect, tex) {
        index = 0;
        this->useItem = useBomb;
    }
};

class Boombox : public Item {
    public:
    Boombox(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    Item(pos_, colliderRect, tex) {
        index = 1;
        this->useItem = useBoombox;
    }
};

class Sugar : public Item {
    public:
    
    Sugar(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    Item(pos_, colliderRect, tex) {
        index = 6;
        this->useItem = useSugar;
    }
};
class ItemStorage : public AnimatedObject {
    public:
    Item item;
    Timer cycleTimer;
    ItemStorage() : AnimatedObject(), cycleTimer(2.0f){}
    ItemStorage(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    AnimatedObject(pos_, colliderRect, tex), cycleTimer(2.0f) {}
    void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
};