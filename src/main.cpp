#define ENET_IMPLEMENTATION
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/player.h"
#include "../headers/globals.h"
#include "../headers/createTiles.h"
#include "../headers/updateMenu.h"
#include "../headers/updateLevel.h"
#include "../headers/helper.h"
#include "../headers/drawMenu.h"
#include "../headers/drawLevel.h"
#include "../headers/state.h"
#include "../headers/playerState.h"
#include "../headers/menu.h"

#include "../headers/enet.h"

//Add as needed
struct PlayerUpdate{
    int playerID;
    float x;
    float y;
    float velX;
    float velY;
    PlayerState state;
};

//Globals for Game
GameState * currState;
using namespace std;

//Globals for Networking
uint64_t lastNetUpdate = 0;
const float netTick = 1.0f / 20.0f; //20 Updates per second
ENetPeer* serverPeer = nullptr;
int pendingLobby = -1;

int main(int argc, char** argv) { // SDL needs to hijack main to do stuff; include argv/argc
    
    //Initialize the ENET Library
    if (enet_initialize() != 0) {
        std::cout << "ENet initialization failed\n";
        return 1;
    }

    //Deinitialize Library when you quit
    atexit(enet_deinitialize);

    //Create Window
    SDLState state;
    state.width = 1600;
    state.height = 900;
    state.logW = 800;
    state.logH = 450;

    if (!initialize(state)) {
        return 1;
    }
    
    // load game assets
    Resources res;
    res.load(state);

    //Initial Game State
    currState = new LevelState();
    currState->nextStateVal = SPACESHIP;
    currState->init = createTiles;
    currState->update = levelUpdate;
    currState->render = drawLevel;
    currState->input = levelInputs;

    // setup game data
    GameData gd(state);
    currState->init(state,gd,res);
    uint64_t prevTime = SDL_GetTicks();

    uint64_t frames = 0;
    uint64_t FPS = 0;
    uint64_t lastTime = 0;


    //Create your ENET Client
    //ONLY NEEDS TO BE DONE WHEN JOINING/CREATING
    ENetAddress clientAddress;
    enet_address_set_host(&clientAddress, "100.74.129.69");
    clientAddress.port = 0; // OS chooses port
    ENetHost* client = enet_host_create(&clientAddress, 1, 2, 0, 0);
    if (!client) {
        printf("Bad client creation\n");
        return -1;
    }

    //Set your host values, including IP address and port
    ENetAddress address;
    //IP Address changes per person
    enet_address_set_host(&address, "100.76.236.38");
    address.port = 1233;

    //Attempt connection to the server
    serverPeer = enet_host_connect(client, &address, 2, 0);
    if (!serverPeer) {
        printf("Bad connection\n");
        return -1;
    }
    
    ENetEvent event;

    while (enet_host_service(client, &event, 100) > 0) {
            std::string message;
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {   //Do the things you do when you connect
                    printf("CONNECTED\n");
                    break;
                }
            }
        }

    // start game loop
    
    while (running) {
        uint64_t nowTime = SDL_GetTicks(); // take time from previous frame to calculate delta
        frames++;
        if (nowTime > lastTime + 1000) { // fps counter
            lastTime = nowTime;
            FPS = frames;           
            frames = 0;
        }
        float deltaTime = (nowTime - prevTime) / 1000.0f; // convert to seconds from ms

        while (enet_host_service(client, &event, 0) > 0) {
            std::string message;
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {   //Add conditionals to do what server says
                    //Draw appropriate info of packets given
                    std::string message((char *) event.packet->data, event.packet->dataLength);
                    enet_packet_destroy(event.packet);
                    //gd.players_[]
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {  //Do the things to do when disconnecting
                  
                    break;
                }
            }
        }

        //Calls functions related to the current GameState
        currState->input(state, gd, res, deltaTime);
        currState->update(state, gd, res, deltaTime);
        currState->render(state, gd, res, deltaTime);

        if (gd.debugMode) {
        // debug info
            SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
            SDL_RenderDebugText(state.renderer, 5, 5,
                            std::format("FPS: {}, State: {}, Grounded: {}, X: {}, Y: {}", 
                            static_cast<int>(FPS), getStateFromEnum(gd.players_[0].state_->stateVal), gd.players_[0].grounded, gd.mapViewport.x, gd.mapViewport.y).c_str());
        }
        //swap buffers and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;

        if ((nowTime - lastNetUpdate) / 1000.0f >= netTick) {
            lastNetUpdate = nowTime;
            PlayerUpdate playerInfo = {gd.playerIndex, gd.players_[gd.playerIndex].pos.x, gd.players_[gd.playerIndex].pos.y, gd.players_[gd.playerIndex].vel.x, gd.players_[gd.playerIndex].vel.y};

            //SEND PACKET HERE
            //std::string updateMsg = std::format(playerInfo);

            ENetPacket* packet = enet_packet_create(&playerInfo,
                                                    sizeof(playerInfo),
                                                    ENET_PACKET_FLAG_UNSEQUENCED);

            enet_peer_send(serverPeer, 0, packet);
            enet_host_flush(client); // make sure packet is sent
        }
        
    }

    delete currState;
    res.unload();
    cleanup(state);
    enet_host_destroy(client);
    return 0;
}