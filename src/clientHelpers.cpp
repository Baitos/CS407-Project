//#pragma once
#include <string>

#include "../headers/gameData.h"
#include "../headers/clientHelpers.h"
#include "../headers/state.h"
extern int pendingLobby;
extern GameState * currState;


void charSelectMessageHandler(ENetEvent * event, GameData * gd, Resources &res, SDLState &state){
    //std::string message;
    //printf("Handling message of type %d\n", event->type);
    switch(event->type){
        case ENET_EVENT_TYPE_CONNECT:{
            printf("CONNECTED2\n");
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
                gd->playerIndex = std::stoi((message).substr(10));
                printf("New ID\n");
            } else if(message.find("CHANGE_STATE ") != std::string::npos){
                if ( std::stoi((message).substr(13)) == 0){
                    currState->nextStateVal = GRASSLANDS;
                } else if (std::stoi((message).substr(13)) == 1){
                    currState->nextStateVal = SPACESHIP;
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
                if(parts.size() == 1+(gd->players_.size()*12)){
                    //printf("%s\n", message.c_str());
                    for(int i = 0; i < gd->players_.size(); i++){
                        gd->players_[std::stoi(parts[12*i+1])].pos.x = std::stof(parts[12*i+2]);
                        gd->players_[std::stoi(parts[12*i+1])].pos.y = std::stof(parts[12*i+3]);
                        gd->players_[std::stoi(parts[12*i+1])].vel.x = std::stof(parts[12*i+4]);
                        gd->players_[std::stoi(parts[12*i+1])].vel.y = std::stof(parts[12*i+5]);
                        //printf("%d to %d\n", gd->players_[std::stoi(parts[12*i+1])].state_->stateVal, std::stoi(parts[12*i+6]));
                        if((std::stoi(parts[12*i+6]) == LAUNCH || std::stoi(parts[12*i+6]) == FASTFALL || std::stoi(parts[12*i+6]) == JUMP || std::stoi(parts[12*i+6]) == SPRINT) && gd->players_[std::stoi(parts[12*i+1])].state_->stateVal !=  std::stoi(parts[12*i+6])){
                            
                            if(std::stoi(parts[12*i+6]) == LAUNCH){
                                PlayerState *newState = new LaunchState();
                                gd->players_[std::stoi(parts[12*i+1])].handleState(newState, *gd, res);
                            } else if (std::stoi(parts[12*i+6]) == JUMP){
                                if(gd->players_[std::stoi(parts[12*i+1])].state_->stateVal != LAUNCH){
                                    PlayerState *newState = new LaunchState();
                                    gd->players_[std::stoi(parts[12*i+1])].handleState(newState, *gd, res);
                                } 
                            } else if((int)std::stof(parts[12*i+6]) == FASTFALL){
                                PlayerState *newState = new FastfallState();
                                gd->players_[std::stoi(parts[12*i+1])].handleState(newState, *gd, res);
                            } else if((int)std::stof(parts[12*i+6]) == SPRINT){
                                PlayerState *newState = new SprintState();
                                gd->players_[std::stoi(parts[12*i+1])].handleState(newState, *gd, res);
                            }
                            
                        }
                        if(gd->players_[std::stoi(parts[12*i+1])].dir != std::stof(parts[12*i+7])){
                            gd->players_[std::stoi(parts[12*i+1])].dir = std::stof(parts[12*i+7]);
                        }
                        
                        if(gd->players_[std::stoi(parts[12*i+1])].canDoubleJump != std::stoi(parts[12*i+8])){
                            gd->players_[std::stoi(parts[12*i+1])].canDoubleJump = std::stoi(parts[12*i+8]);
                        }
                        
                        if(gd->players_[std::stoi(parts[12*i+1])].grounded != std::stoi(parts[12*i+9])){
                            gd->players_[std::stoi(parts[12*i+1])].grounded = std::stoi(parts[12*i+9]);
                        }

                        if(gd->players_[std::stoi(parts[12*i+1])].isStunned != std::stoi(parts[12*i+10])){
                            gd->players_[std::stoi(parts[12*i+1])].isStunned = std::stoi(parts[12*i+10]);
                        }

                        if(gd->players_[std::stoi(parts[12*i+1])].isDead != std::stoi(parts[12*i+11])){
                            gd->players_[std::stoi(parts[12*i+1])].isDead = std::stoi(parts[12*i+11]);
                        }

                        if(gd->players_[std::stoi(parts[12*i+1])].currentDirection != std::stoi(parts[12*i+12])){
                            gd->players_[std::stoi(parts[12*i+1])].currentDirection = std::stoi(parts[12*i+12]);
                        }

                    }
                }

                
            
            }
            break;
        }
    }
}

void joinMessageHandler(ENetEvent * event, GameData * gd, Resources &res, SDLState &state){
    switch(event->type){
        case ENET_EVENT_TYPE_CONNECT:{
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE: {
            
            std::string message((char *) event->packet->data, event->packet->dataLength);
            enet_packet_destroy(event->packet);
            
            if(message.find("LOBBIES ") != std::string::npos){
                printf("%s\n", message.c_str());
                //REI TODO - PARSE MESSAGE TO DETERMINE DISPLAY INFORMATION HERE
            
            }
            break;
        }
    }
}