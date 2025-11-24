#define ENET_IMPLEMENTATION
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <functional>
#include <sstream>
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
#include "../headers/clientHelpers.h"


//Globals for Game
GameState * currState;
using namespace std;

//Globals for Networking
uint64_t lastNetUpdate = 0;
const float netTick = 1.0f / 20.0f; //20 Updates per second
ENetPeer* serverPeer = nullptr;
int pendingLobby = -1;
bool inLobby = false;
ENetHost * client;

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

    state.xRatio = (float)state.logW / state.width; 
    state.yRatio = (float)state.logH / state.height;

    if (!initialize(state)) {
        return 1;
    }
    
    // load game assets
    Resources res;
    res.load(state);

    //initialize SDL text
    if (TTF_Init() < 0) {
        SDL_Log("SDL_ttf init failed");
        return 1;
    }
    
    srand(time(0)); // randomize item generation

    //Initial Game State
    //CHANGE if testing a different screen and you want it up on start

    currState = new TitleState();
    currState->currStateVal =  TITLE;
    //currState->nextStateVal = CHAR_SELECT;
    currState->init = initTitle;
    currState->update = titleUpdate;
    currState->render = drawTitle;
    currState->input = titleInput;
    
    // currState = new SettingsState();
    // currState->currStateVal =  SETTINGS;
    // currState->nextStateVal = SNOW;
    // currState->init = initSettings;
    // currState->update = settingsUpdate;
    // currState->render = drawSettings;
    // currState->input = settingsInputs;
    
        // currState = new LevelState();
        // currState->currStateVal = SNOW;
        // ((LevelState*)currState)->character = JETPACK;
        // currState->nextStateVal = SPACESHIP;
        // currState->init = createTilesSnow;
        // currState->update = levelUpdate;
        // currState->render = drawLevel;
        // currState->input = levelInputs;

    // currState = new CharSelectState();
    // currState->currStateVal =  CHAR_SELECT;
    // //currState->nextStateVal = CHAR_SELECT;
    // currState->init = initCharSelect;
    // currState->update = charSelectUpdate;
    // currState->render = drawCharSelect;
    // currState->input = charSelectInputs;

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
    
    // Rei IP = 100.111.250.61
    // Ellie IP = 100.91.68.8
    // Danny IP = 100.115.232.114
    // Ryan IP = 100.90.167.105

    //Set IP Here
    enet_address_set_host(&clientAddress, "100.91.68.8");
    clientAddress.port = 0; // OS chooses port
    client = enet_host_create(&clientAddress, 1, 2, 0, 0);
    if (!client) {
        printf("Bad client creation\n");
        return -1;
    }

    //Set your host values, including IP address and port
    ENetAddress address;
    //IP Address changes per person
    enet_address_set_host(&address, "100.89.84.24");
    address.port = 1233;

    //Attempt connection to the server
    serverPeer = enet_host_connect(client, &address, 2, 0);
    if (!serverPeer) {
        printf("Bad connection\n");
        return -1;
    }
    
    ENetEvent event;

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
            //std::string message;
            //printf("Handling message %d\n", currState->currStateVal);
            if(!inLobby){                             //Message handling for Matchmaker Server Conection
                switch(event.type){
                    case ENET_EVENT_TYPE_CONNECT:{
                        printf("Connected to matchmaker!\n");
                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE: {
                        std::string message((char *) event.packet->data, event.packet->dataLength);
                        printf("%s\n", message.c_str());
                        enet_packet_destroy(event.packet);
                        if(message.find("LOBBY_PORT ") != std::string::npos){
                            pendingLobby = std::stoi(message.substr(11,5));
                            
                            if (pendingLobby != -1){
                                enet_peer_disconnect(serverPeer, 0);
                            }
                        }  else if (message.find("LOBBIES ") != std::string::npos){
                            for (Lobby * l : gd.md.publicLobbies_) {
        delete l;
    }
    for (Lobby * l : gd.md.privateLobbies_) {
        delete l;
    }
    gd.md.publicLobbies_.clear();
    gd.md.privateLobbies_.clear();
    message = message.substr(8); // Get rid of "LOBBIES " in the message
    Lobby * newLobby;
    std::stringstream stream(message);
    std::string lobbyString;
    char delim = ';'; // Delimiter for separate lobby entries
    while (getline(stream, lobbyString, delim)) {
        printf("LobbyStr = %s\n", lobbyString.c_str());
        newLobby = getLobbyFromString(lobbyString);
        if (newLobby->passwordHash == 0) {
            gd.md.publicLobbies_.push_back(newLobby);
        }
        else {
            gd.md.privateLobbies_.push_back(newLobby);
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
             gd.md.publicLobbies_.push_back(lobby);
        } 
        else {
            lobby->passwordHash = std::hash<std::string>{}(std::to_string(i));
            gd.md.privateLobbies_.push_back(lobby);
        }
    }
                        }
                        break;
                    }
                    case ENET_EVENT_TYPE_DISCONNECT: {
                        printf("Changing to Lobby\n");
                        if(pendingLobby != -1){
                            address.port = pendingLobby;
                            serverPeer = enet_host_connect(client, &address,2,0);
                            pendingLobby = -1;
                            inLobby = true;
                        }
                        break;
                    }
                
                }
            } else {                                            //Message handling for lobby
                if(currState->currStateVal == CHAR_SELECT){
                    printf("Message\n");
                    charSelectMessageHandler(&event, &gd, res, state);
                } else if (currState->currStateVal == SPACESHIP){
                    levelMessageHandler(&event, &gd, res, state);
                } else if (currState->currStateVal == GRASSLANDS){
                    levelMessageHandler(&event, &gd, res, state);
                } else if(currState->currStateVal == SNOW){
                    levelMessageHandler(&event, &gd, res, state);
                }else {
                    printf("Message fell through %d\n", currState->currStateVal);
                    switch(event.type){
                        case ENET_EVENT_TYPE_CONNECT:{
                            printf("Connected to matchmaker!\n");
                            break;
                        }
                        case ENET_EVENT_TYPE_RECEIVE: {
                            std::string message((char *) event.packet->data, event.packet->dataLength);
                            printf("%s\n", message.c_str());
                            break;
                        }
                    }
                }
                
            }
        }

        //Calls functions related to the current GameState
        //printf("Input\n");
        currState->input(state, gd, res, deltaTime);
        //printf("Update\n");
        currState->update(state, gd, res, deltaTime);
        //printf("Draw\n");
        currState->render(state, gd, res, deltaTime);
        //printf("draw done\n");
        //printf("mouseX: %f, mouseY: %f\n", gd.mouseCoords.x, gd.mouseCoords.y);
        if (gd.debugMode) {
        // debug info
            SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
            SDL_RenderDebugText(state.renderer, 5, 5,
                            std::format("FPS: {}, State: {}, Grounded: {}, X: {}, Y: {}", 
                            static_cast<int>(FPS), getStateFromEnum(gd.players_[gd.playerIndex].state_->stateVal), gd.players_[gd.playerIndex].grounded, gd.players_[gd.playerIndex].pos.x, gd.players_[gd.playerIndex].pos.y).c_str());
        }

        //swap buffers and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
    }

    delete currState;
    res.unload();
    cleanup(state);
    enet_host_destroy(client);
    return 0;
}