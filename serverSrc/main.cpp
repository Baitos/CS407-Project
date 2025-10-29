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

void createLobbyServer(int port){
    printf("Game lobby server\n");
    
    
    SDLState state;
    
    
    SDL_Init(0);

    GameData gd(state);
    printf("NUMPLAYERS: %d\n", gd.numPlayers);
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    ENetHost * lobbyServer = enet_host_create(&address, 8, 2, 0, 0);

    if (!lobbyServer){
        enet_deinitialize();
        exit(1);
    }

    ENetEvent event;

    std::vector<ENetPeer *> clients;
    //Initial Game State
    currState = new CharSelectState();
    currState->nextStateVal = SPACESHIP;
    currState->update = charSelectUpdate;
    currState->input = charSelectInputs;
    
    bool ready = false;
    int readyPlayers[8] = {0};
    int mapVote[5] = {0};                       //Spaceship, Plains, Stage 3, stage 4, stage 5
    int playerClasses[8] ={0};

    while(!ready){
        //Checks if there is ever a point where more than 0 events happen in that instant
        while(enet_host_service(lobbyServer, &event, 0) > 0){
            //std::string message;
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                 {   //Do the things you do when you connect
                    printf("Client connected to a Lobby Server\n");
                    
                    clients.push_back(event.peer);
                    //Send player their player_id
                    printf("NUMPLAYERS2: %d\n", gd.numPlayers);
                    
                    //readyPlayers
                    playerClasses[gd.numPlayers] = SWORD;

                    std::string replyMessage = "PLAYER_ID " + std::to_string(gd.numPlayers++);
                    ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                    enet_peer_send(event.peer, 0, packet);
                    enet_host_flush(lobbyServer);

                    for(ENetPeer * client: clients){
                        std::string replyMessage = "CLASS " + std::to_string(gd.numPlayers-1) + " " + std::to_string(SWORD);
                        ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(client, 0, packet);
                        enet_host_flush(lobbyServer);

                    }
                    
                    for(int j = 0; j < 8; j++){
                        if(playerClasses[j] != 0){
                            std::string replyMessage = "CLASS " + std::to_string(j) + " " + std::to_string(playerClasses[j]);
                            ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet);
                            enet_host_flush(lobbyServer);
                        }
                    }

                    break;
                 }
                case ENET_EVENT_TYPE_RECEIVE:
                 {   //Add conditionals to do what server says
                    std::string message((char *) event.packet->data, event.packet->dataLength);
                    enet_packet_destroy(event.packet);
                    printf("%s\n", message.c_str());
                    
                    //Check what the message was
                    if(message.find("READY ") != std::string::npos){            //Player indicated Ready - Format "READY player_id"
                        if(!readyPlayers[std::stoi(message.substr(6,1))]){
                            readyPlayers[std::stoi(message.substr(6,1))] = 1;
                            if(std::stoi(message.substr(8,1)) == SPACESHIP){
                                mapVote[0] += 1;
                            } else {
                                mapVote[1] += 1;
                            }
                            
                            int totalReady = 0;
                            for(int ready: readyPlayers){
                                
                                totalReady += ready;
                            }
                            printf("%d %d\n", totalReady, gd.numPlayers);
                            if(totalReady == gd.numPlayers){   //If 3/4 of players agree to ready, they enter the game
                                //Send message to go to level
                                //Update Game Data
                                int mapWinner = 0;
                                for (int i = 0; i<5; i++){
                                    if( mapVote[i] > mapVote[mapWinner]){
                                        mapWinner = i;
                                    }
                                }
                                printf("READY FOR GAME\n");
                                for(ENetPeer * client : clients){
                                    std::string replyMessage = "CHANGE_STATE " + std::to_string(mapWinner);
                                    ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                                    enet_peer_send(client, 0, packet);
                                    enet_host_flush(lobbyServer);
                                }
                                ready = true;
                                break;
                                //ready = true;
            
                            }
                        }
                    } else if (message.find("CLASS ") != std::string::npos){     //Player indicated Class change - Format "CLASS player_id player_class"
                        playerClasses[std::stoi(message.substr(6,1))] = std::stoi(message.substr(8,1));
                        for(ENetPeer * client : clients){
                            std::string replyMessage = message;
                            ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(client, 0, packet);
                            enet_host_flush(lobbyServer);
                        }
                    }
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                  {  //Do the things to do when disconnecting
                    printf("player left game\n");
                  }
                  break;
            }
        }
        //currState->input(state, gd, res, deltaTime);
        //currState->update(state, gd, res, deltaTime);
    }
    
    bool inGame =true;
    while(inGame){

    }

    //Leave Lobby
    cleanup(state);
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

    //currState->init(state,gd,res);
    uint64_t prevTime = SDL_GetTicks();
    uint64_t lastTime = 0;

    //Create your ENET Server
    ENetAddress address = {};
    //Address of the Raspberry Pi
    enet_address_set_host(&address, "100.76.236.38");
    address.port = 1233;

    ENetHost * matchmakerServer = enet_host_create(&address, 32, 2, 0, 0);
    
    if (!matchmakerServer){
        printf("failed to make server\n");
        enet_deinitialize();
        return EXIT_FAILURE;
    }
    bool hasClient = false;
    // Start Server loop
    while (true) {
        ENetEvent event;
        uint64_t nowTime = SDL_GetTicks(); // take time from previous frame to calculate delta

        if (nowTime > lastTime + 1000) { // fps counter
            lastTime = nowTime;
            
        }
        float deltaTime = (nowTime - prevTime) / 1000.0f; 
        
        
        while(enet_host_service(matchmakerServer, &event, 0) > 0){
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    {
                        std::cout << "Client Connected to Matchmaker Server!\n";
                        //TESTING PURPOSES - Will be changed with menu screen inputs
                        if(!hasClient){
                            hasClient = true;
                            int lobbyPort = getAvailablePort();
                            Lobby newLobby = {currLobbyID++, lobbyPort, (std::string)"username", 1};
                        
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
                                printf("%s\n", replyMessage.c_str());
                            } else {
                                printf("FAILED FORK\n");
                            }
                        } else {
                            std::string replyMessage = "LOBBY_PORT " + std::to_string(40000);
                            ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet);
                            enet_host_flush(matchmakerServer);
                        }
                        break;
                    }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    std::string message((char *) event.packet->data, event.packet->dataLength);
                    //printf("%s\n", message.c_str());
                    enet_packet_destroy(event.packet);
                    printf("Message: %s\n", message.c_str());
                    //Check what the message was
                    if(message == (std::string) "CREATE_LOBBY"){
                        printf("Creating lobby\n");
                        //Fork and create lobby process
                        int lobbyPort = getAvailablePort();
                        printf("%d\n", lobbyPort);
                        Lobby newLobby = {currLobbyID++, lobbyPort, (std::string)"Username", 1};
                    
                        lobbies.push_back(newLobby);
                        fflush(stdout);
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
                            enet_host_flush(matchmakerServer);
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

        //Calls functions related to the current GameState
        //currState->input(state, gd, res, deltaTime);
        //currState->update(state, gd, res, deltaTime);
        //currState->render(state, gd, res, deltaTime);

        prevTime = nowTime;
        }

    delete currState;
    
    enet_deinitialize();
    return 0;
}