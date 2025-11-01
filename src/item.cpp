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

void Pie::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    SDL_FRect rectA{
		.x = (*this).pos.x + (*this).collider.x,
		.y = (*this).pos.y + (*this).collider.y,
		.w = (*this).collider.w,
		.h = (*this).collider.h
	};
    SDL_FRect rectB;
    glm::vec2 resolution{ 0 };
    for (Level &l : gd.mapTiles_){
		rectB = {
            .x = l.pos.x + l.collider.x,
            .y = l.pos.y + l.collider.y,
            .w = l.collider.w,
            .h = l.collider.h
	    };
        if (intersectAABB(rectA, rectB, resolution))
	    {
            this->active = false;
        }
    }
    for (Player &p2 : gd.players_) {
        if (&p != &p2) { // do not check on self
            rectB = {
                .x = p2.pos.x + p2.collider.x,
                .y = p2.pos.y + p2.collider.y,
                .w = p2.collider.w,
                .h = p2.collider.h
            };
            if (intersectAABB(rectA, rectB, resolution) && (*this).visible) {
                p2.vel.y = -200.0f; 
                p2.vel.x = this->vel.x * 0.5;  
                p2.pos.y -= 1;              
                p2.grounded = false;

                PlayerState* stunState = new StunnedState(true); // hard stun
                p2.handleState(stunState, gd, res); // stun player you hit

                this->active = false;
            }
        } 
    }
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
    // if (p.dir == 1) {
    //     glm::vec2 pos = glm::vec2(p.pos.x - 30.f, p.pos.y);
    //     SDL_FRect collider = {
    //         .x = 28 * (p.dir),
    //         .y = 0,
    //         .w = 0.f,
    //         .h = 0.f
    //         };
    // } else {
    //     glm::vec2 pos = glm::vec2(p.pos.x + 30.f, p.pos.y);
    //     SDL_FRect collider = {
    //         .x = 28 * (p.dir),
    //         .y = 0,
    //         .w = 0.f,
    //         .h = 0.f
    //         };
    //     this->sugarEffectObject(pos, collider, res.texSugarEffectR);
    // }

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