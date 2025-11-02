#include <SDL3/SDL.h>
#include <cstdio>
#include "../serverHeaders/player.h"
#include "../serverHeaders/playerState.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/item.h"
#include "../serverHeaders/resources.h"

void AnimatedObject::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) { // just step the anims
    if (this->curAnimation != -1) {
        this->animations[this->curAnimation].step(deltaTime);
    }
}


void Laser::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) { // update laser timer every frame
    Timer &laserTimer = this->laserTimer;
    laserTimer.step(deltaTime);
    if (laserTimer.isTimeOut()){
        //Resets the timer and switches tHe LaSeR
        laserTimer.reset();
        this->laserActive = !this->laserActive;
    }
}

void Hook::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    (*this).pos += updatePos((*this), deltaTime);
}

void Hook::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
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
            (*this).vel = glm::vec2(0);
            if (!(*this).collided) {
                PlayerState* grappleState = new GrappleState();
                p.handleState(grappleState, gd, res);
                (*this).collided = true;
            }
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
            if (intersectAABB(rectA, rectB, resolution) && !(*this).collided) {
                p.vel = 0.7f * (*this).vel;
                p2.vel = -0.3f * (*this).vel;
                removeHook(p);
                removeHook(p2);
                PlayerState* stunState = new StunnedState();
                p2.handleState(stunState, gd, res); // stun player you hit and disable their hook
            }
            Hook h2 = p2.hook;
            rectB = {
                .x = h2.pos.x + h2.collider.x,
                .y = h2.pos.y + h2.collider.y,
                .w = h2.collider.w,
                .h = h2.collider.h
            };
            if (intersectAABB(rectA, rectB, resolution) && !(*this).collided && h2.visible) { // Touching other hook
                removeHook(p);
                removeHook(p2);
            }
        } 
    }
    
}

void ItemBox::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) { // update item box timer every frame (when on cooldown)
    Timer &itemBoxTimer = this->itemBoxTimer;
    itemBoxTimer.step(deltaTime);
    if (itemBoxTimer.isTimeOut()) {
        // Reset timer and reactivate item box
        itemBoxTimer.reset();
        this->itemBoxActive = true;
    }
}

void ItemBox::generateItem(Player &player, GameData &gd, Resources &res) {
    SDL_FRect defaultCollider = {
        .x = 0,
        .y = 0,
        .w = float(TILE_SIZE),
        .h = float(TILE_SIZE)
    };
    Item newItem;
    std::vector<itemType> itemOptions;
    int selected;
    // Limit items for top 25% of players
    if ((player.position / (float)gd.numPlayers) <= 0.25) {
        itemOptions = {itemType::BOMB, itemType::BOOMBOX, itemType::BOUNCYBALL, 
            itemType::ICE, itemType::SUGAR, itemType::PIE};
    }
    // Limit items for bottom 25% of players
    else if ((player.position / (float)gd.numPlayers) >= 0.75) {
        itemOptions = {itemType::BOOMBOX, itemType::BOUNCYBALL, 
            itemType::FOG, itemType::MISSILE, itemType::SUGAR, itemType::PIE};
    }
    else {
        // All items are available
        itemOptions = {itemType::BOMB, itemType::BOOMBOX, itemType::BOUNCYBALL, 
            itemType::FOG, itemType::ICE, itemType::MISSILE, itemType::SUGAR, itemType::PIE};
    }
    selected = rand() % itemOptions.size();
    //printf("%d %d\n", selected, itemOptions.size());
    
    switch (itemOptions[selected]) {
        case itemType::BOMB:
            newItem = Bomb(player.pos, defaultCollider, res.texBomb);
            break;
        case itemType::BOOMBOX:
            newItem = Boombox(player.pos, defaultCollider, res.texBoombox);
            break;
        case itemType::SUGAR:
            newItem = Sugar(player.pos, defaultCollider, res.texSugar);
            break;
        default:
            printf("Your item is in another castle\n");
            newItem = Bomb(player.pos, defaultCollider, res.texBomb);
    }
    player.nextItem = newItem;
    player.pickingItem = true;
}