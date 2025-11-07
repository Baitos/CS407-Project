#include <SDL3/SDL.h>
#include <cstdio>
#include "../serverHeaders/player.h"
#include "../serverHeaders/playerState.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/item.h"


void AnimatedObject::update(const SDLState &state, GameData &gd, float deltaTime) { // just step the anims
    
}

void Laser::update(const SDLState &state, GameData &gd,float deltaTime) { // update laser timer every frame
    Timer &laserTimer = this->laserTimer;
    laserTimer.step(deltaTime);
    if (laserTimer.isTimeOut()){
        //Resets the timer and switches tHe LaSeR
        laserTimer.reset();
        this->laserActive = !this->laserActive;
    }
}

// void Hook::draw(const SDLState &state, GameData &gd, Player &p, float width, float height) {
//     if (!this->visible) {
//         return;
//     }
//     SDL_SetRenderDrawColor(state.renderer, 30, 30, 30, 255); // draw line to hook
//     glm::vec2 pOffset = findCenterOfSprite(p);
//     glm::vec2 hOffset = findCenterOfSprite((*this));
//     //printf("x: %f, y: %f\n", gd.mouseCoords.x, gd.mouseCoords.y);
//     SDL_RenderLine(state.renderer, p.pos.x - gd.mapViewport.x + pOffset.x, 
//                     p.pos.y - gd.mapViewport.y + pOffset.y, 
//                     this->pos.x - gd.mapViewport.x + hOffset.x, 
//                     this->pos.y - gd.mapViewport.y + hOffset.y);
//     SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
//     if (!isOnscreen(state, gd, (*this))) {
//         return;
//     }
//     SDL_FRect dst {
//         .x = this->pos.x - gd.mapViewport.x,
//         .y = this->pos.y - gd.mapViewport.y,
//         .w = width,
//         .h = height
//     };
//     double angle = 0; // default texture faces right, sdl_render rotates clockwise
//     if (this->vel.y > 0 && std::abs(this->vel.y) > std::abs(this->vel.x)) {
//         angle = 90; // down
//     }
//     else if (this->vel.x < 0 && std::abs(this->vel.x) > std::abs(this->vel.y)) {
//         angle = 180; // left
//     }
//     else if (this->vel.y < 0 && std::abs(this->vel.y) > std::abs(this->vel.x)) {
//         angle = 270; // up
//     }
//     SDL_RenderTextureRotated(state.renderer, this->texture, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE); // this could probably be done using the flip variable but there's no way to rotate the image that way
//     this->drawDebug(state, gd, width, height);
// }

void Hook::update(const SDLState &state, GameData &gd, float deltaTime) {
    this->pos += updatePos((*this), deltaTime);
}

void ItemBox::update(const SDLState &state, GameData &gd, float deltaTime) { // update item box timer every frame (when on cooldown)
    Timer &itemBoxTimer = this->itemBoxTimer;
    itemBoxTimer.step(deltaTime);
    if (itemBoxTimer.isTimeOut()) {
        // Reset timer and reactivate item box
        itemBoxTimer.reset();
        this->itemBoxActive = true;
    }
}

void ItemBox::generateItem(Player &player, GameData &gd) {
    SDL_FRect defaultCollider = {
        .x = 0,
        .y = 0,
        .w = float(TILE_SIZE),
        .h = float(TILE_SIZE)
    };
    Item* newItem;
    std::vector<itemType> itemOptions;
    int selected;
    // // Limit items for top 25% of players
    // if ((player.position / (float)gd.numPlayers) <= 0.25) {
    //     itemOptions = {itemType::BOMB, itemType::BOOMBOX, itemType::BOUNCYBALL, 
    //         itemType::ICE, itemType::SUGAR, itemType::PIE};
    // }
    // // Limit items for bottom 25% of players
    // else if ((player.position / (float)gd.numPlayers) >= 0.75) {
    //     itemOptions = {itemType::BOOMBOX, itemType::BOUNCYBALL, 
    //         itemType::FOG, itemType::MISSILE, itemType::SUGAR, itemType::PIE};
    // }
    // else {
        // All items are available
        itemOptions = {itemType::BOMB, itemType::BOOMBOX, itemType::BOUNCYBALL, 
            itemType::FOG, itemType::ICE, itemType::MISSILE, itemType::SUGAR, itemType::PIE};
    //}
    selected = rand() % itemOptions.size();



    switch (itemOptions[selected]) {
        case itemType::BOMB:
            newItem = new Bomb(player.pos, defaultCollider);
            break;
        case itemType::BOOMBOX:
            newItem = new Boombox(player.pos, defaultCollider);
            break;
        case itemType::SUGAR:
            newItem = new Sugar(player.pos, defaultCollider);
            break;
        case itemType::PIE:
            newItem = new PieItem(player.pos, defaultCollider);
            break;
        case itemType::ICE:
            newItem = new Ice(player.pos, defaultCollider);
            break;
        default:
            printf("Your item is in another castle\n");
            newItem = new Ice(player.pos, defaultCollider);
            break;
    }
    
    if (player.heldItem != nullptr) {
        delete player.heldItem;
    }
    player.heldItem = newItem;
    player.pickingItem = true;
}


// void angledStun(AnimatedObject &obj, GameData &gd, Player &player) {
//     // TODO properly implement angles
//     // if (player.index == 1) {
//     //     gd.currPlayer = player;
//     // }
//     float baseKnockback = 100.0f;
//     player.state_ = changePlayerState(gd, player.state_, DEAD);
//     if (player.vel.x > 0) {
//         player.vel.x = changeVel(baseKnockback * obj.knockbackMultiplier, player);
//     }
//     else if (player.vel.x < 0) {
//         player.vel.x = changeVel(-baseKnockback* obj.knockbackMultiplier, player);
//     }
//     player.vel.x = changeVel(-player.vel.x, player);
//     float shouldFlip = player.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
//     if(shouldFlip * obj.pos.y < shouldFlip * player.pos.y ){
//         player.vel.y = changeVel(baseKnockback * obj.knockbackMultiplier, player);
//     } else {
//         player.vel.y = changeVel(-baseKnockback* obj.knockbackMultiplier, player);
//     }
//     player.gravityScale = 1.0f;
//     player.isStunned = true;
//     player.grounded = false;
//     // TODO rework this so it's generic
//     if (player.index == 0) {
//         gd.player = player;
//     }
//     else {
//         gd.player2 = player;
//     }
//     gd.currPlayer = gd.player;
// }

