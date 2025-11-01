#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/helper.h"
#include "../headers/collision.h"
#include "../headers/playerState.h"

void useBomb(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    printf("BOOOOOOM\n");
}

void useBoombox(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    printf("BUMP THAT SHIT\n");
}
void useBouncyBall(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useFog(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useIce(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useMissile(const SDLState &state, GameData &gd, Resources &res, Player &p) {}

void useSugar(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    ((Sugar *) (&p.item))->sugarTimer.reset();
    p.usingSugar = true;
    //printf("COKE\n");
}
void usePie(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    SDL_FRect pieCollider = {
        .x = 6,
        .y = 3,
        .w = 20,
        .h = 26
    };
    Item pie(p.pos, pieCollider, res.texPie);
    pie.dir = p.dir;
    pie.vel.x = 300.0f * pie.dir;
    p.items_.push_back(pie);
}


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

void Item::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    (*this).pos += updatePos((*this), deltaTime);
}

void Item::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // currently only for pie
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
            this->visible = false;
            this->vel = glm::vec2(0);
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

                this->visible = false;
                this->vel = glm::vec2(0);
            }
        } 
    }
}

void ItemStorage::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    if (this->curAnimation != -1) {
        this->animations[this->curAnimation].step(deltaTime);
    }
    this->cycleTimer.step(deltaTime);
    if (this->cycleTimer.isTimeOut()) {
        // Reset timer and set item
        this->cycleTimer.reset();
        Item item = p.nextItem;
        this->item = item;
        p.item = item;
        p.pickingItem = false;
        p.hasItem = true;

        setItemPicked(gd, res, item.index);
    }
}