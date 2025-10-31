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
    for (Portal &portal : gd.portals_) {
        portal.update(state, gd, res, deltaTime);
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

    for (Player &p : gd.players_) {
        p.update(state, gd, res, deltaTime);

        if (p.usingSugar) { // TODO: this currently draws below the screen, and this should probably be in draw
            //Draw sugar effect
            if(p.dir == 1) {
                glm::vec2 pos = glm::vec2(p.pos.x - 30.f, p.pos.y);
                SDL_FRect collider = {
                    .x = 28 * (p.dir),
                    .y = 0,
                    .w = 0.f,
                    .h = 0.f
                    };
                Object sugarEffectObject(pos, collider, res.texSugarEffectL);
                sugarEffectObject.draw(state,gd,32,32);
            } else {
                glm::vec2 pos = glm::vec2(p.pos.x + 30.f, p.pos.y);
                SDL_FRect collider = {
                    .x = 28 * (p.dir),
                    .y = 0,
                    .w = 0.f,
                    .h = 0.f
                    };
                Object sugarEffectObject(pos, collider, res.texSugarEffectR);
                sugarEffectObject.draw(state,gd,32,32);
            }
        }
    }

    //printf("%d\n", gd.player.state_->currStateVal);

    gd.itemStorage_.pos.x = gd.players_[0].pos.x - 368;
    gd.itemStorage_.pos.y = gd.players_[0].pos.y - 200;

    gd.minimap.update(state, gd, res, deltaTime);


    // gd.mapViewport.x = (gd.players_[0].pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    // gd.mapViewport.y = (gd.players_[0].pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 

}

//
//INPUT FUNCTIONS
//

//Input Function for level Spaceship
void levelInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime) {
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
                    for (Player &p : gd.players_) {
                        handleLevelClick(state, gd, res, p, deltaTime, event, true);
                    }
                    break;
                } 
                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    for (Player &p : gd.players_) {
                        handleLevelClick(state, gd, res, p, deltaTime, event, false);
                    }
                    break;
                } 
            }
        }
}

//handler for clicking in the level
void handleLevelClick(SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime, SDL_Event event, bool buttonDown) {
    //LEFT CLICK FOR CHARACTER WEAPON DEPLOY
    if(event.button.button == SDL_BUTTON_LEFT && buttonDown){
        //JETPACK DEPLOY
        if(((LevelState *)currState)->character == JETPACK) {
            if (p.cooldownTimer.isTimeOut() && p.state_->stateVal != JETPACK_DEPLOY) {
                PlayerState* jpState = new JetpackDeployState();
                p.handleState(jpState, gd, res);
            }
        } else if (((LevelState *)currState)->character == SHOTGUN) {
            //SHOTGUN DEPLOY
            if(p.cooldownTimer.isTimeOut() && p.state_->stateVal != SHOTGUN_DEPLOY) {
                PlayerState* sgState = new ShotgunDeployState();
                p.handleState(sgState, gd, res);
            }
        } else if (((LevelState *)currState)->character == SWORD) {
            //SWORD DEPLOY
            if(p.cooldownTimer.isTimeOut() && p.state_->stateVal != SWORD_DEPLOY) {
                PlayerState* swState = new SwordDeployState();
                p.handleState(swState, gd, res);
            }
        }
    } else if (buttonDown && event.button.button == SDL_BUTTON_RIGHT) { // grapple
        glm::vec2 pOffset = findCenterOfSprite(p);
        glm::vec2 hOffset = findCenterOfSprite(p.hook);
        float xDist = gd.mouseCoords.x - (p.pos.x - gd.mapViewport.x + pOffset.x); // A
        float yDist = gd.mouseCoords.y - (p.pos.y - gd.mapViewport.y + pOffset.y); // O
        float dist = std::sqrt(xDist * xDist + yDist * yDist); // distance formula, H
        float aH = xDist / dist; // cos
        float oH = yDist / dist; // sin

        p.hook.pos = p.pos + hOffset;
        p.hook.visible = true;
        p.hook.vel = 500.0f * glm::vec2(aH, oH);
    } else if (!buttonDown && event.button.button == SDL_BUTTON_RIGHT) { // grapple release 
        if (p.hook.collided) { // get out
            PlayerState* jState = new JumpState();
            p.handleState(jState, gd, res);
        }
        removeHook(p);
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
    gd.mouseCoords.x = gd.mouseCoords.x * state.xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * state.yRatio;
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
        gd.players_[0].pos = gd.EntrancePortal;
        gd.players_[0].pos.x -= TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_F10 && key.down && !key.repeat) { // tp to exit portal
        gd.players_[0].pos = gd.ExitPortal;
    }
    /*if (key.scancode == SDL_SCANCODE_F2 && keyDown && !key.repeat) { // anti gravity
        gd.player().flip = -1 * gd.player().flip;
    }*/
    if (key.scancode == SDL_SCANCODE_F1) {
        running = false;
    }
    if (key.scancode == SDL_SCANCODE_F2) {
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
    if (key.scancode == SDL_SCANCODE_F3) {
        
        gd.players_[0].pos.x = 0;
        gd.players_[0].pos.y = 0;
    }
    for (Player &p : gd.players_) {
        p.handleInput(state, gd, res, key, deltaTime); 
        if (key.scancode == SDL_SCANCODE_Q && p.hasItem) {
            Item item = p.item;
            item.useItem(state, gd, res, p);
            p.hasItem = false;
            clearItem(state, gd, res);
        }
    }  
}