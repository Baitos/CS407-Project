#pragma once
#include "../serverHeaders/object.h"
#include "player.h"

enum itemType {
    BOMB,
    BOOMBOX,
    BOUNCYBALL,
    FOG,
    ICE,
    MISSILE,
    SUGAR,
    PIE,
    NOITEM
};
/*void useBomb(const SDLState &state, GameData &gd, Resources &res, Player &p);
void useBoombox(const SDLState &state, GameData &gd, Resources &res, Player &p);
void useBouncyBall(const SDLState &state, GameData &gd, Resources &res, Player &p);
void useFog(const SDLState &state, GameData &gd, Resources &res, Player &p);
void useIce(const SDLState &state, GameData &gd, Resources &res, Player &p);
void useMissile(const SDLState &state, GameData &gd, Resources &res, Player &p);
void useSugar(const SDLState &state, GameData &gd, Resources &res, Player &p);
void usePie(const SDLState &state, GameData &gd, Resources &res, Player &p);*/

void clearItem(const SDLState &state, GameData &gd);
void setItemPicked(GameData &gd, int index);
class Item : public AnimatedObject {
    public:
        bool active;
        int index;
        itemType type;
        virtual ~Item() = default;
        virtual void update(const SDLState &state, GameData &gd, Player &p, float deltaTime) {}
        virtual void checkCollision(const SDLState &state, GameData &gd, Player &p, float deltaTime) {} // do nothing
        virtual void useItem(const SDLState &state, GameData &gd, Player &p) {} // do nothing

        Item(glm::vec2 pos_, SDL_FRect colliderRect) :
        AnimatedObject(pos_, colliderRect) {
            type = NOITEM; // error
            active = true;
            index = -1;
        }
        Item() : AnimatedObject() {
            type = NOITEM; // error
            active = true;
            index = -1;
        }
};

class Bomb : public Item {
    public:
        bool exploded = false; 
        void useItem(const SDLState &state, GameData &gd, Player &p);
        void update(const SDLState &state, GameData &gd, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Player &p, float deltaTime);
        Bomb(glm::vec2 pos_, SDL_FRect colliderRect) :
        Item(pos_, colliderRect) {
            type = BOMB;
            index = 0;
            width = 32;
            height = 32;
        }
        
};

class Boombox : public Item {
    public:
        void useItem(const SDLState &state, GameData &gd, Player &p);
        void update(const SDLState &state, GameData &gd, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Player &p, float deltaTime);
        Boombox(glm::vec2 pos_, SDL_FRect colliderRect) :
        Item(pos_, colliderRect) {
            type = BOOMBOX;
            index = 1;
            width = 196;
            height = 196;
        }
};

class Ice : public Item {
    public:
        int BOUND; // how big is effect
        Timer iceTimer; // lifetime of obj
        Timer flipTimer; // timer to flip particle
        
        void useItem(const SDLState &state, GameData &gd, Player &p);
        void update(const SDLState &state, GameData &gd,  Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Player &p, float deltaTime);
        Ice(glm::vec2 pos_, SDL_FRect colliderRect) :
        Item(pos_, colliderRect), iceTimer(10.0f), flipTimer(0.2f) {
            type = ICE;
            index = 4;
            BOUND = 5;
            width = 32;
            height = 32;
            persistent = true;
        }
};

class Sugar : public Item {
    public:
        Timer sugarTimer;
        AnimatedObject sugarEffectObject;

        void useItem(const SDLState &state, GameData &gd, Player &p);
        void update(const SDLState &state, GameData &gd, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Player &p, float deltaTime) {} // do nothing
        Sugar(glm::vec2 pos_, SDL_FRect colliderRect) :
        Item(pos_, colliderRect), sugarTimer(4.0f) {
            type = SUGAR;
            index = 6;
        }
};

class PieItem : public Item {
    public:   
        void useItem(const SDLState &state, GameData &gd,  Player &p);
        void update(const SDLState &state, GameData &gd, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd,  Player &p, float deltaTime);
        PieItem(glm::vec2 pos_, SDL_FRect colliderRect) :
        Item(pos_, colliderRect) {
            type = PIE;
            width = 32;
            height = 32;
            index = 7;
        }
};


class ItemStorage : public AnimatedObject {
    public:
    Item* boxItem;
    Timer cycleTimer;
    ItemStorage() : AnimatedObject(), cycleTimer(3.0f) {

    }
    ItemStorage(glm::vec2 pos_, SDL_FRect colliderRect) :
    AnimatedObject(pos_, colliderRect), cycleTimer(3.0f) { 
 
    }
    void update(const SDLState &state, GameData &gd, Player &p, float deltaTime);
};
