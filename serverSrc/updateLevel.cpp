#include "../serverHeaders/updateLevel.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/state.h"
#include "../serverHeaders/playerState.h"
#include "../serverHeaders/enet.h"
#include <algorithm>

using namespace std;

//
//UPDATE FUNCTIONS
//

//Update Function for level Spaceship
void levelUpdate(const SDLState &state, GameData &gd, float deltaTime, int keyID, int keyDown, int playerID) {
    //calculate placement
    placement(state, gd, deltaTime);

    // update portals
    for (Portal &portal : gd.portals_) {
        portal.update(state, gd, deltaTime);
    }
    
    // update lasers
    for (Laser &laser : gd.lasers_) {
        laser.update(state, gd, deltaTime);
    }
    for (ItemBox &box : gd.itemBoxes_) {
        if (!box.itemBoxActive) {
            box.update(state, gd, deltaTime);
        }
    }
    // for (int i = 0; i < gd.items_.size(); i++) {
    //     // TODO This shit sucks ass please fix lol - Rei
    //     Item item = gd.items_[i];
    //     item.update(state, gd, deltaTime);
    //     if (item.type == itemType::BOOMBOX) {
    //         item.pos.x = gd.player.pos.x - (item.width / 2) + 16;
    //         item.pos.y = gd.player.pos.y - (item.height/2) + 16;
    //     }
    //     gd.items_[i] = item;
    //     if (item.animations[item.curAnimation].isDone()) {
    //         gd.items_.erase(gd.items_.begin() + i);
    //         i--;
    //     }
    // }
    for (Player &p : gd.players_) {
        if (playerID == p.index){
            p.update(state, gd, deltaTime, keyID, keyDown);
        } else {
            p.update(state, gd, deltaTime, -1, -1);
        }
    }

    //gd.players_[gd.playerIndex].update(state, gd, deltaTime);


    //gd.itemStorage_.pos.x = gd.players_[gd.playerIndex].pos.x - 368;
    //gd.itemStorage_.pos.y = gd.players_[gd.playerIndex].pos.y - 190;
    //gd.minimap.update(state, gd, deltaTime);

    // gd.mapViewport.x = (gd.players_[0].pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    // gd.mapViewport.y = (gd.players_[0].pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 

}

//
//INPUT FUNCTIONS
//

//Input Function for level Spaceship
void levelInputs(SDLState &state, GameData &gd, float deltaTime) {
   
}

//handler for clicking in the level
void handleLevelClick(SDLState &state, GameData &gd, Player &p, float deltaTime, SDL_Event event, bool buttonDown) {
    //LEFT CLICK FOR CHARACTER WEAPON DEPLOY
    //printf("Player %d Character Type %d\n", gd.playerIndex, gd.players_[gd.playerIndex].character);
    // if(gd.controls->actionPerformed(ACTION_ABILITY, event) && buttonDown){
    //     //JETPACK DEPLOY
    //     if(p.character == JETPACK) {
    //         if (p.cooldownTimer.isTimeOut() && p.state_->stateVal != JETPACK_DEPLOY) {
    //             PlayerState* jpState = new JetpackDeployState();
    //             p.handleState(jpState, gd);
    //         }
    //     } else if (p.character == SHOTGUN) {
    //         //SHOTGUN DEPLOY
    //         if(p.cooldownTimer.isTimeOut() && p.state_->stateVal != SHOTGUN_DEPLOY) {
    //             PlayerState* sgState = new ShotgunDeployState();
    //             p.handleState(sgState, gd);
    //         }
    //     } else if (p.character == SWORD) {
    //         //SWORD DEPLOY
    //         if(p.cooldownTimer.isTimeOut() && p.state_->stateVal != SWORD_DEPLOY) {
    //             PlayerState* swState = new SwordDeployState();
    //             p.handleState(swState, gd);
    //         }
    //     }
    // } else if (buttonDown && gd.controls->actionPerformed(ACTION_GRAPPLE, event)) { // grapple
    //     glm::vec2 pOffset = findCenterOfSprite(p);
    //     glm::vec2 hOffset = findCenterOfSprite(p.hook);
    //     float xDist = gd.mouseCoords.x - (p.pos.x - gd.mapViewport.x + pOffset.x); // A
    //     float yDist = gd.mouseCoords.y - (p.pos.y - gd.mapViewport.y + pOffset.y); // O
    //     float dist = std::sqrt(xDist * xDist + yDist * yDist); // distance formula, H
    //     float aH = xDist / dist; // cos
    //     float oH = yDist / dist; // sin

    //     p.hook.pos = p.pos + hOffset;
    //     p.hook.visible = true;
    //     p.hook.vel = 500.0f * glm::vec2(aH, oH);
    // } else if (!buttonDown && gd.controls->actionPerformed(ACTION_GRAPPLE, event)) { // grapple release 
    //     if (p.hook.collided) { // get out
    //         PlayerState* jState = new JumpState();
    //         p.handleState(jState, gd);
    //     }
    //     removeHook(p);
    // }
}

//
//INPUT HANDLERS
//

//Crosshair for in level
void handleCrosshair(const SDLState &state, GameData &gd, float deltaTime) {
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
    //SDL_RenderTexture(state.renderer.texCrosshair, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
}

//Key Input Handler for Level
void handleKeyInput(const SDLState &state, GameData &gd,
                    int playerID, int keyID, int keyDown, float deltaTime) {

    
    gd.players_[playerID].handleInput(state, gd, keyID, keyDown, deltaTime); 
    
    // if (gd.controls->actionPerformed(typeAction::ACTION_USEITEM, event) && gd.players_[gd.playerIndex].hasItem) {
    //     Item* item = gd.players_[gd.playerIndex].heldItem;
    //     item->useItem(state, gd, gd.players_[gd.playerIndex]);
    //     gd.players_[gd.playerIndex].hasItem = false;
    //     clearItem(state, gd);
    // }
    // for (Player &p : gd.players_) {
    //     p.handleInput(state, gd, event, deltaTime); 
    //     if (gd.controls->actionPerformed(typeAction::ACTION_USEITEM, event) && p.hasItem) {
    //         Item* item = p.heldItem;
    //         item->useItem(state, gd, p);
    //         p.hasItem = false;
    //         clearItem(state, gd);
    //     }
    // }  
}

//calculate distance between two
double calculateDistanceSDL(float x1, float y1, float x2, float y2) {
    double dx = static_cast<double>(x2 - x1);
    double dy = static_cast<double>(y2 - y1);
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

void placement(const SDLState &state, GameData &gd, float deltaTime) {
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
