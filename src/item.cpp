#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/helper.h"
#include "../headers/collision.h"
#include "../headers/playerState.h"

// GENERIC ITEM FUNCTIONS

// BOMB
void Bomb::draw(const SDLState &state, GameData &gd, float width, float height) {
    AnimatedObject::draw(state, gd, width, height); // do generic object draw
}

void Bomb::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // currently for pie
    printf("boombox update\n");
}

void Bomb::useItem(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    printf("bomb used\n");
}

void Bomb::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    printf("bomb collide\n");
}

// BOOMBOX
void Boombox::draw(const SDLState &state, GameData &gd, float width, float height) {
    AnimatedObject::draw(state, gd, width, height); // do generic object draw
}

void Boombox::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // currently for pie
    printf("boombox update\n");
}
void Boombox::useItem(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    printf("boombox used\n");
}

void Boombox::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    printf("boombox collide\n");
}

// ICE
void Ice::draw(const SDLState &state, GameData &gd, float width, float height) {
    AnimatedObject::draw(state, gd, this->BOUND * width, this->BOUND * height); // do generic object draw
} 
// right now "persistent" is for this ice block; because it is very large it interacts weird with "isOnscreen"

void Ice::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // currently for pie
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
void Ice::useItem(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    SDL_FRect iceCollider = {
        .x = 0,
        .y = 0,
        .w = (float)TILE_SIZE * this->BOUND,
        .h = (float)TILE_SIZE * this->BOUND
    };
    int OFFSET = (this->BOUND - 1) / 2;
    glm::vec2 iceEffectPos(p.pos.x - OFFSET * TILE_SIZE, p.pos.y - OFFSET * TILE_SIZE);
    
    Ice* ice = new Ice(iceEffectPos, iceCollider, res.texIce);
    p.items_.push_back(ice);
}

// PIE
void Pie::draw(const SDLState &state, GameData &gd, float width, float height) {
    AnimatedObject::draw(state, gd, width, height); // do generic object draw
}

void Pie::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // currently for pie
    //Item::update(state, gd, res, p, deltaTime); // generic update
    (*this).pos += updatePos((*this), deltaTime);
    //printf("pie update\n");
}

void Pie::useItem(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    SDL_FRect pieCollider = {
        .x = 6,
        .y = 3,
        .w = 20,
        .h = 26
    };
    Pie* pie = new Pie(p.pos, pieCollider, res.texPie);
    pie->dir = p.dir;
    pie->vel.x = 350.0f * pie->dir;
    p.items_.push_back(pie);
}

// SUGAR
void Sugar::draw(const SDLState &state, GameData &gd, float width, float height) {
    for (Player &p : gd.players_) {
        if (p.usingSugar) {
            //Draw sugar effect
            this->sugarEffectObject.draw(state, gd, TILE_SIZE, TILE_SIZE);
        }
    }
}

void Sugar::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
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

void Sugar::useItem(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    this->sugarTimer.reset();
    p.usingSugar = true;
    SDL_FRect collider = {
        .x = 0,
        .y = 0,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };
    Sugar* sugar = new Sugar(p.pos, collider, res.texSugar);
    sugar->sugarEffectObject = AnimatedObject(this->pos, collider, res.texSugarEffect);
    sugar->sugarEffectObject.debug = false;
    sugar->visible = false;
    p.items_.push_back(sugar); 
}

void useBouncyBall(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useFog(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useIce(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useMissile(const SDLState &state, GameData &gd, Resources &res, Player &p) {}

void clearItem(const SDLState &state, GameData &gd, Resources &res) {
    gd.itemStorage_.texture = res.texItemStorage;
    gd.itemStorage_.curAnimation = res.ANIM_ITEM_EMPTY;
    gd.itemStorage_.animations[gd.itemStorage_.curAnimation].reset();
    
}

void setItemPicked(GameData &gd, Resources &res, int index) {
        gd.itemStorage_.texture = res.itemTextures[index];
		gd.itemStorage_.curAnimation = res.ANIM_ITEM_PICKED;
        gd.itemStorage_.animations[gd.itemStorage_.curAnimation].reset();
}

void ItemStorage::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    if (this->curAnimation != -1) {
        this->animations[this->curAnimation].step(deltaTime);
    }
    this->cycleTimer.step(deltaTime);
    if (this->cycleTimer.isTimeOut()) {
        // Reset timer and set item
        this->cycleTimer.reset();
        this->boxItem = p.heldItem;
        p.pickingItem = false;
        p.hasItem = true;

        setItemPicked(gd, res, p.heldItem->index);
    }
}