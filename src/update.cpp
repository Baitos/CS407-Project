#include "../headers/update.h"
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

// Update Function for level Spaceship
void levelUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime)
{
    // update portals
        //gd.player.currentDirection = 0;
        for (Portal &portal : gd.portals_) {
            portal.update(state, gd, res, deltaTime);
        }
        if (gd.player.blast != nullptr) {
            printf("here\n");
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
        for (int i = 0; i < gd.effects_.size(); i++) {
            Effect *effect = gd.effects_[i];
            effect->update(state, gd, res, deltaTime);
            if (effect->animations[effect->curAnimation].isDone()) {
                printf("erase it\n");
                delete effect;
                gd.effects_.erase(gd.effects_.begin() + i);
                i--;
                printf("erased\n");
            }

        }
        if (gd.player.pickingItem) {
            gd.itemStorage_.update(state, gd, res, deltaTime);
        }
        //gd.player.currentDirection = 0;

        gd.player.state_->update(state, gd, res,deltaTime);
        if(gd.player.currentDirection){
            gd.player.dir = gd.player.currentDirection;
        }
        //printf("Velocity at Update: %f\n", gd.player.vel.x);
        //printf("%f\n", gd.player.dir);
        
        gd.player.vel += static_cast<float>(gd.player.currentDirection) * gd.player.acc * deltaTime;
        //printf("Velocity after Update: %f\n", gd.player.vel.x);
        //printf("currDirection = %d\n", gd.player.currentDirection);
        if (std::abs(gd.player.vel.x) > gd.player.maxSpeedX) {
            //gd.player.vel.x = gd.player.maxSpeedX * gd.player.dir;
            if (!isSliding(gd.player)) { // if not sliding slow down
                gd.player.vel.x -= 1.5 * gd.player.acc.x * deltaTime * gd.player.currentDirection;
            }
        }

        // add vel to pos
        //printf("%f\n", gd.player.vel);
        gd.player.pos += gd.player.vel * deltaTime;
        //printf("%f", gd.player.vel);
        // collision
        bool foundGround = gd.player.grounded;
        gd.player.grounded = false;

        //printf("Is fastfalling: %d\n", gd.player.fastFalling);
        collisionCheckAndResponse(state,gd,res,gd.player,deltaTime);

        //printf("%d\n", gd.player.state_->currStateVal);
        
        // UPDATE ITEM STORAGE
        
        gd.itemStorage_.pos.x = gd.player.pos.x - 368;
        gd.itemStorage_.pos.y = gd.player.pos.y - 200;
}

//Update for Character Select Screen
void charSelectUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
        for (AnimatedObject &preview : gd.previews_) {
            preview.update(state, gd, res, deltaTime);
        }

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
                    handleLevelClick(state, gd, res, deltaTime, event);
                    break;
                } 
            }
        }
}

//handler for clicking in the level
void handleLevelClick(SDLState &state, GameData &gd, Resources &res, float deltaTime, SDL_Event event) {
    //LEFT CLICK FOR CHARACTER WEAPON DEPLOY
    if(event.button.button == SDL_BUTTON_LEFT){
        //JETPACK DEPLOY
        if(((LevelState *)currState)->character == JETPACK) {
            if(gd.player.cooldownTimer.isTimeOut()) {
                    gd.player.state_->nextStateVal = JETPACK_DEPLOY;
                    PlayerState *newState = changePlayerState(gd.player.state_);
                    delete gd.player.state_;
                    gd.player.state_ = newState;
                    gd.player.state_->enter(gd.player, gd, res);
            }
        } else if (((LevelState *)currState)->character == SHOTGUN) {
            //SHOTGUN DEPLOY
            if(gd.player.cooldownTimer.isTimeOut()) {
                gd.player.state_->nextStateVal = SHOTGUN_DEPLOY;
                PlayerState *newState = changePlayerState(gd.player.state_);
                delete gd.player.state_;
                gd.player.state_ = newState;
                gd.player.state_->enter(gd.player, gd, res);
            }
        } else if (((LevelState *)currState)->character == SWORD) {
            //SWORD DEPLOY
            if(gd.player.cooldownTimer.isTimeOut()) {
                gd.player.state_->nextStateVal = SWORD_DEPLOY;
                PlayerState *newState = changePlayerState(gd.player.state_);
                delete gd.player.state_;
                gd.player.state_ = newState;
                gd.player.state_->enter(gd.player, gd, res);
            }
        }
    } else if (event.button.button == SDL_BUTTON_RIGHT) {
        //RIGHT CLICK FOR CHARACTER GRAPPLE (?)
    }
}

//Input Function for Character Select Screen
void charSelectInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime){
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
                    handleCharSelectKeyInput(state, gd, res, event.key, true, deltaTime);
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    handleCharSelectKeyInput(state, gd, res, event.key, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    handleCharSelectClick(state, gd, res, deltaTime);
                    break;
                    
                } 
            }
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
    SDL_FRect dst{
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = CROSSHAIR_SIZE,
        .h = CROSSHAIR_SIZE};
    SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255); // draw line to crosshair
    glm::vec2 pOffset = findCenterOfSprite(gd.player);
    // printf("x: %d, y: %d\n", pOffset.x, pOffset.y);
    SDL_RenderLine(state.renderer, gd.player.pos.x - gd.mapViewport.x + pOffset.x, gd.player.pos.y - gd.mapViewport.y + pOffset.y, gd.mouseCoords.x, gd.mouseCoords.y);
    SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);

    // printf("mouseX: %f, mouseY: %f\n", gd.mouseCoords.x, gd.mouseCoords.y);
    SDL_RenderTexture(state.renderer, res.texCrosshair, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/
}
//Mouse Cursor for Title/Settings/Char Select/Etc.
void handleMousePointer(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
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
        .w = 32,
        .h = 32
    };
    SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
}
//Key Input Handler for Level
void handleKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime)
{

    if (key.scancode == SDL_SCANCODE_F12 && key.down && !key.repeat)
    { // debug
        gd.debugMode = !gd.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F11 && key.down && !key.repeat)
    { // tp to entrance portal
        gd.player.pos = gd.EntrancePortal;
        gd.player.pos.x -= TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_F10 && key.down && !key.repeat)
    { // tp to exit portal
        gd.player.pos = gd.ExitPortal;
    }
    /*if (key.scancode == SDL_SCANCODE_F2 && keyDown && !key.repeat) { // anti gravity
        gd.obj().flip = -1 * gd.obj().flip;
    }*/
    if (key.scancode == SDL_SCANCODE_F1)
    {
        running = false;
    }
    // Item select, assuming last place for now
    if (key.scancode == SDL_SCANCODE_0) {
        selectedItem = 0; // Boombox
    }
    if (key.scancode == SDL_SCANCODE_1) {
        selectedItem = 1; // Bomb
    }
    if (key.scancode == SDL_SCANCODE_2) {
        selectedItem = 5; // Sugar
    }
    if(key.scancode == SDL_SCANCODE_F2){
        //printf("F2 key clicked");
        currState = changeState(currState, gd);
        currState->init(state, gd, res);

    }
    if(key.scancode == SDL_SCANCODE_Q && gd.player.hasItem) {
        Item item = gd.player.item;
        item.useItem(state, gd, res);
        gd.player.hasItem = false;
        clearItem(state, gd, res);
    }
    gd.player.state_->handleInput(gd, res, key);    
}

//Key Input Handler for Char Select
void handleCharSelectKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime) {

    if (key.scancode == SDL_SCANCODE_F12 && key.down && !key.repeat) { // debug
            gd.debugMode = !gd.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F1) {
        running = false;
    }
    if(key.scancode == SDL_SCANCODE_F2){
        currState = changeState(currState, gd);
        currState->init(state,gd, res);
    }
}
//Handles Clicking for Character Select Screen
void handleCharSelectClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 156 && gd.mouseCoords.y <= 156+36)){
        //Set Icons and Player to Sword
        gd.previews_[0].texture = res.texSword;
        gd.previews_[0].pos = glm::vec2(530,200);
        gd.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = SWORD;
        for (charIconObject &ci : gd.charIcons_){
            ci.spriteFrame = ci.spriteFrame % 4;
            if (ci.spriteFrame == 3){
                ci.spriteFrame +=4;
                gd.charIcons_[0].spriteFrame = ci.spriteFrame;
            }
        }
    } else if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 220 && gd.mouseCoords.y <= 220+36)){
        //Set Icons and Player to Jetpack
        gd.previews_[0].texture = res.texJetpack;
        gd.previews_[0].pos = glm::vec2(510,200);
        gd.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = JETPACK;
        for (charIconObject &ci : gd.charIcons_){
            ci.spriteFrame = ci.spriteFrame % 4;
            if (ci.spriteFrame == 2){
                ci.spriteFrame +=4;
                gd.charIcons_[0].spriteFrame = ci.spriteFrame;
            }
        }
    } else if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 284 && gd.mouseCoords.y <= 284+36)){
        //Set Icons and Player to Shotgun
        gd.previews_[0].texture = res.texShotgun;
        gd.previews_[0].pos = glm::vec2(520,200);
        gd.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = SHOTGUN;
        for (charIconObject &ci : gd.charIcons_){
            ci.spriteFrame = ci.spriteFrame % 4;
            if (ci.spriteFrame == 1){
                ci.spriteFrame +=4;
                gd.charIcons_[0].spriteFrame = ci.spriteFrame;
            }
        }
    } else if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){
        //Enter Stage
        //TO DO - ONLY DO WHEN PLAYERS AGREE TO READY UP
        int character = ((CharSelectState*) currState)->character;
        currState = changeState(currState, gd);
        ((LevelState*) currState)->character = character;
        currState->init(state,gd, res);
    } else if ((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){
        //Exit to Title
        currState->nextStateVal = TITLE;
        currState = changeState(currState, gd);
        currState->init(state,gd, res);
    }
    
}

