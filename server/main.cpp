#define ENET_IMPLEMENTATION
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
//#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
//#include <format>
#include <set>
#include <unistd.h>

#include "../serverHeaders/initState.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/resources.h"
#include "../serverHeaders/player.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/createTiles.h"
#include "../serverHeaders/updateMenu.h"
#include "../serverHeaders/updateLevel.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/drawMenu.h"
#include "../serverHeaders/drawLevel.h"
#include "../serverHeaders/state.h"
#include "../serverHeaders/playerState.h"
#include "../serverHeaders/menu.h"

#include "../serverHeaders/enet.h"

//int fork();

//Globals for Game
GameState * currState;
using namespace std;

//Globals for Networking
struct Lobby{
    int id;
    int port;
    std::string hostName;
    int playerCount;
};

const int startPort = 40000;
const int endPort = 41000;
int currLobbyID = 1;

std::vector<Lobby> lobbies;
std::set<int> usedPorts;

int createLobbyServer(int port){
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    ENetHost * lobbyServer = enet_host_create(&address, 8, 2, 0, 0);

    if (!lobbyServer){
        enet_deinitialize();
        return EXIT_FAILURE;
    }

    ENetEvent event;
    //int nextLobbyPort = 0;
    std::vector<Lobby> lobbies;

    while(true){
        //Checks if there is ever a point where more than 0 events happen in 1000ms
        while(enet_host_service(lobbyServer, &event, 10) > 0){
            std::string message;
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                 {   //Do the things you do when you connect
                    printf("Client connected to a Lobby Server\n");
                    break;
                 }
                case ENET_EVENT_TYPE_RECEIVE:
                 {   //Add conditionals to do what server says
                    printf("input received by client\n");
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                  {  //Do the things to do when disconnecting
                    printf("player left game\n");
                  }
            }
        }
    }
}

int getAvailablePort(){
    for(int port = startPort; port < endPort; ++port){
        if(usedPorts.find(port) == usedPorts.end()){
            usedPorts.insert(port);
            printf("Getting Port\n");
            return port;
        }
    }
    return -1;
}

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


    //Create your ENET Server
    ENetAddress address = {};
    enet_address_set_host(&address, "100.76.236.38");
    //address.host = ENET_HOST_ANY;
    address.port = 1233;

    ENetHost * matchmakerServer = enet_host_create(&address, 32, 2, 0, 0);
    
    if (!matchmakerServer){
        printf("failed to make server\n");
        enet_deinitialize();
        return EXIT_FAILURE;
    }

    // Start Server loop
    
    while (running) {
        ENetEvent event;
        uint64_t nowTime = SDL_GetTicks(); // take time from previous frame to calculate delta
        frames++;
        if (nowTime > lastTime + 1000) { // fps counter
            lastTime = nowTime;
            FPS = frames;           
            frames = 0;
        }
        float deltaTime = (nowTime - prevTime) / 1000.0f; 
        

        while(enet_host_service(matchmakerServer, &event, 0) > 0){
            std::cout << "In loop\n";
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    {
                        std::cout << "Client Connected\n";
                    }
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    printf("Here\n");
                    std::string message((char *) event.packet->data, event.packet->dataLength);
                    //printf("%s\n", message.c_str());
                    enet_packet_destroy(event.packet);
                    printf("%s\n", message.c_str());
                    //Check what the message was
                    if(message == (std::string) "CREATE_LOBBY"){
                        printf("Creating lobby\n");
                        //Fork and create lobby process
                        int lobbyPort = getAvailablePort();
                        printf("%d\n", lobbyPort);
                        Lobby newLobby = {currLobbyID++, lobbyPort, (std::string)"Ellie", 1};
                    
                        lobbies.push_back(newLobby);
                        pid_t pid = fork();

                        if(pid == 0){ //Child Process
                            //Destroy file descriptor for matchmaker server in the child
                            enet_host_destroy(matchmakerServer);
                            createLobbyServer(lobbyPort);
                            enet_deinitialize();
                            return 0;
                        } else if (pid > 0){//Parent Process
                            //Send new port to client
                            std::string replyMessage = "LOBBY_PORT " + std::to_string(lobbyPort);
                            ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet);
                            enet_host_flush(matchmakerServer);
                        } else {
                            printf("FAILED FORK\n");
                        }
                        
                    } else if(message == (std::string) "JOIN_LOBBY"){
                        std::string lobbiesReturnMessage = "";
                        for (Lobby l : lobbies){
                            lobbiesReturnMessage += std::to_string(l.id) + " " + std::to_string(l.port) + " " + l.hostName + " " + std::to_string(l.playerCount) + "\n";
                            ENetPacket* packet = enet_packet_create(lobbiesReturnMessage.c_str(), lobbiesReturnMessage.size(), ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet);
                        }
                    } else if(message.find("JOIN ") != std::string::npos){
                        //Client indicated desire to join a lobby "JOIN X"
                        int desiredLobbyID = std::stoi(message.substr(5, message.length() - 5));
                        printf("desiredLobbyID: %d", desiredLobbyID);
                        int lobbyPort = -1;
                        for (Lobby &l: lobbies){
                            if(l.id == desiredLobbyID && l.playerCount < 8){
                                lobbyPort = l.port;
                                l.playerCount++;
                            }
                        }
                        std::string replyMessage = "LOBBY_PORT " + std::to_string(lobbyPort);
                        ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(event.peer, 0, packet);
                        enet_host_flush(matchmakerServer);


                    }
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    printf("Client left :(\n");
                }
            }
        }
        //usleep(500);
        

        //Calls functions related to the current GameState
        currState->input(state, gd, res, deltaTime);
        currState->update(state, gd, res, deltaTime);
        //currState->render(state, gd, res, deltaTime);

        // if (gd.debugMode) {
        // // debug info
        //     SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
        //     SDL_RenderDebugText(state.renderer, 5, 5,
        //                     std::format("FPS: {}, State: {}, Grounded: {}, X: {}, Y: {}", 
        //                     static_cast<int>(FPS), getStateFromEnum(gd.players_[0].state_->stateVal), gd.players_[0].grounded, gd.mapViewport.x, gd.mapViewport.y).c_str());
        // }
        //swap buffers and present
        SDL_RenderPresent(state.renderer);
        prevTime = nowTime;
        }
        
    

    delete currState;
    res.unload();
    cleanup(state);
    enet_deinitialize();
    return 0;
}