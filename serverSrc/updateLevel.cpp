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
#include<vector>

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

    for (Revolver &revolver : gd.revolvers_) {
        revolver.update(state, gd, deltaTime);
    }

    for (ItemBox &box : gd.itemBoxes_) {
        if (!box.itemBoxActive) {
            box.update(state, gd, deltaTime);
        }
    }

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

    // gd.mapViewport.x = (gd.players_[gd.playerIndex].pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    // gd.mapViewport.y = (gd.players_[gd.playerIndex].pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 

}

//
//INPUT FUNCTIONS
//

//Input Function for level Spaceship
void levelInputs(SDLState &state, GameData &gd, float deltaTime) {
   
}

//handler for clicking in the level
void handleLevelClick(SDLState &state, GameData &gd, int playerID, float deltaTime, int key, bool buttonDown, float aH, float oH, ENetHost* lobbyServer, std::vector<ENetPeer *> clients) {
    //LEFT CLICK FOR CHARACTER WEAPON DEPLOY
    //printf("Player %d Character Type %d\n", gd.playerIndex, gd.players_[gd.playerIndex].character);
    if (gd.players_[playerID].state_->stateVal == STUNNED || gd.players_[playerID].state_->stateVal == DEAD) { // dont let player grapple or use ability while stunned/dead
        return;
    }
    if(key == 1 && buttonDown){
        
        //JETPACK DEPLOY
        if(gd.players_[playerID].character == JETPACK) {
            if (gd.players_[playerID].cooldownTimer.isTimeOut() && gd.players_[playerID].state_->stateVal != JETPACK_DEPLOY) {
                
                PlayerState* jpState = new JetpackDeployState();
                gd.players_[playerID].handleState(jpState, gd);
            }
        } else if (gd.players_[playerID].character == SHOTGUN) {
            //SHOTGUN DEPLOY
            if(gd.players_[playerID].cooldownTimer.isTimeOut() && gd.players_[playerID].state_->stateVal != SHOTGUN_DEPLOY) {
                //Message telling players it was used
                std::string updateMessage = "SHOT " + std::to_string(playerID);
                ENetPacket * packet = enet_packet_create(updateMessage.c_str(), updateMessage.size()+1, ENET_PACKET_FLAG_RELIABLE);             //0 means unreliable
                for(ENetPeer * c : clients){
                    //Broadcast player states
                    
                    enet_peer_send(c, 0, packet);
                    
                   
                }
                enet_host_flush(lobbyServer);

                PlayerState* sgState = new ShotgunDeployState();
                gd.players_[playerID].handleState(sgState, gd);
            }
        } else if (gd.players_[playerID].character == SWORD) {
            //SWORD DEPLOY
            if(gd.players_[playerID].cooldownTimer.isTimeOut() && gd.players_[playerID].state_->stateVal != SWORD_DEPLOY) {
                //Message telling players it was used
                std::string updateMessage = "SWORD " + std::to_string(playerID);
                ENetPacket * packet = enet_packet_create(updateMessage.c_str(), updateMessage.size()+1, ENET_PACKET_FLAG_RELIABLE);             //0 means unreliable
                 for(ENetPeer * c : clients){
                    //Broadcast player states
                    
                    enet_peer_send(c, 0, packet);
                    
                   
                }
                enet_host_flush(lobbyServer);

                PlayerState* swState = new SwordDeployState();
                gd.players_[playerID].handleState(swState, gd);


            }
        }
    } else if (key == 2 && buttonDown) { // grapple
        glm::vec2 pOffset = findCenterOfSprite(gd.players_[playerID]);
        glm::vec2 hOffset = findCenterOfSprite(gd.players_[playerID].hook);
        // float xDist = xMouse - (gd.players_[playerID].pos.x - xViewport + pOffset.x); // A
        // float yDist = yMouse - (gd.players_[playerID].pos.y - yViewport + pOffset.y); // O
        // float dist = std::sqrt(xDist * xDist + yDist * yDist); // distance formula, H
        // float aH = xDist / dist; // cos
        // float oH = yDist / dist; // sin

        gd.players_[playerID].hook.pos = gd.players_[playerID].pos + hOffset;
        gd.players_[playerID].hook.visible = true;
        gd.players_[playerID].hook.vel = 500.0f * glm::vec2(aH, oH);
        
    
    } else if (key == 2 && !buttonDown) { // grapple release 
        gd.players_[gd.playerIndex].aH = -1;
        gd.players_[gd.playerIndex].oH = -1;
        
        if (gd.players_[playerID].hook.collided) { // get out
            PlayerState* jState = new JumpState();
            gd.players_[playerID].handleState(jState, gd);
        }
        removeHook(gd.players_[playerID]);
    
    }
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
                    int playerID, int keyID, int keyDown, float deltaTime, ENetHost* lobbyServer, std::vector<ENetPeer *> clients) {

    
    gd.players_[playerID].handleInput(state, gd, keyID, keyDown, deltaTime); 
    
    if (keyID==SDL_SCANCODE_Q && keyDown && !gd.players_[playerID].hasItem) {
        printf("no item used\n");
    }

    if (keyID==SDL_SCANCODE_Q && keyDown && gd.players_[playerID].hasItem) {
        printf("item used\n");
        Item* item = gd.players_[playerID].heldItem;

        std::string itemMessage = "I " + std::to_string(playerID) + " " + std::to_string(item->type);
         ENetPacket * packet = enet_packet_create(itemMessage.c_str(), itemMessage.size()+1, ENET_PACKET_FLAG_RELIABLE);
        for(ENetPeer * c : clients){
            //Send info about item
           
            enet_peer_send(c, 0, packet);
            
            
        }
        enet_host_flush(lobbyServer);
        
        if(item->type != FOG){
            item->useItem(state, gd, gd.players_[playerID]);
        }
        gd.players_[playerID].hasItem = false;
        clearItem(state, gd);
    }
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
