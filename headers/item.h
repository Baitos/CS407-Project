#pragma once
#include "../headers/object.h"
#include "../headers/player.h"

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

void clearItem(const SDLState &state, GameData &gd, Resources &res);
void setItemPicked(GameData &gd, Resources &res, int index);
class Item : public AnimatedObject {
    public:
        bool active;
        int index;
        itemType type;
        virtual ~Item() = default;
        virtual void draw(const SDLState &state, GameData &gd, float width, float height) {}
        virtual void update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {}
        virtual void checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {} // do nothing
        virtual void useItem(const SDLState &state, GameData &gd, Resources &res, Player &p) {} // do nothing

        Item(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
        AnimatedObject(pos_, colliderRect, tex) {
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
        void useItem(const SDLState &state, GameData &gd, Resources &res, Player &p);
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        Bomb(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
        Item(pos_, colliderRect, tex) {
            index = 0;
        }
        
};

class Boombox : public Item {
    public:
        void useItem(const SDLState &state, GameData &gd, Resources &res, Player &p);
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        Boombox(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
        Item(pos_, colliderRect, tex) {
            index = 1;
        }
};

class Ice : public Item {
    public:
        int BOUND; // how big is effect
        Timer iceTimer; // lifetime of obj
        Timer flipTimer; // timer to flip particle
        
        void useItem(const SDLState &state, GameData &gd, Resources &res, Player &p);
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        Ice(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
        Item(pos_, colliderRect, tex), iceTimer(30.0f), flipTimer(0.2f) {
            index = 4;
            BOUND = 5;
            persistent = true;
        }
};

class Sugar : public Item {
    public:
        Timer sugarTimer;
        AnimatedObject sugarEffectObject;

        void useItem(const SDLState &state, GameData &gd, Resources &res, Player &p);
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {} // do nothing
        Sugar(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
        Item(pos_, colliderRect, tex), sugarTimer(4.0f) {
            index = 6;
        }
};

class Pie : public Item {
    public:   
        void useItem(const SDLState &state, GameData &gd, Resources &res, Player &p);
        void draw(const SDLState &state, GameData &gd, float width, float height);
        void update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        Pie(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
        Item(pos_, colliderRect, tex) {
            index = 7;
        }
};


class ItemStorage : public AnimatedObject {
    public:
    Item* boxItem;
    Timer cycleTimer;
    ItemStorage() : AnimatedObject(), cycleTimer(2.0f) {

    }
    ItemStorage(glm::vec2 pos_, SDL_FRect colliderRect, SDL_Texture *tex) :
    AnimatedObject(pos_, colliderRect, tex), cycleTimer(2.0f) {

    }
    void update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
};