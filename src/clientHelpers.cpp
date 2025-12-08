//#pragma once
#include <string>
#include <sstream>
#include "../headers/gameData.h"
#include "../headers/clientHelpers.h"
#include "../headers/state.h"
//#include "../headers/item.h"




extern int pendingLobby;
extern GameState * currState;
extern bool reconnectMatchmaker;


void charSelectMessageHandler(ENetEvent * event, GameData * gd, Resources &res, SDLState &state){
    //std::string message;
    //printf("Handling message of type %d\n", event->type);
    switch(event->type){
        case ENET_EVENT_TYPE_CONNECT:{
            reconnectMatchmaker = false;
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE: {
            
            std::string message((char *) event->packet->data, event->packet->dataLength);
            enet_packet_destroy(event->packet);

            printf("Message in handler: %s\n", message.c_str());
            
            if(message.find("CLASS ") != std::string::npos){        //Player updated their class
                printf("%s\n", message.c_str());
                
                gd->playerTypes[std::stoi(message.substr(6,1))] = std::stoi(message.substr(8,1));
                
                if(std::stoi(message.substr(6,1)) != gd->playerIndex || gd->md.charIcons_[gd->playerIndex].spriteFrame == 4){
                    if (std::stoi(message.substr(8,1)) == SHOTGUN){
                        gd->md.charIcons_[std::stoi(message.substr(6,1))].spriteFrame = 1;
                    } else if(std::stoi(message.substr(8,1)) == SWORD){
                        gd->md.charIcons_[std::stoi(message.substr(6,1))].spriteFrame = 3;
                    } else {
                        //printf("MESSAGE  NUMBER: %s\n",message.substr(8,1) );
                        gd->md.charIcons_[std::stoi(message.substr(6,1))].spriteFrame = 2;
                    }
                    if (std::stoi(message.substr(6,1)) == gd->playerIndex){
                        gd->md.charIcons_[std::stoi(message.substr(6,1))].spriteFrame+=4;
                    }
                }   
            } else if(message.find("PLAYER_ID ") != std::string::npos){
                if(gd->playerIndex ==-1){
                    gd->playerIndex = std::stoi((message).substr(10));
                }
                printf("New ID\n");
            } else if(message.find("CHANGE_STATE ") != std::string::npos){
                if ( std::stoi((message).substr(13)) == 0){
                    currState->nextStateVal = GRASSLANDS;
                } else if (std::stoi((message).substr(13)) == 1){
                    currState->nextStateVal = SPACESHIP;
                } else if (std::stoi((message).substr(13)) == 2){
                    currState->nextStateVal = SNOW;
                }
                printf("here\n");
                currState = changeState(currState, *gd);
                printf("here2\n");
                currState->init(state,*gd,res);
            }
            break;
        }
    }
}


void levelMessageHandler(ENetEvent * event, GameData * gd, Resources &res, SDLState &state){
    switch(event->type){
        case ENET_EVENT_TYPE_CONNECT:{
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE: {
            
            std::string message((char *) event->packet->data, event->packet->dataLength);
            enet_packet_destroy(event->packet);
            
            if(message.find("U ") != std::string::npos){
                std::vector<std::string> parts;

                std::string word;
                for (char c : message) {
                    if (c == ' ') {
                        if (!word.empty()) {
                            parts.push_back(word);
                            word.clear();
                        }
                    } else {
                        word += c;
                    }
                }
                if (!word.empty()){
                    parts.push_back(word);
                }
                if(parts.size() == 1+(gd->players_.size()*16)){
                    //printf("%s\n", parts[12*0 + 6].c_str());
                    for(int i = 0; i < gd->players_.size(); i++){
                        //printf("Original Values: %f %f %f %f\n", gd->players_[std::stoi(parts[16*i+1])].pos.x,  gd->players_[std::stoi(parts[16*i+1])].pos.y, gd->players_[std::stoi(parts[16*i+1])].vel.x , gd->players_[std::stoi(parts[16*i+1])].vel.y);
                        gd->players_[std::stoi(parts[16*i+1])].pos.x = std::stof(parts[16*i+2]);
                        gd->players_[std::stoi(parts[16*i+1])].pos.y = std::stof(parts[16*i+3]);
                        gd->players_[std::stoi(parts[16*i+1])].vel.x = std::stof(parts[16*i+4]);
                        gd->players_[std::stoi(parts[16*i+1])].vel.y = std::stof(parts[16*i+5]);
                        //printf("Updated: %f %f %f %f\n", gd->players_[std::stoi(parts[16*i+1])].pos.x,  gd->players_[std::stoi(parts[16*i+1])].pos.y, gd->players_[std::stoi(parts[16*i+1])].vel.x , gd->players_[std::stoi(parts[16*i+1])].vel.y);
                        //printf("%d to %d\n", gd->players_[std::stoi(parts[16*i+1])].state_->stateVal, std::stoi(parts[16*i+6]));
                        if(gd->players_[std::stoi(parts[16*i+1])].state_->stateVal !=  JETPACK_DEPLOY || gd->players_[std::stoi(parts[16*i+1])].state_->stateVal !=  SWORD_DEPLOY){
                            if((std::stoi(parts[16*i+6]) == LAUNCH || std::stoi(parts[16*i+6]) == FASTFALL || std::stoi(parts[16*i+6]) == JUMP || std::stoi(parts[16*i+6]) == SPRINT || std::stoi(parts[16*i+6]) == JETPACK_DEPLOY || std::stoi(parts[16*i+6]) == GRAPPLE) && gd->players_[std::stoi(parts[16*i+1])].state_->stateVal !=  std::stoi(parts[16*i+6])){
                                
                                if(std::stoi(parts[16*i+6]) == LAUNCH){
                                    PlayerState *newState = new LaunchState();
                                    gd->players_[std::stoi(parts[16*i+1])].handleState(newState, *gd, res);
                                } else if (std::stoi(parts[16*i+6]) == JUMP){
                                    if(gd->players_[std::stoi(parts[16*i+1])].state_->stateVal != LAUNCH){
                                        PlayerState *newState = new LaunchState();
                                        gd->players_[std::stoi(parts[16*i+1])].handleState(newState, *gd, res);
                                    } 
                                } else if((int)std::stof(parts[16*i+6]) == FASTFALL){
                                    PlayerState *newState = new FastfallState();
                                    gd->players_[std::stoi(parts[16*i+1])].handleState(newState, *gd, res);
                                } else if((int)std::stof(parts[16*i+6]) == SPRINT){
                                    PlayerState *newState = new SprintState();
                                    gd->players_[std::stoi(parts[16*i+1])].handleState(newState, *gd, res);
                                } else if ((int)std::stof(parts[16*i+6]) == JETPACK_DEPLOY){
                                    PlayerState *newState = new JetpackDeployState();
                                    gd->players_[std::stoi(parts[16*i+1])].handleState(newState, *gd, res);
                                } else if ((int)std::stof(parts[16*i+6]) == GRAPPLE){
                                    PlayerState *newState = new GrappleState();
                                    gd->players_[std::stoi(parts[16*i+1])].handleState(newState, *gd, res);
                                } 
                                
                            }
                        }
                        if(gd->players_[std::stoi(parts[16*i+1])].dir != std::stof(parts[16*i+7])){
                            gd->players_[std::stoi(parts[16*i+1])].dir = std::stof(parts[16*i+7]);
                        }
                        
                        if(gd->players_[std::stoi(parts[16*i+1])].canDoubleJump != std::stoi(parts[16*i+8])){
                            gd->players_[std::stoi(parts[16*i+1])].canDoubleJump = std::stoi(parts[16*i+8]);
                        }
                        
                        if(gd->players_[std::stoi(parts[16*i+1])].grounded != std::stoi(parts[16*i+9])){
                            gd->players_[std::stoi(parts[16*i+1])].grounded = std::stoi(parts[16*i+9]);
                        }

                        if(gd->players_[std::stoi(parts[16*i+1])].isStunned != std::stoi(parts[16*i+10])){
                            gd->players_[std::stoi(parts[16*i+1])].isStunned = std::stoi(parts[16*i+10]);
                        }

                        if(gd->players_[std::stoi(parts[16*i+1])].isDead != std::stoi(parts[16*i+11])){
                            gd->players_[std::stoi(parts[16*i+1])].isDead = std::stoi(parts[16*i+11]);
                        }

                        if(gd->players_[std::stoi(parts[16*i+1])].currentDirection != std::stoi(parts[16*i+12])){
                            gd->players_[std::stoi(parts[16*i+1])].currentDirection = std::stoi(parts[16*i+12]);
                        }

                        gd->players_[std::stoi(parts[16*i+1])].hook.pos.x = std::stof(parts[16*i+13]);
                        gd->players_[std::stoi(parts[16*i+1])].hook.pos.y = std::stof(parts[16*i+14]);
                        gd->players_[std::stoi(parts[16*i+1])].hook.vel.x = std::stof(parts[16*i+15]);
                        gd->players_[std::stoi(parts[16*i+1])].hook.vel.y = std::stof(parts[16*i+16]);
                        if(gd->players_[std::stoi(parts[16*i+1])].hook.pos.x > 0 && gd->players_[std::stoi(parts[16*i+1])].hook.pos.y >= 0){
                            gd->players_[std::stoi(parts[16*i+1])].hook.visible = true;
                        } else {
                            gd->players_[std::stoi(parts[16*i+1])].hook.visible = false;
                        }
                        

                    }
                }
            
            } else if(message.find("I ") != std::string::npos){
                printf("item used by player\n");
                if(std::stoi(message.substr(2,1)) != gd->playerIndex){
                    SDL_FRect collider = {
                        .x = 0,
                        .y = 0,
                        .w = float(TILE_SIZE),
                        .h = float(TILE_SIZE)
                    };
                    Item * newItem;
                    if (std::stoi(message.substr(4,1)) == PIE){
                        newItem = new PieItem(gd->players_[std::stoi(message.substr(2,1))].pos, collider, res.texPie);
                    } else if (std::stoi(message.substr(4,1)) == BOOMBOX){
                        newItem = new Boombox(gd->players_[std::stoi(message.substr(2,1))].pos, collider, res.texBoombox);
                    } else if (std::stoi(message.substr(4,1)) == ICE){
                        newItem = new Ice(gd->players_[std::stoi(message.substr(2,1))].pos, collider, res.texIce);
                    } else if (std::stoi(message.substr(4,1)) == SUGAR){
                        newItem = new Sugar(gd->players_[std::stoi(message.substr(2,1))].pos, collider, res.texSugar);
                    } else if (std::stoi(message.substr(4,1)) == BOMB){
                        newItem = new Bomb(gd->players_[std::stoi(message.substr(2,1))].pos, collider, res.texBomb);
                    } else if (std::stoi(message.substr(4,1)) == FOG){
                        newItem = new Fog(gd->players_[std::stoi(message.substr(2,1))].pos, collider, res.texFog);
                    }
                    gd->players_[std::stoi(message.substr(2,1))].heldItem = newItem;
                    gd->players_[std::stoi(message.substr(2,1))].heldItem->useItem(state,*gd,res,gd->players_[std::stoi(message.substr(2,1))]);
                    gd->players_[std::stoi(message.substr(2,1))].hasItem = false;
                    
                }
                
            } else if(message.find("IM ") != std::string::npos){
                if(std::stoi(message.substr(3,1)) == gd->playerIndex){
                    printf("%s\n", message.c_str());
                    SDL_FRect collider = {
                        .x = 0,
                        .y = 0,
                        .w = float(TILE_SIZE),
                        .h = float(TILE_SIZE)
                    };
                    Item * newItem;
                    if (std::stoi(message.substr(5,1)) == PIE){
                        newItem = new PieItem(gd->players_[gd->playerIndex].pos, collider, res.texPie);
                    } else if (std::stoi(message.substr(5,1)) == BOOMBOX){
                        newItem = new Boombox(gd->players_[gd->playerIndex].pos, collider, res.texBoombox);
                    } else if (std::stoi(message.substr(5,1)) == ICE){
                        newItem = new Ice(gd->players_[gd->playerIndex].pos, collider, res.texIce);
                    } else if (std::stoi(message.substr(5,1)) == SUGAR){
                        newItem = new Sugar(gd->players_[gd->playerIndex].pos, collider, res.texSugar);
                    } else if (std::stoi(message.substr(5,1)) == BOMB){
                        newItem = new Bomb(gd->players_[gd->playerIndex].pos, collider, res.texBomb);
                    } else if(std::stoi(message.substr(5,1)) == FOG){
                        newItem = new Fog(gd->players_[gd->playerIndex].pos, collider, res.texFogStorage);
                    } else {
                        printf("Bad item id\n");
                    }
                    if (gd->players_[gd->playerIndex].heldItem != nullptr) {
                        delete gd->players_[gd->playerIndex].heldItem;
                    }
                    gd->players_[gd->playerIndex].heldItem = newItem;
                    //gd->players_[gd->playerIndex].hasItem = true;
                    gd->players_[gd->playerIndex].itemMessageReceived = true;
                }
            } else if(message.find("SHOT ") != std::string::npos){
                PlayerState *newState = new ShotgunDeployState();
                gd->players_[std::stoi(message.substr(5,1))].handleState(newState, *gd, res);            
            } else if (message.find("SWORD ") != std::string::npos){
                PlayerState *newState = new SwordDeployState();
                gd->players_[std::stoi(message.substr(6,1))].handleState(newState, *gd, res);     

            }
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT:
        {
            printf("Disconnect\n");
            break;

        }
    }
}

void joinMessageHandler(GameData * gd, std::string message){
    for (Lobby * l : gd->md.publicLobbies_) {
        delete l;
    }
    for (Lobby * l : gd->md.privateLobbies_) {
        delete l;
    }
    gd->md.publicLobbies_.clear();
    gd->md.privateLobbies_.clear();
    message = message.substr(8); // Get rid of "LOBBIES " in the message
    Lobby * newLobby;
    std::stringstream stream(message);
    std::string lobbyString;
    char delim = ';'; // Delimiter for separate lobby entries
    while (getline(stream, lobbyString, delim)) {
        printf("LobbyStr = %s\n", lobbyString.c_str());
        newLobby = getLobbyFromString(lobbyString);
        if (newLobby->passwordHash == 0) {
            gd->md.publicLobbies_.push_back(newLobby);
        }
        else {
            gd->md.privateLobbies_.push_back(newLobby);
        }
    }
    //JOIN LOBBY TESTING
    Lobby * lobby;
    for (int i = 0; i < 24; i++) {
        lobby = new Lobby();
        lobby->id = i;
        lobby->port = 40000 + i;
        lobby->hostName = "TestUser" + std::to_string(i);
        lobby->playerCount = i;
        if (i % 2 == 0) {
            lobby->passwordHash = 0;
             gd->md.publicLobbies_.push_back(lobby);
        } 
        else {
            lobby->passwordHash = std::hash<std::string>{}(std::to_string(i));
            gd->md.privateLobbies_.push_back(lobby);
        }
    }
}

Lobby * getLobbyFromString(std::string lobbyStr) {
    // ID,PORT,HOSTNAME,PLAYERCOUNT,PASSHASH,isGrandPrix[0/1],numLaps; 
    Lobby * newLobby = new Lobby();
    std::string tmp;
    std::stringstream tmpStream;
    char delim = ',';
    std::stringstream stream(lobbyStr);
    // ID
    getline(stream, tmp, delim);
    newLobby->id = atoi(tmp.c_str());
    // Port
    getline(stream, tmp, delim);
    newLobby->port = atoi(tmp.c_str());
    // Hostname
    getline(stream, tmp, delim);
    newLobby->hostName = tmp;
    // Playercount
    getline(stream, tmp, delim);
    newLobby->playerCount = atoi(tmp.c_str());
    // PassHash
    getline(stream, tmp, delim);
    tmpStream = std::stringstream(tmp);
    tmpStream >> newLobby->passwordHash;
    // isGrandPrix
    getline(stream, tmp, delim);
    newLobby->isGrandPrix = (atoi(tmp.c_str()) == 1);
    // numLaps
    getline(stream, tmp, delim);
    newLobby->numLaps = atoi(tmp.c_str());
    return newLobby;
}
