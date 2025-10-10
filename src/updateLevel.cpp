#include "../headers/updateLevel.h"
#include "../headers/collision.h"
#include "../headers/gameData.h"
#include "../headers/initState.h"
#include "../headers/helper.h"
#include "../headers/globals.h"
#include "../headers/resources.h"
#include "../headers/state.h"
#include "../headers/playerState.h"

using namespace std;

extern GameState * currState;

//
//UPDATE FUNCTIONS
//

//Update Function for level Spaceship
void levelUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    // update portals
        //gd.player.currentDirection = 0;
        for (Portal &portal : gd.portals_) {
            portal.update(state, gd, res, deltaTime);
        }

        if (gd.player.blast != nullptr) {
            gd.player.blast->update(state, gd, res, deltaTime);
        }

        // update lasers
        for (Laser &laser : gd.lasers_) {
            laser.update(state, gd, res, deltaTime);
        }

        for (ItemBox &box : gd.itemBoxes_) {
            if (!box.itemBoxActive) {
                box.update(state, gd, res, deltaTime);
            }
        }

        if (gd.player.pickingItem) {
            gd.itemStorage_.update(state, gd, res, deltaTime);
        }

        //gd.player.currentDirection = 0;
        gd.player.state_->update(state, gd, res, deltaTime);
        if(gd.player.currentDirection){
            gd.player.dir = gd.player.currentDirection;
        }

        gd.hook.update(state, gd, res, deltaTime);
        gd.hook.checkCollision(state, gd, res, deltaTime);
        
        gd.player.vel += static_cast<float>(gd.player.currentDirection) * gd.player.acc * deltaTime;

        if (std::abs(gd.player.vel.x) > gd.player.maxSpeedX) {
            if (!isSliding(gd.player)) { // if not sliding slow down
                gd.player.vel.x -= 1.5 * gd.player.acc.x * deltaTime * gd.player.currentDirection;
            }
            
        }

        if(gd.player.usingSugar){
            ((Sugar *) &gd.player.item)->sugarTimer.step(deltaTime);
            gd.player.vel.x += .5f * gd.player.currentDirection;
            if(((Sugar *) &gd.player.item)->sugarTimer.isTimeOut()){
                //printf("Stopped sugar\n");
                gd.player.usingSugar = false;
            }
        }

        // add vel to pos 
        gd.player.pos += updatePos(gd.player, deltaTime);

        gd.player2.pos += updatePos(gd.player2, deltaTime);

        // collision
        gd.player.grounded = false;
        collisionCheckAndResponse(state,gd,res,gd.player,deltaTime);

        //printf("%d\n", gd.player.state_->currStateVal);
        gd.itemStorage_.pos.x = gd.player.pos.x - 368;
        gd.itemStorage_.pos.y = gd.player.pos.y - 200;
        
}

//
//INPUT FUNCTIONS
//

//Input Function for level Spaceship
void levelInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_Event event { 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: 
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    //printf("Width = %d, Height = %d", state.width, state.height);
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    handleKeyInput(state, gd, res, event.key, true, deltaTime);
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    handleKeyInput(state, gd, res, event.key, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    //handleClick(state, gd, res, gd.player(), deltaTime);
                    handleLevelClick(state, gd, res, deltaTime, event, true);
                    break;
                } 
                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    //handleClick(state, gd, res, gd.player(), deltaTime);
                    handleLevelClick(state, gd, res, deltaTime, event, false);
                    break;
                } 
            }
        }
}

//handler for clicking in the level
void handleLevelClick(SDLState &state, GameData &gd, Resources &res, float deltaTime, SDL_Event event, bool buttonDown) {
    //LEFT CLICK FOR CHARACTER WEAPON DEPLOY
    if(event.button.button == SDL_BUTTON_LEFT && buttonDown){
        //JETPACK DEPLOY
        if(((LevelState *)currState)->character == JETPACK) {
            if(gd.player.cooldownTimer.isTimeOut()) {
                    gd.player.state_ = changePlayerState(gd, res, gd.player.state_, JETPACK_DEPLOY);
            }
        } else if (((LevelState *)currState)->character == SHOTGUN) {
            //SHOTGUN DEPLOY
            if(gd.player.cooldownTimer.isTimeOut()) {
                gd.player.state_ = changePlayerState(gd, res, gd.player.state_, SHOTGUN_DEPLOY);
            }
        } else if (((LevelState *)currState)->character == SWORD) {
            //SWORD DEPLOY
            if(gd.player.cooldownTimer.isTimeOut()) {
                gd.player.state_ = changePlayerState(gd, res, gd.player.state_, SWORD_DEPLOY);
            }
        }
    } else if (buttonDown && event.button.button == SDL_BUTTON_RIGHT) { // grapple
        glm::vec2 hOffset = findCenterOfSprite(gd.hook);
        gd.hook.pos = gd.player.pos + hOffset;
        gd.hook.visible = true;
        std::vector dist = distanceForm(gd, gd.player, gd.hook);
        gd.hook.vel = 500.0f * glm::vec2(dist.at(3), dist.at(4));
    } else if (!buttonDown && event.button.button == SDL_BUTTON_RIGHT) { // grapple release 
        gd.hook.pos = glm::vec2(-1000.0f, -1000.0f); // maybe unnecessary
        gd.hook.visible = false;
        if (gd.hook.collided) { // get out
            gd.player.state_ = changePlayerState(gd, res, gd.player.state_, JUMP);
        }
        gd.hook.collided = false;
    }
}

//
//INPUT HANDLERS
//

//Crosshair for in level
void handleCrosshair(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    gd.mouseCoords.x = gd.mouseCoords.x * xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = CROSSHAIR_SIZE,
        .h = CROSSHAIR_SIZE
    };

    //printf("mouseX: %f, mouseY: %f\n", gd.mouseCoords.x, gd.mouseCoords.y);
    SDL_RenderTexture(state.renderer, res.texCrosshair, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
}

//Key Input Handler for Level
void handleKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime) {

    if (key.scancode == SDL_SCANCODE_F12 && key.down && !key.repeat) { // debug
            gd.debugMode = !gd.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F11 && key.down && !key.repeat) { // tp to entrance portal
        gd.player.pos = gd.EntrancePortal;
        gd.player.pos.x -= TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_F10 && key.down && !key.repeat) { // tp to exit portal
        gd.player.pos = gd.ExitPortal;
    }
    /*if (key.scancode == SDL_SCANCODE_F2 && keyDown && !key.repeat) { // anti gravity
        gd.player().flip = -1 * gd.player().flip;
    }*/
    if (key.scancode == SDL_SCANCODE_F1) {
        running = false;
    }
    if (key.scancode == SDL_SCANCODE_F2) {
        printf("currState: %d, nextState: %d", currState->currStateVal, currState->nextStateVal);
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
    if (key.scancode == SDL_SCANCODE_Q && gd.player.hasItem) {
        Item item = gd.player.item;
        item.useItem(state, gd, res);
        gd.player.hasItem = false;
        clearItem(state, gd, res);
    }
    gd.player.state_->handleInput(gd, res, key);   
}