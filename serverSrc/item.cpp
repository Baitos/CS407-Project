#include "../serverHeaders/gameData.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/playerState.h"

// GENERIC ITEM FUNCTIONS

// BOMB

void Bomb::update(const SDLState &state, GameData &gd,  Player &p, float deltaTime) { 
    if (this->exploded) {
        this->active = false;
        
    }
}

void Bomb::useItem(const SDLState &state, GameData &gd, Player &p) {
    SDL_FRect bombCollider = {
        .x = 0,
        .y = 0,
        .w = 32,
        .h = 32
    };
    glm::vec2 bombPos;
    int xdiff;
    // Set item 1 tile behind player, but we want it to be drawn at the beginning of tile
    if (p.dir >= 0) { // moving right, place to left
        xdiff = 32 + (int)p.pos.x % 32;
        bombPos.x = p.pos.x - xdiff;
    }
    else {
        xdiff = 32 + (32- (int)p.pos.x % 32);
        bombPos.x = p.pos.x + xdiff;
    }
    bombPos.y = p.pos.y;
    Bomb* bomb = new Bomb(bombPos, bombCollider);
    p.items_.push_back(bomb);
}

// BOOMBOX
void Boombox::update(const SDLState &state, GameData &gd, Player &p, float deltaTime) {
    this->pos.x = p.pos.x - 82;
    this->pos.y = p.pos.y - 82;
    
    this->active = false;
    
}
void Boombox::useItem(const SDLState &state, GameData &gd, Player &p) {
        SDL_FRect soundCollider = {
        .x = 0,
        .y = 0,
        .w = 196,
        .h = 196
    };
    soundCollider;
    Boombox * boombox = new Boombox(p.pos, soundCollider);
    p.items_.push_back(boombox);
}


// ICE

// right now "persistent" is for this ice block; because it is very large it interacts weird with "isOnscreen"

void Ice::update(const SDLState &state, GameData &gd, Player &p, float deltaTime) { // currently for pie
    this->iceTimer.step(deltaTime);
    if (this->iceTimer.isTimeOut()) { // kill ice obj
        this->active = false;
    }
    this->flipTimer.step(deltaTime);
    if (this->flipTimer.isTimeOut()) { // animation
        this->dir = -1 * this->dir;
        this->flipTimer.reset();
    }
}
void Ice::useItem(const SDLState &state, GameData &gd, Player &p) {
    SDL_FRect iceCollider = {
        .x = 0,
        .y = 0,
        .w = (float)TILE_SIZE * this->BOUND,
        .h = (float)TILE_SIZE * this->BOUND
    };
    int OFFSET = (this->BOUND - 1) / 2;
    glm::vec2 iceEffectPos(p.pos.x - OFFSET * TILE_SIZE, p.pos.y - OFFSET * TILE_SIZE);
    
    Ice* ice = new Ice(iceEffectPos, iceCollider);
    p.items_.push_back(ice);
}

// PIE

void Pie::update(const SDLState &state, GameData &gd, Player &p, float deltaTime) { // currently for pie
    //Item::update(state, gd, res, p, deltaTime); // generic update
    this->pos += updatePos((*this), deltaTime);
    //printf("pie update\n");
}

void Pie::useItem(const SDLState &state, GameData &gd, Player &p) {
    SDL_FRect pieCollider = {
        .x = 6,
        .y = 3,
        .w = 20,
        .h = 26
    };
    Pie* pie = new Pie(p.pos, pieCollider);
    pie->dir = p.dir;
    pie->vel.x = 350.0f * pie->dir;
    p.items_.push_back(pie);
}


// SUGAR
void Sugar::update(const SDLState &state, GameData &gd, Player &p, float deltaTime) {
    if (p.usingSugar) {
        this->sugarTimer.step(deltaTime);
        p.vel.x += 500.0f * p.currentDirection * deltaTime;
        if(this->sugarTimer.isTimeOut()){
            //printf("Stopped sugar\n");
            p.usingSugar = false;
            this->active = false;
        }
        this->sugarEffectObject; // draw sugar effect
        this->sugarEffectObject.dir = p.dir;
        if (this->sugarEffectObject.dir == 1) {
            this->sugarEffectObject.pos = glm::vec2(p.pos.x - 30.f, p.pos.y);
        } else if (this->sugarEffectObject.dir == -1) {
            this->sugarEffectObject.pos = glm::vec2(p.pos.x + 30.f, p.pos.y);
        }
    }
}

void Sugar::useItem(const SDLState &state, GameData &gd, Player &p) {
    this->sugarTimer.reset();
    p.usingSugar = true;
    SDL_FRect collider = {
        .x = 0,
        .y = 0,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };
    Sugar* sugar = new Sugar(p.pos, collider);
    sugar->sugarEffectObject = AnimatedObject(this->pos, collider);
    sugar->sugarEffectObject.debug = false;
    sugar->visible = false;
    p.items_.push_back(sugar); 
}

void useBouncyBall(const SDLState &state, GameData &gd, Player &p) {}
void useFog(const SDLState &state, GameData &gd, Player &p) {}
void useIce(const SDLState &state, GameData &gd, Player &p) {}
void useMissile(const SDLState &state, GameData &gd, Player &p) {}

void clearItem(const SDLState &state, GameData &gd) {

    
}

void setItemPicked(GameData &gd, int index) {
        
}

void ItemStorage::update(const SDLState &state, GameData &gd, Player &p, float deltaTime) {
    
    
    if (this->cycleTimer.isTimeOut()) {
        // Reset timer and set item
        this->cycleTimer.reset();
        this->boxItem = p.heldItem;
        p.pickingItem = false;
        p.hasItem = true;

        setItemPicked(gd, p.heldItem->index);
    }
}

