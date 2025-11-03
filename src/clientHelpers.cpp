//#pragma once
#include <string>

#include "../headers/GameData.h"
#include "../headers/clientHelpers.h"
#include "../headers/state.h"
extern int pendingLobby;
extern GameState * currState;


void charSelectMessageHandler(ENetEvent * event, GameData * gd, Resources &res, SDLState &state){
    //std::string message;
    printf("Handling message of type %d\n", event->type);
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
                
                //PARSE MESSAGE TO DETERMINE DISPLAY INFORMATION HERE
            
            }
            break;
        }
    }
}