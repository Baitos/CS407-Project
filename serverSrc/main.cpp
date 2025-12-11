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
#include <sstream>
#include <algorithm>

#include "../serverHeaders/initState.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/player.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/createTiles.h"
#include "../serverHeaders/updateLevel.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/state.h"
#include "../serverHeaders/playerState.h"
#include "../serverHeaders/updateLevel.h"


#include "../serverHeaders/enet.h"

//int fork();

//Globals for Game
GameState * currState;
using namespace std;


//Globals for Networking
struct Lobby{ // isGrandPrix, numLaps, ID, Port, playerCount, passHash, hostName
    bool isGrandPrix;
    int numLaps;
    int id;
    int port;
    int playerCount;
    size_t passwordHash; // hash = 0 means no password
    std::string hostName;
    // Lobby info in comma delimited string format
    std::string to_string() {
        // ID,PORT,HOSTNAME,PLAYERCOUNT,PASSHASH,isGrandPrix[0/1],numLaps
        std::string lobbyStr;
        std::string grandPrix = isGrandPrix ? "1" : "0";
        lobbyStr = std::to_string(id) + "," + std::to_string(port) + "," + 
            hostName + "," + std::to_string(playerCount) + "," +
            std::to_string(passwordHash) + "," + grandPrix + "," + std::to_string(numLaps) + ";";
        return lobbyStr;
    }
};

const int startPort = 40000;
const int endPort = 41000;
int currLobbyID = 1;

std::vector<Lobby> lobbies;
std::set<int> usedPorts;

Lobby parseHostMessage(std::string message, int lobbyID, int lobbyPort) {
    size_t passHash;
    bool isGrandPrix;
    int numLaps;
    std::string hostName;
    std::string tmp;
    std::stringstream tmpStream;
    char delim = ',';

    message = message.substr(5);
    std::stringstream stream(message);
    // Get hostname
    getline(stream, hostName, delim);
    // Get password hash
    getline(stream, tmp, delim);
    tmpStream = std::stringstream(tmp);
    tmpStream >> passHash;
    // Get isGrandPrix
    getline(stream, tmp, delim);
    isGrandPrix = atoi(tmp.c_str()) == 1;
    // Get numLaps
    getline(stream, tmp, delim);
    numLaps = atoi(tmp.c_str());
    //printf("%s", message.c_str());
    printf("Creating lobby\n");
    //Fork and create lobby process
    printf("%d\n", lobbyPort);
    // isGrandPrix, numLaps, ID, Port, playerCount, passHash, hostName
    Lobby newLobby = {isGrandPrix, numLaps, lobbyID, lobbyPort, 1, passHash, hostName};
    return newLobby;
}

void createLobbyServer(int port){
    printf("Game lobby server\n");
    srand(time(0));
    std::string usernames[8];
    SDLState state;
    
    SDL_Init(0);

    uint64_t prevTime = SDL_GetTicks();
    uint64_t lastTime = 0;

    GameData gd(state);
    gd.laps_per_race = lobbies[lobbies.size()-1].numLaps;
    gd.isGrandPrix = lobbies[lobbies.size()-1].isGrandPrix;
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
    //currState->nextStateVal = SPACESHIP;
    
    bool ready = false;
    int readyPlayers[8] = {0};
    int prixReadyPlayers[8] ={0};
    int mapVote[5] = {0};                       //Spaceship, Grasslands, Stage 3, stage 4, stage 5
    int playerClasses[8] ={0};
    for (int i =0; i < 8; i++){
        playerClasses[i] = -1;
    }
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
                    gd.playerTypes[gd.numPlayers] = SWORD;

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
                        if(playerClasses[j] != -1){
                            std::string replyMessage = "CLASS " + std::to_string(j) + " " + std::to_string(playerClasses[j]);
                            ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet);
                            enet_host_flush(lobbyServer);
                            enet_packet_destroy(packet);
                            replyMessage = "USER " + std::to_string(j) + " " + usernames[j];
                            packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet);
                            enet_host_flush(lobbyServer);
                            enet_packet_destroy(packet);
                        }
                    }

                    break;
                 }
                case ENET_EVENT_TYPE_RECEIVE:
                 {   //Add conditionals to do what server says
                    std::string message((char *) event.packet->data, event.packet->dataLength);
                    enet_packet_destroy(event.packet);
                    printf("1 Message: %s\n", message.c_str());
                    
                    //Check what the message was
                    if(message.find("READY ") != std::string::npos){            //Player indicated Ready - Format "READY player_id"
                        
                        if(!readyPlayers[std::stoi(message.substr(6,1))]){
                            readyPlayers[std::stoi(message.substr(6,1))] = 1;
                            for (int i = 0; i < 5; i++){
                                if(std::stoi(message.substr(8,1)) == i){
                                    mapVote[i] += 1;
                                }
                            }
                            
                            int totalReady = 0;
                            for(int ready: readyPlayers){
                                
                                totalReady += ready;
                            }
                            //printf("%d %d\n", totalReady, gd.numPlayers);
                            printf("Total ready: %d NumPlayers: %d\n", totalReady, gd.numPlayers);
                            if(totalReady == gd.numPlayers){   //If all players agree to ready, they enter the game
                                //Send message to go to level
                                //Update Game Data
                                int mapWinner = 0;
                                for (int i = 1; i<5; i++){
                                    if( mapVote[i] > mapVote[mapWinner]){
                                        mapWinner = i;
                                    }
                                }
                                //0 is grasslands, 1 is spaceship
                                if(mapWinner == 0){
                                    printf("Grasslands\n");
                                    currState->nextStateVal =  GRASSLANDS;
                                } else if( mapWinner == 1){
                                    printf("Spaceship\n");
                                    currState->nextStateVal = SPACESHIP;
                                } else if(mapWinner == 2){
                                    printf("Snowy Resort\n");
                                    currState->nextStateVal = SNOW;
                                } else if(mapWinner == 3){
                                    currState->nextStateVal = DESERT;
                                    printf("DESERT\n");
                                }

                                currState = changeState(currState, gd);
                                printf("about to init\n");
                                currState->init(state, gd);
                                printf("here\n");
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
                    } else if(message.find("USER ") != std::string::npos){
                        usernames[std::stoi(message.substr(5,1))] = message.substr(7, message.size()-7);
                        std::string replyMessage = message;
                        ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                        for(ENetPeer * client : clients){
                            enet_peer_send(client, 0, packet);
                            enet_host_flush(lobbyServer);
                        }
                    }else if (message.find("CLASS ") != std::string::npos){     //Player indicated Class change - Format "CLASS player_id player_class"
                        gd.playerTypes[std::stoi(message.substr(6,1))] = std::stoi(message.substr(8,1));
                        playerClasses[std::stoi(message.substr(6,1))] = std::stoi(message.substr(8,1));
                        for(ENetPeer * client : clients){
                            std::string replyMessage = message;
                            ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(client, 0, packet);
                            enet_host_flush(lobbyServer);
                        }
                    } else if(message.find("RECON ") != std::string::npos){
                        printf("here\n");
                        //printf("Message: \'%s\'", message.c_str());
                        int index = std::stoi(message.substr(6,1)); 
                        auto it = std::find(clients.begin(), clients.end(), event.peer);
                        if (it != clients.end()) {
                            clients.erase(it);
                        }
                        
                        printf("%d", index);
                        clients[index] = event.peer;
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
    
    const Uint64 UPDATE_INTERVAL_MS = 30;
 
    Uint64 lastBroadcast = SDL_GetTicks();
    
    if(gd.players_[gd.playerIndex].grounded == 0){
        //printf("not grounded");
    }

    for(ENetPeer * c : clients){
        //Broadcast player states
        std::string updateMessage = "U";
        for (Player p : gd.players_){
            updateMessage += " " + std::to_string(p.index) + " ";
            updateMessage += std::to_string((p.pos.x)) + " " + std::to_string((p.pos.y)) + " ";
            updateMessage += std::to_string((p.vel.x)) + " " + std::to_string((p.vel.y)) + " ";
            updateMessage += std::to_string(p.state_->stateVal) + " ";
            updateMessage += std::to_string(p.dir) + " " + std::to_string(p.canDoubleJump) + " " + std::to_string(p.grounded) + " ";
            updateMessage += std::to_string(p.isStunned) + " " + std::to_string(p.isDead) + " " + std::to_string(p.currentDirection);
        }
        ENetPacket * packet = enet_packet_create(updateMessage.c_str(), updateMessage.size()+1, 0);             //0 means unreliable
        enet_peer_send(c, 0, packet);
        enet_host_flush(lobbyServer);
    }
    prevTime = SDL_GetTicks();
    bool inGame =true;
    bool inLobby = true;
    bool inResults = false;
    while(inLobby){
        while(inGame){
            uint64_t nowTime = SDL_GetTicks(); // take time from previous frame to calculate delta

            if (nowTime > lastTime + 1000) { // fps counter
                lastTime = nowTime;
                
            }
            float deltaTime = (nowTime - prevTime) / 1000.0f; 
            int keyID= -1;
            int keyDown = -1;
            int playerID = -1;

            while(enet_host_service(lobbyServer, &event, 0) > 0){
                //std::string message;
                switch (event.type) {
                    case ENET_EVENT_TYPE_CONNECT:
                    {   //Do the things you do when you connect
                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:
                    {   //Add conditionals to do what server says
                        std::string message((char *) event.packet->data, event.packet->dataLength);
                        enet_packet_destroy(event.packet);
                        //printf("2 Message: %s\n", message.c_str());

                        //If key input
                        if(message.find("INPUT ") != std::string::npos){                                //KEY INPUT
                            playerID = std::stoi(message.substr(6, 1));
                            keyID = std::stoi(message.substr(8, message.length()-10));
                            keyDown = std::stoi(message.substr(message.length() -2, 1));
                            if(keyDown == 1){
                                state.keys[playerID][keyID] = true;
                            } else {
                                state.keys[playerID][keyID] = false;
                            }
                            //printf("1: %f\n", gd.players_[gd.playerIndex].pos.y);
                            handleKeyInput(state, gd, playerID, keyID, keyDown, deltaTime, lobbyServer, clients);
                            //printf("key handled\n");
                        } else {                                                                        //MOUSE INPUT 
                            playerID = std::stoi(message.substr(8, 1));                                 //2 IS LMB, 1 IS RMB
                            keyID = std::stoi(message.substr(10, 1));
                            keyDown = std::stoi(message.substr(12,1));
                            if(keyID == 1){
                                handleLevelClick(state,gd,playerID, deltaTime, keyID, keyDown, 0, 0, lobbyServer, clients);
                            } else {
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
                                printf("%s\n", message.c_str());
                                float aH = std::stof(parts[4]);
                                float oH = std::stof(parts[5]);
                                gd.players_[playerID].aH = aH;
                                gd.players_[playerID].oH = oH;
                                handleLevelClick(state,gd,playerID, deltaTime, keyID, keyDown, aH, oH, lobbyServer, clients);
                            }
                        }
                        //printf("%d %d %d\n", playerID, keyID, keyDown);
                        
                        
                        //printf("2: %f\n", gd.players_[gd.playerIndex].pos.y);
                        break;
                    }
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {  //Do the things to do when disconnecting
                        printf("player left game\n");
                        inGame = false;
                    }
                    break;
                }
            }
            //printf("update start\n");
            bool itemOwner[8] = {false};
            for(int i = 0; i < gd.numPlayers; i++){
                itemOwner[i] = gd.players_[i].pickingItem;
            }
        
            currState->update(state, gd, deltaTime, keyID, keyDown, playerID);
            for(int i = 0; i < gd.numPlayers; i++){
                if(itemOwner[i] == false && gd.players_[i].pickingItem == true){
                    
                    std::string itemMessage = "IM " + std::to_string(i) + " " + std::to_string(gd.players_[i].heldItem->type);
                    printf("%s\n", itemMessage.c_str());
                    ENetPacket * packet = enet_packet_create(itemMessage.c_str(), itemMessage.size()+1, 1);             //0 means unreliable
                    enet_peer_send(clients[i], 0, packet);
                    enet_host_flush(lobbyServer);
                }
            }
            
            

            if(nowTime-lastBroadcast >= UPDATE_INTERVAL_MS){
                std::string updateMessage = "U";
                    for (Player p : gd.players_){
                        updateMessage += " " + std::to_string(p.index) + " ";
                        updateMessage += std::to_string((p.pos.x)) + " " + std::to_string((p.pos.y)) + " ";
                        updateMessage += std::to_string((p.vel.x)) + " " + std::to_string((p.vel.y)) + " ";
                        updateMessage += std::to_string(p.state_->stateVal) + " ";
                        updateMessage += std::to_string(p.dir) + " " + std::to_string(p.canDoubleJump) + " " + std::to_string(p.grounded) + " ";
                        updateMessage += std::to_string(p.isStunned) + " " + std::to_string(p.isDead) + " " + std::to_string(p.currentDirection) + " ";
                        updateMessage += std::to_string(p.hook.pos.x) + " " + std::to_string(p.hook.pos.y) + " " + std::to_string(p.hook.vel.x) + " " + std::to_string(p.hook.vel.y);
                        //printf("%s %s %s %s\n", std::to_string(p.pos.x).c_str(), std::to_string(p.pos.y).c_str(), std::to_string(p.vel.x).c_str(), std::to_string(p.vel.y).c_str());
                    }   
                    
                for(ENetPeer * c : clients){
                    //Broadcast player states
                    ENetPacket * packet = enet_packet_create(updateMessage.c_str(), updateMessage.size()+1, 0);             //0 means unreliable
                    enet_peer_send(c, 0, packet);
                    enet_host_flush(lobbyServer);
                }

                // std::string updateMessageItems = "I";
                // for (Player p : gd.players_){
                //     for(Item * i : p.items_){
                //         updateMessage += " " + std::to_string(p.index) + " ";
                //         updateMessage += std::to_string((i->pos.x)) + " " + std::to_string((i->pos.y)) + " ";
                //         updateMessage += std::to_string((i->vel.x)) + " " + std::to_string((i->vel.y)) + " ";
                        
                //     }
                    
                // }

                // for(ENetPeer * c : clients){
                //     //Broadcast player states
                    
                //     ENetPacket * packet = enet_packet_create(updateMessage.c_str(), updateMessage.size()+1, 0);             //0 means unreliable
                //     enet_peer_send(c, 0, packet);
                //     enet_host_flush(lobbyServer);
                // }
                lastBroadcast = nowTime;

            }
            prevTime = nowTime;
            if(gd.round_is_over){
                printf("round over\n");
                inGame = false;
                if(gd.isGrandPrix){
                   
                    if(currState->currStateVal != DESERT){		//TODO ELLIE MAKE THIS THE LAST STAGE IN THE GRAND PRIX
                        for(int i = 0; i < gd.numPlayers; i++){
                            state.keys[i][4] = false;
                            state.keys[i][7] = false;
                        }
                        currState->nextStateVal = RESULTS;
                        currState = changeState(currState,gd);
                        //currState->init(state, gd);
                        inGame = false;
                        inResults = true;
                        
                        if(currState->prevStateVal==GRASSLANDS) {
                            currState->nextStateVal = SPACESHIP;
                        } else if(currState->prevStateVal==SPACESHIP) {
                            currState->nextStateVal = SNOW;
                        } else if(currState->prevStateVal == SNOW){
                            currState->nextStateVal = DESERT;
                        }
                        
                    } else {
                        inGame = false;
                        inLobby = false;
                    }
                } else {
                    //End Results
                    printf("Not grand prix not inLobby\n");
                    inGame = false;
                    inLobby = false;
                }
                gd.round_is_over = false;
            }
        }
        
        if(gd.isGrandPrix){
            
            while(inResults){                 //Means that you are in interm. results screen
                
                while(enet_host_service(lobbyServer, &event, 0) > 0 && inResults){
                    //std::string message;
                    switch (event.type) {
                        case ENET_EVENT_TYPE_CONNECT:
                        {   //Do the things you do when you connect
                            break;
                        }
                        case ENET_EVENT_TYPE_RECEIVE:
                        {   //Add conditionals to do what server says
                            std::string message((char *) event.packet->data, event.packet->dataLength);
                            enet_packet_destroy(event.packet);
                            //printf("T: %s\n", message.c_str());
                                if(message.find("READY ") != std::string::npos){            //Player indicated Ready - Format "READY player_id"
                                
                                if(!prixReadyPlayers[std::stoi(message.substr(6,1))]){
                                    prixReadyPlayers[std::stoi(message.substr(6,1))] = 1;
                                    
                                    
                                    int totalReady = 0;
                                    for(int ready: prixReadyPlayers){
                                        
                                        totalReady += ready;
                                    }
                                    //printf("%d %d\n", totalReady, gd.numPlayers);
                                    printf("Total ready: %d NumPlayers: %d\n", totalReady, gd.numPlayers);
                                    if(totalReady == gd.numPlayers){   //If all players agree to ready, they enter the game
                                        //Send message to go to level
                                        //Update Game Data
                                        ready = true;
                                        inGame = true;
                                        inResults = false;
                                        for(int i = 0; i < 8; i++){
                                            prixReadyPlayers[i] = 0;
                                        }
                                        
                                        //0 is grasslands, 1 is spaceship
                                        if(currState->prevStateVal == GRASSLANDS){
                                            currState->nextStateVal = SPACESHIP;
                                        } else if (currState->prevStateVal == SPACESHIP){
                                            currState->nextStateVal = SNOW;
                                        } else if(currState->prevStateVal == SNOW){
                                            //TODO ADD OTHER STAGES ELLIE
                                            currState->nextStateVal = DESERT;
                                        }

                                        std::string replyMessage = "CHANGE_STATE " + std::to_string(currState->currStateVal);
                                        ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                                        currState = changeState(currState, gd);
                                        currState->init(state, gd);
                                        for(ENetPeer * client : clients){
                                            enet_peer_send(client, 0, packet);
                                            enet_host_flush(lobbyServer);
                                        }
                                        

                                        
                                        
                                        
                                        

                                        break;
                                        
                    
                                    }
                                }
                            } 

                            break;
                        }
                        case ENET_EVENT_TYPE_DISCONNECT:
                        {  //Do the things to do when disconnecting
                            printf("player left game\n");
                            inGame = false;
                            break;
                        }
                        
                    }
                }
            }
        }
    }
    printf("Lobby is over\n");
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
    

    //Create your ENET Server
    ENetAddress address = {};
    //Address of the Raspberry Pi
    enet_address_set_host(&address, "100.89.84.24");
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
        
        
        while(enet_host_service(matchmakerServer, &event, 0) > 0){
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    {
                        std::cout << "Client Connected to Matchmaker Server!\n";
                        break;
                    }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    std::string message((char *) event.packet->data, event.packet->dataLength);
                    //printf("%s\n", message.c_str());
                    enet_packet_destroy(event.packet);
                    printf("Message: %s\n", message.c_str());
                    //Check what the message was
                    if(message.find("HOST ") != std::string::npos){
                        int lobbyPort = getAvailablePort();
                        Lobby newLobby = parseHostMessage(message, currLobbyID++, lobbyPort);
                        lobbies.push_back(newLobby);
                        //fflush(stdout);
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
                            //printf("%s", replyMessage.c_str());
                            ENetPacket * packet = enet_packet_create(replyMessage.c_str(), replyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet);
                            enet_host_flush(matchmakerServer);
                        } else {
                            printf("FAILED FORK\n");
                        }
                        
                    } else if(message.find("LOBBY_QUERY") !=  std::string::npos){
                        std::string lobbiesReturnMessage = "LOBBIES ";
                        for (Lobby l : lobbies){
                            lobbiesReturnMessage += l.to_string();
                        }
                        //printf("%s\n", lobbiesReturnMessage);
                            ENetPacket* packet = enet_packet_create(lobbiesReturnMessage.c_str(), lobbiesReturnMessage.size(), ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet);
                            enet_host_flush(matchmakerServer);
                        
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
                        //lobbyPort = 40000;
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


    }

    printf("delete gameState\n");
    delete currState;
    
    enet_deinitialize();
    return 0;
}