#pragma once
#include "object.h"
#include "player.h"
#include "resources.h"
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

void setNoEffect(GameData &gd, Resources &res, AnimatedObject obj);

void clearItem(const SDLState &state, GameData &gd, Resources &res);
void setItemPicked(GameData &gd, Resources &res);

class Item : public AnimatedObject {
    public:
    int ownerIndex = 0; // player index
    int index = 0;
    itemType type;
    bool deleteOnCollision = false;
    bool persistsOnCollision; // bomb
    Timer sugarTimer;
    Item() : AnimatedObject(), sugarTimer(3.0f) {}
    Item(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    AnimatedObject(pos_, colliderRect, tex), sugarTimer(4.0f) {}

    void (*useItem)(const SDLState &state, GameData &gd, Resources &res);
    void (*setEffect)(GameData &gd, Resources &res, AnimatedObject obj) = setNoEffect;
    void (*onCollision)(AnimatedObject &obj, GameData &gd, Resources &res, Player &player);
};

class Bomb : public Item {
    public:
    Bomb(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    Item(pos_, colliderRect, tex) {
        deleteOnCollision = true;
        type = itemType::BOMB;
        index = 0;
        this->useItem = useBomb;
        this->onCollision = angledStun;
        this->knockbackMultiplier = 5.0f;
        this->setEffect = effectExplosion;
    }
};

class Boombox : public Item {
    public:
    Boombox(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    Item(pos_, colliderRect, tex) {
        width = 196;
        height = 196;
        index = 1;
        type = itemType::BOOMBOX;
        this->useItem = useBoombox;
        this->onCollision = angledStun;
        this->knockbackMultiplier = 3.0f;
    }
    void update(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
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
    void update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
};
