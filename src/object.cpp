#include <SDL3/SDL.h>
#include <cstdio>
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/helper.h"
#include "../headers/item.h"
#include "../headers/resources.h"
void Object::draw(const SDLState &state, GameData &gd, float width, float height) {
    if (!isOnscreen(state, gd, (*this))) {
        return;
    }
    SDL_FRect dst {
        .x = this->pos.x - gd.mapViewport.x,
        .y = this->pos.y - gd.mapViewport.y,
        .w = width,
        .h = height
    };
    SDL_RenderTexture(state.renderer, this->texture, nullptr, &dst);
    this->drawDebug(state, gd, width, height);
}

void Object::drawDebug(const SDLState &state, GameData &gd, float width, float height) {
    if (gd.debugMode) {
        SDL_FRect rectA {
            .x = this->pos.x + this->collider.x - gd.mapViewport.x, 
            .y = this->pos.y + this->collider.y - gd.mapViewport.y,
            .w = this->collider.w, 
            .h = this->collider.h
        };
        SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 150);
        SDL_RenderFillRect(state.renderer, &rectA);
        SDL_FRect sensor{  
            .x = this->pos.x + this->collider.x - gd.mapViewport.x,
            .y = this->pos.y + this->collider.y + this->collider.h - gd.mapViewport.y,
            .w = this->collider.w, 
            .h = 1
        };
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 150);
        SDL_RenderFillRect(state.renderer, &sensor);

        SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_NONE);
    }
}

void AnimatedObject::draw(const SDLState &state, GameData &gd, float width, float height) {
    if (!isOnscreen(state, gd, (*this))) {
        return;
    }
    float srcX = this->curAnimation != -1 ? this->animations[this->curAnimation].currentFrame() * width : (this->spriteFrame - 1) * width;
    SDL_FRect src {
        .x = srcX,
        .y = 0,
        .w = width,
        .h = height
    };

    SDL_FRect dst {
        .x = this->pos.x - gd.mapViewport.x,
        .y = this->pos.y - gd.mapViewport.y,
        .w = width,
        .h = height
    };
    SDL_FlipMode flipMode; // = obj.dir == -1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    if (this->dir == -1) {            
        if (this->flip == -1) {
            flipMode = (SDL_FlipMode)3; // FLIP_VERTICAL_AND_HORIZONTAL
        } else {
            flipMode = SDL_FLIP_HORIZONTAL;
        }
    } else {
        if (this->flip == -1) {
            flipMode = SDL_FLIP_VERTICAL;
        } else {
            flipMode = SDL_FLIP_NONE;
        }
    }
    SDL_RenderTextureRotated(state.renderer, this->texture, &src, &dst, 0, nullptr, flipMode); // src is for sprite stripping, dest is for where sprite should be drawn
    this->drawDebug(state, gd, width, height);
}

void AnimatedObject::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) { // just step the anims
    if (this->curAnimation != -1) {
        this->animations[this->curAnimation].step(deltaTime);
    }
}

void Effect::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    if (this->curAnimation != -1) {
        this->animations[this->curAnimation].step(deltaTime);
    }
    if (this->followsPlayer) {
        this->pos = gd.player.pos;
    }
}

void charIconObject::draw(const SDLState &state, GameData &gd, float width, float height) {

    float srcX = ((*this).spriteFrame - 1) * width;
    
    SDL_FRect src {
        .x = srcX,
        .y = 0,
        .w = width,
        .h = height
    };

    SDL_FRect dst {
        .x = (*this).pos.x,
        .y = (*this).pos.y,
        .w = width,
        .h = height
    };
    SDL_FlipMode flipMode; // = obj.dir == -1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    flipMode = SDL_FLIP_NONE;
    SDL_RenderTextureRotated(state.renderer, (*this).texture, &src, &dst, 0, nullptr, flipMode); // src is for sprite stripping, dest is for where sprite should be drawn
    (*this).drawDebug(state, gd, width, height);
}

void charIconObject::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime, int newState) { // just step the anims

}

void BackgroundObject::draw(const SDLState &state, GameData &gd, float width, float height) { // same as Object.draw, just no debug option (maybe we can optimize this?)
    if (!isOnscreen(state, gd, (*this))) {
        return;
    }
    SDL_FRect dst {
        .x = this->pos.x - gd.mapViewport.x,
        .y = this->pos.y - gd.mapViewport.y,
        .w = width,
        .h = height
    };
    SDL_RenderTexture(state.renderer, this->texture, nullptr, &dst);
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
            itemType::ICE, itemType::PIE, itemType::SUGAR};
    }
    // Limit items for bottom 25% of players
    else if ((player.position / (float)gd.numPlayers) >= 0.75) {
        itemOptions = {itemType::BOOMBOX, itemType::BOUNCYBALL, 
            itemType::FOG, itemType::MISSILE, itemType::PIE, itemType::SUGAR};
    }
    else {
        // All items are available
        itemOptions = {itemType::BOMB, itemType::BOOMBOX, itemType::BOUNCYBALL, 
            itemType::FOG, itemType::ICE, itemType::MISSILE, itemType::PIE, itemType::SUGAR};
    }
    selected = 1;//rand() % itemOptions.size();
    switch (itemOptions[selected]) {
        case itemType::BOMB:
            newItem = Bomb(player.pos, defaultCollider, res.texBomb);
            break;
        case itemType::BOOMBOX:
            newItem = Boombox(player.pos, defaultCollider, res.texBoombox);
            break;
        case itemType::SUGAR:
            newItem = Sugar(player.pos, defaultCollider, res.texSugar);
        default:
            printf("Your item is in another castle\n");
            newItem = Bomb(player.pos, defaultCollider, res.texBomb);
    }
    gd.player.nextItem = newItem;
    player.pickingItem = true;
}


void angledStun(AnimatedObject &obj, GameData &gd, Resources &res) {
    // TODO properly implement angles
    float baseKnockback = 100.0f;
    Player player = gd.player;
    player.state_->nextStateVal = DEAD;
    PlayerState * newState = changePlayerState(player.state_);
    delete player.state_;
    player.state_ = newState;
    player.state_->enter(player, gd, res);
    if (player.vel.x > 0) {
        player.vel.x = changeVel(baseKnockback * obj.knockbackMultiplier, player);
    }
    else if (player.vel.x < 0) {
        player.vel.x = changeVel(-baseKnockback* obj.knockbackMultiplier, player);
    }
    player.vel.x = changeVel(-player.vel.x, player);
    float shouldFlip = player.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
    if(shouldFlip * obj.pos.y < shouldFlip * player.pos.y ){
        player.vel.y = changeVel(baseKnockback * obj.knockbackMultiplier, player);
    } else {
        player.vel.y = changeVel(-baseKnockback* obj.knockbackMultiplier, player);
    }
    player.gravityScale = 1.0f;
    player.isStunned = true;
    player.grounded = false;
    gd.player = player;
}

void effectExplosion(GameData &gd, Resources &res, AnimatedObject obj) {
    SDL_FRect expCollider = {
        .x = 0,
        .y = 0,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };
    glm::vec2 expPos;
    expPos.x = obj.pos.x - TILE_SIZE * 2;
    expPos.y = obj.pos.y - TILE_SIZE * 2;
    Effect * explosion = new Effect(expPos, expCollider, res.texExplosion);
    explosion->animations = res.itemAnims;
    explosion->followsPlayer = false;
    explosion->curAnimation = res.ANIM_ITEM_EXPLOSION;
    explosion->animations[explosion->curAnimation].reset();
    gd.effects_.push_back(explosion);
}