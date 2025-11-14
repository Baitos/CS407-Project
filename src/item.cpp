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

void Bomb::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { 
    if (this->exploded) {
        if (this->curAnimation != -1) {
            this->animations[this->curAnimation].step(deltaTime);
        }
        if (this->animations[this->curAnimation].isDone()) {
            this->active = false;
        }
    }
}

void Bomb::useItem(const SDLState &state, GameData &gd, Resources &res, Player &p) {
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
    Bomb* bomb = new Bomb(bombPos, bombCollider, res.texBomb);
    p.items_.push_back(bomb);
}

// BOOMBOX
void Boombox::draw(const SDLState &state, GameData &gd, float width, float height) {
    AnimatedObject::draw(state, gd, width, height); // do generic object draw
}

void Boombox::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    this->pos.x = p.pos.x - 82;
    this->pos.y = p.pos.y - 82;
    this->animations[this->curAnimation].step(deltaTime);
    if (this->animations[this->curAnimation].isDone()) {
        this->active = false;
    }
}
void Boombox::useItem(const SDLState &state, GameData &gd, Resources &res, Player &p) {
        SDL_FRect soundCollider = {
        .x = 0,
        .y = 0,
        .w = 196,
        .h = 196
    };
    soundCollider;
    Boombox * boombox = new Boombox(p.pos, soundCollider, res.texSoundwaves);
    boombox->animations = res.itemAnims;
    boombox->curAnimation = res.ANIM_ITEM_SOUNDWAVE;
    boombox->animations[boombox->curAnimation].reset();
    p.items_.push_back(boombox);
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
    this->pos += updatePos((*this), deltaTime);
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

void Fog::draw(const SDLState &state, GameData &gd, float width, float height) {
    if (gd.players_[gd.playerIndex].hasFog) {
        //Draw sugar effect
        //printf("has fog\n");
        this->pos = glm::vec2(gd.mapViewport.x, gd.mapViewport.y);
        AnimatedObject::draw(state, gd, gd.mapViewport.w, gd.mapViewport.h);
        
    }
    
}

void Fog::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    if (gd.players_[gd.playerIndex].hasFog) {
        //this->pos = glm::vec2(gd.mapViewport.x, gd.mapViewport.y);
        this->fogTimer.step(deltaTime);
        if(this->fogTimer.getTime() < 5.f){
            if (this->animations[curAnimation].currentFrame() != 6) {
                this->animations[this->curAnimation].step(deltaTime);
            } 
        }
        
        if(this->fogTimer.getTime() >= 8.f){
            this->texture = res.texFogExit;
            if(!this->reset){
               this->animations[curAnimation].reset();
               this->reset = true; 
            }
            if (this->animations[curAnimation].currentFrame() != 6) {
                this->animations[this->curAnimation].step(deltaTime);
            } 
        }

        
        if(this->fogTimer.isTimeOut()){
            //printf("Stopped sugar\n");
            p.hasFog = false;
            this->active = false;
        }
        
    }
}

void Fog::useItem(const SDLState &state, GameData &gd, Resources &res, Player &p){
    this->fogTimer.reset();
    for(int i = 0; i < gd.players_.size(); i++){
        if(i != gd.playerIndex){
            gd.players_[i].hasFog = true;
            SDL_FRect collider = {
                .x = 0,
                .y = 0,
                .w = 0,
                .h = 0
            };
            glm::vec2 pos = glm::vec2(gd.mapViewport.x, gd.mapViewport.y);
            Fog* fog = new Fog(pos, collider, res.texFog);
            fog->animations = res.itemAnims;
            fog->curAnimation = res.ANIM_ITEM_FOG;
            
            gd.players_[i].items_.push_back(fog); 
            }
        }
}

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

