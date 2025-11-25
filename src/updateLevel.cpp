#include "../headers/updateLevel.h"
#include "../headers/collision.h"
#include "../headers/gameData.h"
#include "../headers/initState.h"
#include "../headers/helper.h"
#include "../headers/globals.h"
#include "../headers/resources.h"
#include "../headers/state.h"
#include "../headers/playerState.h"
#include "../headers/enet.h"
#include <algorithm>

using namespace std;

extern GameState * currState;
extern ENetPeer* serverPeer;
extern ENetHost * client;
//
//UPDATE FUNCTIONS
//

//Update Function for level Spaceship
void levelUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    //calculate placement
    placement(state, gd, res, deltaTime);

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
    }

    //gd.players_[gd.playerIndex].update(state, gd, res, deltaTime);


    gd.itemStorage_.pos.x = gd.players_[gd.playerIndex].pos.x - 368;
    gd.itemStorage_.pos.y = gd.players_[gd.playerIndex].pos.y - 190;
    gd.minimap.update(state, gd, res, deltaTime);

    // gd.mapViewport.x = (gd.players_[gd.playerIndex].pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    // gd.mapViewport.y = (gd.players_[gd.playerIndex].pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 

    //check if game over
    if(gd.round_is_over) {
        gd.round_is_over = false;
        if(gd.isGrandPrix){
            if(currState->currStateVal != SNOW){		//TODO ELLIE MAKE THIS THE LAST STAGE IN THE GRAND PRIX
                currState->nextStateVal = RESULTS;
                currState = changeState(currState,gd);
                currState->init(state, gd, res);
            } else {
                currState->nextStateVal = END_RESULTS;
                currState = changeState(currState,gd);
                currState->init(state, gd, res);
            }
        } else {
            printf("END RESULTS\n");
            currState->nextStateVal = END_RESULTS;
            currState = changeState(currState,gd);
            currState->init(state, gd, res);
        }
        // if(gd.isGrandPrix) {
        //     if(currState->currStateVal==GRASSLANDS) {
        //         currState->nextStateVal = SPACESHIP;
        //     } else if(currState->currStateVal==SPACESHIP) {
        //         currState->nextStateVal = SNOW;
        //     } else if (currState->currStateVal==SNOW) {
        //         gd.isGrandPrix = false;
        //         currState->nextStateVal = TITLE;
        //     }
        //     currState = changeState(currState, gd);
        //     currState->init(state, gd, res);
        // } else {
        //     currState->nextStateVal = TITLE;
        //     currState = changeState(currState, gd);
        //     currState->init(state, gd, res);
        // }
    }

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
                    handleKeyInput(state, gd, res, event, true, deltaTime);
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {   
                    handleKeyInput(state, gd, res, event, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {

                    
                        handleLevelClick(state, gd, res, gd.players_[gd.playerIndex], deltaTime, event, true);
                    
                    break;
                } 
                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    //for (Player &p : gd.players_) {
                        handleLevelClick(state, gd, res, gd.players_[gd.playerIndex], deltaTime, event, false);
                    //}
                    break;
                } 
            }
        }
}

//handler for clicking in the level
void handleLevelClick(SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime, SDL_Event event, bool buttonDown) {
    //LEFT CLICK FOR CHARACTER WEAPON DEPLOY
    //printf("Player %d Character Type %d\n", gd.playerIndex, gd.players_[gd.playerIndex].character);
    if(gd.controls->actionPerformed(ACTION_ABILITY, event) && buttonDown){
        //JETPACK DEPLOY
        if(p.character == JETPACK) {
            if (p.cooldownTimer.isTimeOut() && p.state_->stateVal != JETPACK_DEPLOY) {
                PlayerState* jpState = new JetpackDeployState();
                p.handleState(jpState, gd, res);
            }
        } else if (p.character == SHOTGUN) {
            //SHOTGUN DEPLOY
            if(p.cooldownTimer.isTimeOut() && p.state_->stateVal != SHOTGUN_DEPLOY) {
                PlayerState* sgState = new ShotgunDeployState();
                p.handleState(sgState, gd, res);
            }
        } else if (p.character == SWORD) {
            //SWORD DEPLOY
            if(p.cooldownTimer.isTimeOut() && p.state_->stateVal != SWORD_DEPLOY) {
                PlayerState* swState = new SwordDeployState();
                p.handleState(swState, gd, res);
            }
        }
    } else if (buttonDown && gd.controls->actionPerformed(ACTION_GRAPPLE, event)) { // grapple
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
    } else if (!buttonDown && gd.controls->actionPerformed(ACTION_GRAPPLE, event)) { // grapple release 
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
                    SDL_Event event, bool keyDown, float deltaTime) {
    SDL_KeyboardEvent key = event.key;

    //Send server your input
    if(!key.repeat){
        std::string pendingMessage = "INPUT " + std::to_string(gd.playerIndex) + " " + std::to_string(key.scancode) + " " + std::to_string(key.down);
        ENetPacket * packet = enet_packet_create(pendingMessage.c_str(), pendingMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(serverPeer, 0, packet);
        enet_host_flush(client);
    }

    if (key.scancode == SDL_SCANCODE_F12 && key.down && !key.repeat) { // debug
            gd.debugMode = !gd.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F11 && key.down && !key.repeat) { // tp to entrance portal
        gd.players_[gd.playerIndex].pos = gd.EntrancePortal;
        gd.players_[gd.playerIndex].pos.x -= TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_F10 && key.down && !key.repeat) { // tp to exit portal
        gd.players_[gd.playerIndex].pos = gd.ExitPortal;
    }
    /*if (key.scancode == SDL_SCANCODE_F2 && keyDown && !key.repeat) { // anti gravity
        gd.player().flip = -1 * gd.player().flip;
    }*/
       // Item select, assuming last place for now
    if (key.scancode == SDL_SCANCODE_0) {
        selectedItem = (int)(BOOMBOX); // Boombox
    }
    if (key.scancode == SDL_SCANCODE_1) {
        selectedItem = (int)(BOMB); // Bomb
    }
    if (key.scancode == SDL_SCANCODE_2) {
        selectedItem = (int)(SUGAR); // Sugar
    }
    if (key.scancode == SDL_SCANCODE_3) {
        selectedItem = (int)(PIE); // Pie
    }
    if (key.scancode == SDL_SCANCODE_4) {
        selectedItem = (int)(ICE); // Ice
    }

    if (key.scancode == SDL_SCANCODE_F1) {
        running = false;
    }
    if (key.scancode == SDL_SCANCODE_F2 && key.down) {
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
    if (key.scancode == SDL_SCANCODE_F3 && key.down) {
        
        gd.players_[gd.playerIndex].pos.x = 0;
        gd.players_[gd.playerIndex].pos.y = 0;
    }

    gd.players_[gd.playerIndex].handleInput(state, gd, res, event, deltaTime); 
    if (gd.controls->actionPerformed(typeAction::ACTION_USEITEM, event) && gd.players_[gd.playerIndex].hasItem && !gd.players_[gd.playerIndex].pickingItem) {
        Item* item = gd.players_[gd.playerIndex].heldItem;
        item->useItem(state, gd, res, gd.players_[gd.playerIndex]);
        gd.players_[gd.playerIndex].hasItem = false;
        printf("HAS ITEM IS FALSE via Client\n");
        clearItem(state, gd, res);
    }
    // for (Player &p : gd.players_) {
    //     p.handleInput(state, gd, res, event, deltaTime); 
    //     if (gd.controls->actionPerformed(typeAction::ACTION_USEITEM, event) && p.hasItem) {
    //         Item* item = p.heldItem;
    //         item->useItem(state, gd, res, p);
    //         p.hasItem = false;
    //         clearItem(state, gd, res);
    //     }
    // }  
}

//calculate distance between two
double calculateDistanceSDL(float x1, float y1, float x2, float y2) {
    double dx = static_cast<double>(x2 - x1);
    double dy = static_cast<double>(y2 - y1);
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

void placement(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    //calculate the placement of each player based on their checkpoints and position
    //has to use pointers because we need to modify the .placements of the actual gd objects
    std::vector<Player*> playersPlacement;
    playersPlacement.reserve(gd.players_.size());
    for (auto &p : gd.players_) {
        playersPlacement.push_back(&p);
    }

    std::stable_sort(playersPlacement.begin(), playersPlacement.end(),
        [&gd](const Player* p1, const Player* p2) {
            //first sort by laps completed
            if (p1->lapsCompleted != p2->lapsCompleted) {
                return p1->lapsCompleted > p2->lapsCompleted;
            } else if (p1->lastCheckpoint != p2->lastCheckpoint) {
                //next by last checkpoint
                return p1->lastCheckpoint > p2->lastCheckpoint;
            } else {
                //finally by distance until the next checkpoint
                return abs(calculateDistanceSDL(
                    gd.checkpoints_[p1->lastCheckpoint+1].collider.x,
                    gd.checkpoints_[p1->lastCheckpoint+1].collider.y,
                    p1->pos.x, p1->pos.y
                )) < abs(calculateDistanceSDL(
                    gd.checkpoints_[p2->lastCheckpoint+1].collider.x,
                    gd.checkpoints_[p2->lastCheckpoint+1].collider.y,
                    p2->pos.x, p2->pos.y
                ));
            }
        });

    for (int i = 0; i < playersPlacement.size(); i++) {
        playersPlacement[i]->position = i + 1;  // Updates the real gd.players_ objects
    }
}
