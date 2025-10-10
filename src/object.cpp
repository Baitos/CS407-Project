#include <SDL3/SDL.h>
#include <cstdio>
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/helper.h"
#include "../headers/collision.h"
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

void Hook::draw(const SDLState &state, GameData &gd, float width, float height) {
    if (!(*this).visible) {
        return;
    }
    SDL_SetRenderDrawColor(state.renderer, 30, 30, 30, 255); // draw line to hook
    glm::vec2 pOffset = findCenterOfSprite(gd.player);
    glm::vec2 hOffset = findCenterOfSprite(gd.hook);
    //printf("x: %f, y: %f\n", gd.mouseCoords.x, gd.mouseCoords.y);
    SDL_RenderLine(state.renderer, gd.player.pos.x - gd.mapViewport.x + pOffset.x, 
                    gd.player.pos.y - gd.mapViewport.y + pOffset.y, 
                    gd.hook.pos.x - gd.mapViewport.x + hOffset.x, 
                    gd.hook.pos.y - gd.mapViewport.y + hOffset.y);
    SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
    if (!isOnscreen(state, gd, (*this))) {
        return;
    }
    SDL_FRect dst {
        .x = (*this).pos.x - gd.mapViewport.x,
        .y = (*this).pos.y - gd.mapViewport.y,
        .w = width,
        .h = height
    };
    double angle = 0; // default texture faces right, sdl_render rotates clockwise
    if ((*this).vel.y > 0 && std::abs((*this).vel.y) > std::abs((*this).vel.x)) {
        angle = 90; // down
    }
    else if ((*this).vel.x < 0 && std::abs((*this).vel.x) > std::abs((*this).vel.y)) {
        angle = 180; // left
    }
    else if ((*this).vel.y < 0 && std::abs((*this).vel.y) > std::abs((*this).vel.x)) {
        angle = 270; // up
    }
    SDL_RenderTextureRotated(state.renderer, (*this).texture, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE); // this could probably be done using the flip variable but there's no way to rotate the image that way
    (*this).drawDebug(state, gd, width, height);
}

void Hook::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    (*this).pos += updatePos((*this), deltaTime);
}

void Hook::checkCollision(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
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
                gd.player.state_ = changePlayerState(gd, res, gd.player.state_, GRAPPLE);
                (*this).collided = true;
            }
        }
    }
    Player p2 = gd.player2;
    rectB = {
        .x = p2.pos.x + p2.collider.x,
        .y = p2.pos.y + p2.collider.y,
        .w = p2.collider.w,
        .h = p2.collider.h
    };
    if (intersectAABB(rectA, rectB, resolution)) { // TODO: if touching player, hardcoded for player2 for now
        gd.player.vel = 0.7f * (*this).vel;
        p2.vel = -0.3f * (*this).vel;
        (*this).visible = false;
        (*this).collided = true;
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
    gd.player.nextItem = newItem;
    player.pickingItem = true;
}