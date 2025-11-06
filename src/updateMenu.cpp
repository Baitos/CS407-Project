#include "../headers/updateMenu.h"
#include "../headers/collision.h"
#include "../headers/gameData.h"
#include "../headers/initState.h"
#include "../headers/helper.h"
#include "../headers/globals.h"
#include "../headers/resources.h"
#include "../headers/state.h"
#include "../headers/playerState.h"
#include "../headers/enet.h"


#include "../headers/controls.h"
#include "../headers/sound.h"
using namespace std;

extern GameState * currState;
extern ENetPeer* serverPeer;
extern ENetHost * client;

//
//UPDATE FUNCTIONS
//

//Update for Character Select Screen
void titleUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    Uint64 now = SDL_GetTicks();

    if (now - gd.md.lastCursorToggle > 500) { // blink rate
        gd.md.showCursor = !gd.md.showCursor;
        gd.md.lastCursorToggle = now;
    }
}

void charSelectUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
        for (AnimatedObject &preview : gd.md.previews_) {
            preview.update(state, gd, res, deltaTime);
        }
}

//Update for Settings Screen
void settingsUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    // Update controls
    Controls * controls  = gd.controls;
    SettingsState * curSettings = ((SettingsState *)currState);
    controlStruct construct;
    std::string keyName;
    for (int i = 0; i < 7; i++) {
        construct = controls->keyboardControls[i];
        if (construct.isMouseButton) {
            if (construct.key == SDL_BUTTON_LEFT) {
                keyName = std::string("LMB");
            }
            else if (construct.key == SDL_BUTTON_RIGHT) {
                keyName = std::string("RMB");
            }
            else {
                keyName = std::string("Mouse3"); // middle click
            }
        }
        else {
            // gets the name of the key for the current action
            keyName = std::string(SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)construct.key, SDL_KMOD_NONE, false)));
        }
        curSettings->controlStrings[i] = keyName;
    }
    // Update audio
    //Note that volume ratio is dial.pos.x / (290-84)
    masterVolume = gd.md.settingsDials_[0].pos.x / (290 - 84) -40.8;
    if (masterVolume > 1.0f) {
        masterVolume = 1.0f;
    }
    else if (masterVolume < 0.0f) {
        masterVolume = 0.0f;
    }
    musicVolume = gd.md.settingsDials_[1].pos.x / (290 - 84) - 40.8;
        if (musicVolume > 1.0f) {
        musicVolume = 1.0f;
    }
    else if (masterVolume < 0.0f) {
        musicVolume = 0.0f;
    }
    sfxVolume = gd.md.settingsDials_[2].pos.x / (290 - 84) - 40.8;
        if (sfxVolume > 1.0f) {
        sfxVolume = 1.0f;
    }
    else if (sfxVolume < 0.0f) {
        sfxVolume = 0.0f;
    }
    
    //printf("Master = %.1f, music = %.1f, sfx = %f\n", masterVolume, musicVolume, sfxVolume);
}

void gameplaySettingsUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    
}
void hostLobbyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    Uint64 now = SDL_GetTicks();
    if (now - gd.md.lastCursorToggle > 500) { // blink rate
        gd.md.showCursor = !gd.md.showCursor;
        gd.md.lastCursorToggle = now;
    }
}
void joinLobbyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    // TODO Receive lobbies from server
    // std::string lobbyQuery = "LOBBY_QUERY";
    //         ENetPacket * packet = enet_packet_create(lobbyQuery.c_str(), lobbyQuery.size()+1, ENET_PACKET_FLAG_RELIABLE);
    //         enet_peer_send(serverPeer, 0, packet);
    //         enet_host_flush(client);
    Lobby lobby;
    gd.md.publicLobbies_.clear();
    gd.md.privateLobbies_.clear();
    for (int i = 0; i < 40; i++) {
        lobby.id = i;
        lobby.port = 40000 + i;
        lobby.hostName = "lobby lobby" + to_string(i);
        lobby.playerCount = i + 1;
        if (i % 2 == 0) {
            lobby.passwordHash = 0;
             gd.md.publicLobbies_.push_back(lobby);
        } 
        else {
            lobby.passwordHash = hash<string>{}(to_string(i));
            gd.md.privateLobbies_.push_back(lobby);
        }
    }
    if (gd.md.verticalDial == nullptr) {
        return;
    }
    // set gd.md.startLobbyIndex
    std::vector<Lobby> lobbies = gd.md.isPrivate ? gd.md.privateLobbies_ : gd.md.publicLobbies_;
    float dialPercentage = (gd.md.verticalDial->pos.y - 56.0) / 130.0f; // Top yPpos = 56, bottom yPos = 186, diff = 130
    if (lobbies.size() <= 7) { // max 7 lobbies in join screen
        gd.md.startLobbyIndex = 0;
        return;
    }
    else {
        gd.md.startLobbyIndex = floor(lobbies.size() * dialPercentage);
    }
    if (gd.md.startLobbyIndex > lobbies.size() - 7) {
        gd.md.startLobbyIndex = lobbies.size() - 7;
    }
    //         //For Testing
    //         std::string joinMessage = "JOIN 1";
    //         packet = enet_packet_create(joinMessage.c_str(), joinMessage.size()+1, ENET_PACKET_FLAG_RELIABLE);
    //         enet_peer_send(serverPeer, 0, packet);
    //         enet_host_flush(client);
    //         currState->nextStateVal = CHAR_SELECT;
    //         currState = changeState(currState, gd);
    //         currState->init(state, gd, res);

}

//
//INPUT FUNCTIONS
//

//Input function for Title Screen
void titleInput(SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_Event event { 0 };
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
            {
                running = false;
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: 
            {
                state.width = event.window.data1;
                state.height = event.window.data2;
                //printf("Width = %d, Height = %d", state.width, state.height);
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                if (!event.key.repeat && gd.md.stringEditing) {
                    SDL_Scancode sc = event.key.scancode;
                    if (sc == SDL_SCANCODE_BACKSPACE) {
                        if(!gd.md.tempStr.empty()) {
                            gd.md.tempStr.pop_back();
                        }
                    } else if (sc == SDL_SCANCODE_RETURN) {
                        gd.md.displayName = gd.md.tempStr;
                        gd.md.stringEditing = false;
                        username = gd.md.tempStr;
                    } else {
                        bool shift = (SDL_GetModState() & SDL_KMOD_SHIFT);
                        char c = 0;

                        if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z) {
                            c = (shift) ? ('A' + (sc - SDL_SCANCODE_A))
                                        : ('a' + (sc - SDL_SCANCODE_A));
                        }
                        else if (sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_9) {
                            c = '1' + (sc - SDL_SCANCODE_1);
                        }
                        else if (sc == SDL_SCANCODE_0) {
                            c = '0';
                        }
                        else if (sc == SDL_SCANCODE_SPACE) {
                            c = ' ';
                        }

                        if (c && gd.md.tempStr.length() < 12) { // limit length
                            gd.md.tempStr += c;
                        }
                    }
                }
                
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (gd.mouseCoords.x >= 363 && gd.mouseCoords.x <= 595 &&
                    gd.mouseCoords.y >= 165 && gd.mouseCoords.y <= 200 &&
                    gd.md.stringEditing == false) {
                    gd.md.stringEditing = true;
                    gd.md.tempStr = username;
                } else {
                    if (gd.md.stringEditing) {
                        gd.md.displayName = gd.md.tempStr;
                        gd.md.stringEditing = false; // Clicked away
                        username = gd.md.tempStr;
                    }
                }
                handleTitleClick(state,gd,res,deltaTime);
                break;
                
            }  case SDL_EVENT_MOUSE_BUTTON_UP:

            {
                //handleTitleClick(state,gd, res, deltaTime);
                //gd.md.updatedDial = NULL;
            }
        }
    }
}
//Input Function for Character Select Screen
void charSelectInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_Event event { 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: 
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    //printf("Width = %d, Height = %d", state.width, state.height);
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    handleCharSelectKeyInput(state, gd, res, event.key, true, deltaTime);
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    handleCharSelectKeyInput(state, gd, res, event.key, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    handleCharSelectClick(state, gd, res, deltaTime);
                    break;
                    
                } 
            }
        }
}
//Input function for Settings Screen
void settingsInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_Event event { 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: 
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    //printf("Width = %d, Height = %d", state.width, state.height);
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    if(waitingForKey) {
                        hasNewKey = true;
                        gd.controls->setControls(gd.controls->currActionRebind, {event.key.scancode, false});
                        waitingForKey = false;
                    }
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    if (waitingForKey) {
                        hasNewKey = true;
                        gd.controls->setControls(gd.controls->currActionRebind, {event.button.button, true});
                        waitingForKey = false;
                    }
                    else {
                        handleSettingsClick(state,gd,res,deltaTime);
                    }
                    break;
                    
                }  case SDL_EVENT_MOUSE_BUTTON_UP:

                {
                    gd.md.updatedDial = NULL;
                }
            }
        }
}
//Input function for GAmeplaySettings Screen
void gameplaySettingsInput(SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_Event event { 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: 
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    //printf("Width = %d, Height = %d", state.width, state.height);
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    handleGameplaySettingsClick(state,gd,res,deltaTime);
                    break;
                    
                }  case SDL_EVENT_MOUSE_BUTTON_UP:

                {
                    //gd.md.updatedDial = NULL;
                }
            }
        }
}
//Input function for create lobby screen
void hostLobbyInput(SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_Event event { 0 };
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
            {
                running = false;
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: 
            {
                state.width = event.window.data1;
                state.height = event.window.data2;
                //printf("Width = %d, Height = %d", state.width, state.height);
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                if (!event.key.repeat && gd.md.stringEditing) {
                    SDL_Scancode sc = event.key.scancode;
                    if (sc == SDL_SCANCODE_BACKSPACE) {
                        if(!gd.md.tempStr.empty()) {
                            gd.md.tempStr.pop_back();
                        }
                    } else if (sc == SDL_SCANCODE_RETURN) {
                        gd.md.password = gd.md.tempStr;
                        gd.md.stringEditing = false;
                    } else {
                        bool shift = (SDL_GetModState() & SDL_KMOD_SHIFT);
                        char c = 0;

                        if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z) {
                            c = (shift) ? ('A' + (sc - SDL_SCANCODE_A))
                                        : ('a' + (sc - SDL_SCANCODE_A));
                        }
                        else if (sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_9) {
                            c = '1' + (sc - SDL_SCANCODE_1);
                        }
                        else if (sc == SDL_SCANCODE_0) {
                            c = '0';
                        }
                        else if (sc == SDL_SCANCODE_SPACE) {
                            c = ' ';
                        }

                        if (c && gd.md.tempStr.length() < 12) { // limit length
                            gd.md.tempStr += c;
                        }
                    }
                }
                
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                // Text box
                if (gd.mouseCoords.x >= 210 && gd.mouseCoords.x <= 605 &&
                    gd.mouseCoords.y >= 256 && gd.mouseCoords.y <= 290 &&
                    gd.md.stringEditing == false && gd.md.isPrivate) {
                    gd.md.stringEditing = true;
                    gd.md.tempStr = gd.md.password;
                } else if (gd.md.stringEditing) {
                    gd.md.password = gd.md.tempStr;
                    gd.md.stringEditing = false; // Clicked away
                }
                handleHostLobbyClick(state,gd,res,deltaTime);
                break;   
            }  case SDL_EVENT_MOUSE_BUTTON_UP:

            {
                //handleTitleClick(state,gd, res, deltaTime);
                //gd.md.updatedDial = NULL;
            }
        }
    }
}
//Input function for join lobby screen
void joinLobbyInput(SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_Event event { 0 };
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
            {
                running = false;
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED: 
            {
                state.width = event.window.data1;
                state.height = event.window.data2;
                //printf("Width = %d, Height = %d", state.width, state.height);
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                if (!event.key.repeat && gd.md.stringEditing) {
                    SDL_Scancode sc = event.key.scancode;
                    if (sc == SDL_SCANCODE_BACKSPACE) {
                        if(!gd.md.tempStr.empty()) {
                            gd.md.tempStr.pop_back();
                        }
                    } else if (sc == SDL_SCANCODE_RETURN) {
                        gd.md.password = gd.md.tempStr;
                        gd.md.stringEditing = false;
                    } else {
                        bool shift = (SDL_GetModState() & SDL_KMOD_SHIFT);
                        char c = 0;

                        if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z) {
                            c = (shift) ? ('A' + (sc - SDL_SCANCODE_A))
                                        : ('a' + (sc - SDL_SCANCODE_A));
                        }
                        else if (sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_9) {
                            c = '1' + (sc - SDL_SCANCODE_1);
                        }
                        else if (sc == SDL_SCANCODE_0) {
                            c = '0';
                        }
                        else if (sc == SDL_SCANCODE_SPACE) {
                            c = ' ';
                        }

                        if (c && gd.md.tempStr.length() < 12) { // limit length
                            gd.md.tempStr += c;
                        }
                    }
                }
                
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                // Text box
                if (gd.mouseCoords.x >= 180 && gd.mouseCoords.x <= 579 &&
                    gd.mouseCoords.y >= 11 && gd.mouseCoords.y <= 48 &&
                    !gd.md.stringEditing && gd.md.isPrivate) {
                    gd.md.stringEditing = true;
                    gd.md.tempStr = gd.md.password;
                } else if (gd.md.stringEditing) {
                    gd.md.password = gd.md.tempStr;
                    gd.md.stringEditing = false; // Clicked away
                }
                handleJoinLobbyClick(state,gd,res,deltaTime);
                break;   
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                gd.md.updatedDial = NULL;
            }
        }
    }
}

//
//INPUT HANDLERS
//

//Mouse Cursor for Title/Settings/Char Select/Etc.
void handleMousePointerTitle(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    gd.mouseCoords.x = gd.mouseCoords.x * xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };

    if(gd.md.stringEditing){
        gd.md.displayName = gd.md.tempStr;
        if (gd.md.stringEditing && gd.md.showCursor) {
            gd.md.displayName += '|';
        }
    } else if((gd.mouseCoords.x >= 363 && gd.mouseCoords.x <= 595) && (gd.mouseCoords.y >= 165 && gd.mouseCoords.y <= 200)) { 
        SDL_RenderTexture(state.renderer, res.texTextCursor, nullptr, &dst);
    } else {
        SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst);
    }

    if ((gd.mouseCoords.x >= 40 && gd.mouseCoords.x <= 219) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436)) { //Host
        gd.md.border.pos = glm::vec2(38,366);
        gd.md.border.texture = res.texBigBorder;
    } else if((gd.mouseCoords.x >= 315 && gd.mouseCoords.x <= 485) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436)){        //Join
        gd.md.border.pos = glm::vec2(312,366);
        gd.md.border.texture = res.texBigBorder;
    }else if((gd.mouseCoords.x >= 589 && gd.mouseCoords.x <= 767) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436)){         //Settings
        gd.md.border.pos = glm::vec2(586,366);
        gd.md.border.texture = res.texBigBorder;
    } else {
        gd.md.border.pos = glm::vec2(500,500);
    }
}
void handleMousePointerHostLobby(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    gd.mouseCoords.x = gd.mouseCoords.x * xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };

    if(gd.md.stringEditing){
        gd.md.password = gd.md.tempStr;
        if (gd.md.stringEditing && gd.md.showCursor) {
            gd.md.password += '|';
    }
    } else if((gd.mouseCoords.x >= 210 && gd.mouseCoords.x <= 605) && (gd.mouseCoords.y >= 256 && gd.mouseCoords.y <= 290)) { // Text cursor
        SDL_RenderTexture(state.renderer, res.texTextCursor, nullptr, &dst);
    } else {
        SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst);
    }
    if ((gd.mouseCoords.x >= 40 && gd.mouseCoords.x <= 219) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing) { //Back
        gd.md.border.pos = glm::vec2(38,366);
        gd.md.border.texture = res.texBigBorder;
    }
    else if ((gd.mouseCoords.x >= 315 && gd.mouseCoords.x <= 495) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){       //Settings
        gd.md.border.pos = glm::vec2(313, 366);
        gd.md.border.texture = res.texBigBorder;
    }
    else if((gd.mouseCoords.x >= 589 && gd.mouseCoords.x <= 767) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){ //Host
        gd.md.border.pos = glm::vec2(586,366);
        gd.md.border.texture = res.texBigBorder;
    } else {
        gd.md.border.pos = glm::vec2(500,500);
    }
}
void handleMousePointerJoinLobby(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    gd.mouseCoords.x = gd.mouseCoords.x * xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };

    if(gd.md.stringEditing){
        gd.md.password = gd.md.tempStr;
        if (gd.md.stringEditing && gd.md.showCursor) {
            gd.md.password += '|';
    }
    
    } else if((gd.mouseCoords.x >= 180 && gd.mouseCoords.x <= 579 && gd.mouseCoords.y >= 11 && gd.mouseCoords.y <= 48)) { // Text cursor
        SDL_RenderTexture(state.renderer, res.texTextCursor, nullptr, &dst);
    } else {
        SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst);
    }

    if(gd.md.updatedDial != NULL){
        gd.md.updatedDial->pos.y = gd.mouseCoords.y;
        if (gd.md.updatedDial->pos.y  > 186.0f){ 
            gd.md.updatedDial->pos.y  = 186.0f;
        } else if(gd.md.updatedDial->pos.y < 56.0f) {
            gd.md.updatedDial->pos.y = 56.0f;
        }
        //gd.md.updatedDial->draw(state,gd,static_cast<float>( gd.md.updatedDial->texture->w), static_cast<float>( gd.md.updatedDial->texture->h));
    }
    if ((gd.mouseCoords.x >= 40 && gd.mouseCoords.x <= 219) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing) { //Public
        gd.md.border.pos = glm::vec2(38,366);
        gd.md.border.texture = res.texBigBorder;
    }
    else if ((gd.mouseCoords.x >= 315 && gd.mouseCoords.x <= 495) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){ //Private
        gd.md.border.pos = glm::vec2(313, 366);
        gd.md.border.texture = res.texBigBorder;
    }
    else if((gd.mouseCoords.x >= 589 && gd.mouseCoords.x <= 767) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){ //Join
        gd.md.border.pos = glm::vec2(586,366);
        gd.md.border.texture = res.texBigBorder;
    } else {
        gd.md.border.pos = glm::vec2(500,500);
    }
}
void handleMousePointerCharSelect(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    gd.mouseCoords.x = gd.mouseCoords.x * state.xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * state.yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };
    if((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big Border on Exit
        //printf("1\n");
        gd.md.border.pos = glm::vec2(38,366);
        gd.md.border.texture = res.texBigBorder;
    } else if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big border on Save
        //printf("2\n");
        gd.md.border.pos = glm::vec2(586,366); 
        gd.md.border.texture = res.texBigBorder;
    } else {
        //printf("3\n");
        gd.md.border.pos = glm::vec2(500,500);
    }

    //hover for arrows
    if(!gd.isGrandPrix) {
        if((gd.mouseCoords.x >= 107 && gd.mouseCoords.x <= 125) && (gd.mouseCoords.y >= 286 && gd.mouseCoords.y <= 310)){
            gd.md.arrows_[0].visible = true;
        } else if ((gd.mouseCoords.x >= 274 && gd.mouseCoords.x <= 292) && (gd.mouseCoords.y >= 286 && gd.mouseCoords.y <= 310)) {
            gd.md.arrows_[1].visible = true;
        } else {
            gd.md.arrows_[0].visible = false;
            gd.md.arrows_[1].visible = false;
        }
    }

    SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
    
}

void handleMousePointerSettings(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    gd.mouseCoords.x = gd.mouseCoords.x * state.xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * state.yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };
    if(gd.md.updatedDial != NULL){
        gd.md.updatedDial->pos.x = gd.mouseCoords.x;
        if (gd.md.updatedDial->pos.x  > 290.f){ 
            gd.md.updatedDial->pos.x  = 290.f;
        } else if(gd.md.updatedDial->pos.x < 84.f) {
            gd.md.updatedDial->pos.x  = 84.f;
        }
        gd.md.updatedDial->draw(state,gd,static_cast<float>( gd.md.updatedDial->texture->w) * 2, static_cast<float>( gd.md.updatedDial->texture->h) * 2);
    } else {
        if((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big Border on Exit
            //printf("1\n");
            gd.md.border.pos = glm::vec2(38,366);
            gd.md.border.texture = res.texBigBorder;
        } else if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big border on Save
            //printf("2\n");
            gd.md.border.pos = glm::vec2(585,366);
            gd.md.border.texture = res.texBigBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 106 && gd.mouseCoords.y <= 126)){ //Sprint
            gd.md.border.pos = glm::vec2(576,104);
            gd.md.border.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 130 && gd.mouseCoords.y <= 150)){ //Grapple
            gd.md.border.pos = glm::vec2(576,130);
            gd.md.border.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 156 && gd.mouseCoords.y <= 174)){ //Ability
            gd.md.border.pos = glm::vec2(576,156);
            gd.md.border.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 182 && gd.mouseCoords.y <= 202)){ //Jump
            gd.md.border.pos = glm::vec2(576,182);
            gd.md.border.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 208 && gd.mouseCoords.y <= 228)){ //Use Item
            gd.md.border.pos = glm::vec2(576,208);
            gd.md.border.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 234 && gd.mouseCoords.y <= 254)){ //Pause
            gd.md.border.pos = glm::vec2(576,234);
            gd.md.border.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 260 && gd.mouseCoords.y <= 280)){ //Fast-Fall
            gd.md.border.pos = glm::vec2(576,260);
            gd.md.border.texture = res.texSmallBorder;
        } else {
            //printf("3\n");
            gd.md.border.pos = glm::vec2(500,500);
        }
    }
    SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
}
void handleMousePointerGameplaySettings (const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    gd.mouseCoords.x = gd.mouseCoords.x * xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };
    
    //hover over back button
    if((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big Border on Exit
        gd.md.border.pos = glm::vec2(38,366);
        gd.md.border.texture = res.texBigBorder;
    } else {
        gd.md.border.pos = glm::vec2(500,500);
    }

    //hover for arrows
    if((gd.mouseCoords.x >= 494 && gd.mouseCoords.x <= 512) && (gd.mouseCoords.y >= 254 && gd.mouseCoords.y <= 272)){
        gd.md.arrows_[0].visible = true;
    } else if ((gd.mouseCoords.x >= 548 && gd.mouseCoords.x <= 566) && (gd.mouseCoords.y >= 254 && gd.mouseCoords.y <= 272)) {
        gd.md.arrows_[1].visible = true;
    } else {
        gd.md.arrows_[0].visible = false;
        gd.md.arrows_[1].visible = false;
    }


    SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
}


//Key Input Handler for Char Select
void handleCharSelectKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime) {

    if (key.scancode == SDL_SCANCODE_F12 && key.down && !key.repeat) { // debug
            gd.debugMode = !gd.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F1) {
        running = false;
    }
    if(key.scancode == SDL_SCANCODE_F2){
        currState = changeState(currState, gd);
        currState->init(state,gd, res);
    }
}

//
// MOUSE CLICK HANDLERS
//
void handleCharSelectClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 156 && gd.mouseCoords.y <= 156+36)){
        //Send message saying swapped to sword
        std::string pendingMessage = "CLASS " + std::to_string(gd.playerIndex) + " " + std::to_string(SWORD);
        ENetPacket * packet = enet_packet_create(pendingMessage.c_str(), pendingMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(serverPeer, 0, packet);
        enet_host_flush(client);
        
        //Set Icons and Player to Sword
        gd.md.previews_[0].texture = res.texSword;
        gd.md.previews_[0].pos = glm::vec2(530,200);
        gd.md.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = SWORD;
        for (int i = 8; i<11; i++){
            gd.md.charIcons_[i].spriteFrame = gd.md.charIcons_[i].spriteFrame % 4;
            if (gd.md.charIcons_[i].spriteFrame == 3){
                gd.md.charIcons_[i].spriteFrame +=4;
                gd.md.charIcons_[gd.playerIndex].spriteFrame = gd.md.charIcons_[i].spriteFrame;
            }
        }
    } else if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 220 && gd.mouseCoords.y <= 220+36)){
        //Send message saying swapped to jetpack
        std::string pendingMessage = "CLASS " + std::to_string(gd.playerIndex) + " " + std::to_string(JETPACK);
        ENetPacket * packet = enet_packet_create(pendingMessage.c_str(), pendingMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(serverPeer, 0, packet);
        enet_host_flush(client);
        //Set Icons and Player to Jetpack
        gd.md.previews_[0].texture = res.texJetpack;
        gd.md.previews_[0].pos = glm::vec2(510,200);
        gd.md.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = JETPACK;
        for (int i = 8; i<11; i++){
            gd.md.charIcons_[i].spriteFrame = gd.md.charIcons_[i].spriteFrame % 4;
            if (gd.md.charIcons_[i].spriteFrame == 2){
                gd.md.charIcons_[i].spriteFrame +=4;
                gd.md.charIcons_[gd.playerIndex].spriteFrame = gd.md.charIcons_[i].spriteFrame;
            }
        }
        
    } else if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 284 && gd.mouseCoords.y <= 284+36)){
        //Send message saying swapped to sword
        std::string pendingMessage = "CLASS " + std::to_string(gd.playerIndex) + " " + std::to_string(SHOTGUN);
        ENetPacket * packet = enet_packet_create(pendingMessage.c_str(), pendingMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(serverPeer, 0, packet);
        enet_host_flush(client);
        //Set Icons and Player to Shotgun
        gd.md.previews_[0].texture = res.texShotgun;
        gd.md.previews_[0].pos = glm::vec2(520,200);
        gd.md.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = SHOTGUN;
        for (int i = 8; i<11; i++){
            gd.md.charIcons_[i].spriteFrame = gd.md.charIcons_[i].spriteFrame % 4;
            if (gd.md.charIcons_[i].spriteFrame == 1){
                gd.md.charIcons_[i].spriteFrame +=4;
                gd.md.charIcons_[gd.playerIndex].spriteFrame = gd.md.charIcons_[i].spriteFrame;
            }
        }
    } else if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) {
        //Ready up message to server
        
        //Change to value Danny made
        int selectedStage = gd.md.currMapVote;
        std::string pendingMessage = "READY " + std::to_string(gd.playerIndex) + " " + std::to_string(selectedStage);
        ENetPacket * packet = enet_packet_create(pendingMessage.c_str(), pendingMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(serverPeer, 0, packet);
        enet_host_flush(client);
        
        //Enter Stage
        //TO DO - ONLY DO WHEN PLAYERS AGREE TO READY UP
        // characterType character = ((CharSelectState*) currState)->character;
        // if(gd.md.map_previews_[0].curAnimation == res.MAP_SPACESHIP) {
        //     currState->nextStateVal = SPACESHIP;
        // } else if(gd.md.map_previews_[0].curAnimation == res.MAP_GRASSLAND)  {
        //     currState->nextStateVal = GRASSLANDS;
        // } else {
        //     printf("INVALID MAP\n");
        //     return;
        // }
        // currState = changeState(currState, gd);
        // ((LevelState*) currState)->character = character;
        // currState->init(state, gd, res);
    } else if ((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) { // Exit
        currState->nextStateVal = TITLE;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    } else if ((gd.mouseCoords.x >= 107 && gd.mouseCoords.x <= 125) && (gd.mouseCoords.y >= 286 && gd.mouseCoords.y <= 310)){
        if(!gd.isGrandPrix) {
            //move map sprite - one frame
            for (AnimatedObject &map_preview : gd.md.map_previews_) {
                int index = map_preview.curAnimation;
                index--;
                if(index<0) {
                    index = 4;
                }
                map_preview.texture = res.texMapPreviews[index];
                map_preview.curAnimation = index; 
                map_preview.animations[map_preview.curAnimation].reset();
            }
            //move map text sprite - one frame
            for (AnimatedObject &map_preview_text : gd.md.map_previews_text_) {
                int index = map_preview_text.curAnimation;
                index--;
                if(index<0) {
                    index = 4;
                }
                map_preview_text.texture = res.texMapTextPreviews[index];
                map_preview_text.curAnimation = index; 
                map_preview_text.animations[map_preview_text.curAnimation].reset();
            }
            gd.md.currMapVote = gd.md.currMapVote - 1;
            if(gd.md.currMapVote<0) {
                gd.md.currMapVote = 4;
            }
        }
    } else if ((gd.mouseCoords.x >= 274 && gd.mouseCoords.x <= 292) && (gd.mouseCoords.y >= 286 && gd.mouseCoords.y <= 310)) {
        if(!gd.isGrandPrix) {
            //move map sprite + one 
            for (AnimatedObject &map_preview : gd.md.map_previews_) {
                int index = map_preview.curAnimation;
                index++;
                if(index>4) {
                    index = 0;
                }
                map_preview.texture = res.texMapPreviews[index];
                map_preview.curAnimation = index; 
                map_preview.animations[map_preview.curAnimation].reset();
            }
            //move map text sprite + one frame
            for (AnimatedObject &map_preview_text : gd.md.map_previews_text_) {
                int index = map_preview_text.curAnimation;
                index++;
                if(index>4) {
                    index = 0;
                }
                map_preview_text.texture = res.texMapTextPreviews[index];
                map_preview_text.curAnimation = index; 
                map_preview_text.animations[map_preview_text.curAnimation].reset();
            }
            gd.md.currMapVote = gd.md.currMapVote + 1;
            if(gd.md.currMapVote>4) {
                gd.md.currMapVote = 0;
            }
        }
    }
}

//Handles Clicking for Settings
void handleSettingsClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    Controls * controls = gd.controls;
    if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) {
        printf("Save\n");
    } else if ((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) {
        currState->nextStateVal = TITLE;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 106 && gd.mouseCoords.y <= 126)){ //Sprint
        printf("Sprint\n");
        waitingForKey = true;
        controls->currActionRebind = typeAction::ACTION_SPRINT;
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 130 && gd.mouseCoords.y <= 150)){ //Grapple
        printf("Grapple\n");
        waitingForKey = true;
        controls->currActionRebind = typeAction::ACTION_GRAPPLE;
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 156 && gd.mouseCoords.y <= 174)){ //Ability
        printf("Ability\n");
        waitingForKey = true;
        controls->currActionRebind = typeAction::ACTION_ABILITY;
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 182 && gd.mouseCoords.y <= 202)){ //Jump
        printf("Jump\n");
        waitingForKey = true;
        controls->currActionRebind = typeAction::ACTION_JUMP;
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 208 && gd.mouseCoords.y <= 228)){ //Use Item
        printf("Use Item\n");
        waitingForKey = true;
        controls->currActionRebind = typeAction::ACTION_USEITEM;
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 234 && gd.mouseCoords.y <= 254)){ //Pause
        printf("Pause\n");
        waitingForKey = true;
        controls->currActionRebind = typeAction::ACTION_PAUSE;
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 260 && gd.mouseCoords.y <= 280)){ //Fast-Fall
        printf("Fast-fall\n");
        waitingForKey = true;
        controls->currActionRebind = typeAction::ACTION_FASTFALL;
    }   
    for(Object &o : gd.md.settingsDials_){
        
        if((gd.mouseCoords.x >= o.pos.x && gd.mouseCoords.x <= o.pos.x + (static_cast<float>(o.texture->w) * 2)) && (gd.mouseCoords.y >= o.pos.y && gd.mouseCoords.y <= o.pos.y + (static_cast<float>(o.texture->h) * 2))){
           gd.md.updatedDial = &o; 
        }
    }
}

void handleGameplaySettingsClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    //click back
    if ((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) {
        //set the num of laps
        for(Object &ob: gd.md.gameplaySettingsNumLaps_) {
            if(ob.texture == res.texGameplaySettings1) {
                gd.laps_per_race = 1;
            } else if(ob.texture == res.texGameplaySettings2) {
                gd.laps_per_race = 2;
            } else if(ob.texture == res.texGameplaySettings3) {
                gd.laps_per_race = 3;
            } else if(ob.texture == res.texGameplaySettings4) {
                gd.laps_per_race = 4;
            } else if(ob.texture == res.texGameplaySettings5) {
                gd.laps_per_race = 5;
            }
        }
        currState->nextStateVal = HOST;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }

    //click for arrows
    if((gd.mouseCoords.x >= 494 && gd.mouseCoords.x <= 512) && (gd.mouseCoords.y >= 254 && gd.mouseCoords.y <= 272)){
        //move left
        for(Object &ob: gd.md.gameplaySettingsNumLaps_) {
            if(ob.texture == res.texGameplaySettings1) {
                ob.texture = res.texGameplaySettings5;
            } else if(ob.texture == res.texGameplaySettings2) {
                ob.texture = res.texGameplaySettings1;
            } else if(ob.texture == res.texGameplaySettings3) {
                ob.texture = res.texGameplaySettings2;
            } else if(ob.texture == res.texGameplaySettings4) {
                ob.texture = res.texGameplaySettings3;
            } else if(ob.texture == res.texGameplaySettings5) {
                ob.texture = res.texGameplaySettings4;
            }
        }
    } else if ((gd.mouseCoords.x >= 548 && gd.mouseCoords.x <= 566) && (gd.mouseCoords.y >= 254 && gd.mouseCoords.y <= 272)) {
        //move right
        for(Object &ob: gd.md.gameplaySettingsNumLaps_) {
            if(ob.texture == res.texGameplaySettings1) {
                ob.texture = res.texGameplaySettings2;
            } else if(ob.texture == res.texGameplaySettings2) {
                ob.texture = res.texGameplaySettings3;
            } else if(ob.texture == res.texGameplaySettings3) {
                ob.texture = res.texGameplaySettings4;
            } else if(ob.texture == res.texGameplaySettings4) {
                ob.texture = res.texGameplaySettings5;
            } else if(ob.texture == res.texGameplaySettings5) {
                ob.texture = res.texGameplaySettings1;
            }
        }
    }

    //click for game mode
    if((gd.mouseCoords.x >= 175 && gd.mouseCoords.x <= 380) && (gd.mouseCoords.y >= 152 && gd.mouseCoords.y <= 185)) {
        gd.isGrandPrix = true;
    } else if((gd.mouseCoords.x >= 400 && gd.mouseCoords.x <= 605) && (gd.mouseCoords.y >= 152 && gd.mouseCoords.y <= 185)) {
        gd.isGrandPrix = false;
    }
}

void handleHostLobbyClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    if ((gd.mouseCoords.x >= 40 && gd.mouseCoords.x <= 219) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){       //Back
        gd.md.isPrivate = false;
        gd.md.password = "";
        gd.md.tempStr = "";
        currState->nextStateVal = TITLE;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
    else if ((gd.mouseCoords.x >= 225 && gd.mouseCoords.x <= 355) && (gd.mouseCoords.y >= 170 && gd.mouseCoords.y <= 210) && gd.md.isPrivate) { //Public 
        gd.md.isPrivate = false;
        gd.md.password = "";
        gd.md.tempStr = "";
        // Change Background
        SDL_FRect collider = {
        .x = 0,
        .y = 0,
        .w = (float)res.texHostLobbyPublic->w,
        .h = (float)res.texHostLobbyPublic->h
        };
        BackgroundObject bg(glm::vec2(0,0), collider, res.texHostLobbyPublic);
        gd.bgTiles_.pop_back();
        gd.bgTiles_.push_back(bg);
    }
    else if ((gd.mouseCoords.x >= 440 && gd.mouseCoords.x <= 590) && (gd.mouseCoords.y >= 170 && gd.mouseCoords.y <= 210) && !gd.md.isPrivate) { //Private 
        gd.md.isPrivate = true;
        // Change Background
        SDL_FRect collider = {
        .x = 0,
        .y = 0,
        .w = (float)res.texHostLobbyPrivate->w,
        .h = (float)res.texHostLobbyPrivate->h
        };
        BackgroundObject bg(glm::vec2(0,0), collider, res.texHostLobbyPrivate);
        gd.bgTiles_.pop_back();
        gd.bgTiles_.push_back(bg);
    }
    else if ((gd.mouseCoords.x >= 315 && gd.mouseCoords.x <= 495) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){       //Settings
        gd.md.isPrivate = false;
        gd.md.password = "";
        gd.md.tempStr = "";
        currState->nextStateVal = GAMEPLAY_SETTINGS;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
    else if((gd.mouseCoords.x >= 589 && gd.mouseCoords.x <= 767) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){   //Host
        // TODO ADD PASSWORD HASH TO MESSAGE
        size_t passwordHash = hash<string>{}(gd.md.password);
        //TELL SERVER TO MAKE LOBBY
        std::string createLobbyMessage = "HOST " + gd.md.displayName + " " + std::to_string(gd.isGrandPrix) + " " + std::to_string(gd.laps_per_race);
        ENetPacket * packet = enet_packet_create(createLobbyMessage.c_str(), createLobbyMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(serverPeer, 0, packet);
        enet_host_flush(client);

        currState->nextStateVal = CHAR_SELECT;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
}
void handleJoinLobbyClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
if ((gd.mouseCoords.x >= 40 && gd.mouseCoords.x <= 219) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){ //Back
        currState->nextStateVal = TITLE;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
    else if ((gd.mouseCoords.x >= 315 && gd.mouseCoords.x <= 495) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){ //Public or Private
        // Change Background
        SDL_FRect collider = {
        .x = 0,
        .y = 0,
        .w = (float)res.texJoinLobbyPublic->w,
        .h = (float)res.texJoinLobbyPublic->h
        };
        BackgroundObject bg;
        if (gd.md.isPrivate) {
            bg = BackgroundObject(glm::vec2(0,0), collider, res.texJoinLobbyPublic);
        }
        else {
            bg = BackgroundObject(glm::vec2(0,0), collider, res.texJoinLobbyPrivate);
        }
        gd.bgTiles_.pop_back();
        gd.bgTiles_.push_back(bg);
        // Switch to opposite mode, and reset password strings
        gd.md.isPrivate = !gd.md.isPrivate;
        gd.md.password = "";
        gd.md.tempStr = "";
        gd.md.lobbySelectBorder->visible = false;
    }
    else if((gd.mouseCoords.x >= 589 && gd.mouseCoords.x <= 767) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436) && !gd.md.stringEditing){   //Join
        // TODO check if lobby selected and password, then change state to char select after receiving response
        if (gd.md.isPrivate) {
            if (hash<string>{}(gd.md.password) == gd.md.privateLobbies_[gd.md.lobbyPicked].passwordHash) {
                printf("Password Good! Join %d\n", gd.md.privateLobbies_[gd.md.lobbyPicked].id);
            }
        }
        else 
            printf("join %d\n", gd.md.publicLobbies_[gd.md.lobbyPicked].id);
    }
    else if ((gd.mouseCoords.x >= gd.md.verticalDial->pos.x && gd.mouseCoords.x <= gd.md.verticalDial->pos.x + gd.md.verticalDial->texture->w * 2) && // Vertical Slider
        (gd.mouseCoords.y >= gd.md.verticalDial->pos.y && gd.mouseCoords.y <= gd.md.verticalDial->pos.y + gd.md.verticalDial->texture->h))
    {
        gd.md.updatedDial = gd.md.verticalDial;
        gd.md.lobbySelectBorder->visible = false;
        gd.md.lobbyPicked = 0;
    }
    else if ((gd.mouseCoords.x >= 50 && gd.mouseCoords.x <= 720) && (gd.mouseCoords.y >= 65 && gd.mouseCoords.y <= 330)) { // Within lobby select
        // 38 is vertical difference between lobby entries, lobbyPicked is between startLobbyIndex and startLobbyIndex + 6
        gd.md.lobbyPicked = gd.md.startLobbyIndex + (int)(gd.mouseCoords.y - 65) / 38;
        std::vector<Lobby> lobbies = gd.md.isPrivate ? gd.md.privateLobbies_ : gd.md.publicLobbies_;
        if (gd.md.lobbyPicked < lobbies.size()) {
            // Activate lobby selection border
            gd.md.lobbySelectBorder->pos.y = 56 + ((gd.md.lobbyPicked - gd.md.startLobbyIndex) * 38);
            gd.md.lobbySelectBorder->visible = true;
        }
    }
}

void handleTitleClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    if((gd.mouseCoords.x >= 363 && gd.mouseCoords.x <= 595) && (gd.mouseCoords.y >= 165 && gd.mouseCoords.y <= 200)) {
        //Text box   

    } else if ((gd.mouseCoords.x >= 40 && gd.mouseCoords.x <= 219) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436)){       //Host
        if(gd.md.tempStr!="") {
            currState->nextStateVal = HOST;
            currState = changeState(currState, gd);
            currState->init(state, gd, res);
        }
    } else if((gd.mouseCoords.x >= 315 && gd.mouseCoords.x <= 485) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436)){        //Join
        if(gd.md.tempStr!="") {    
            currState->nextStateVal = JOIN;
            currState = changeState(currState, gd);
            currState->init(state, gd, res);

            // std::string lobbyQuery = "LOBBY_QUERY";
            // ENetPacket * packet = enet_packet_create(lobbyQuery.c_str(), lobbyQuery.size()+1, ENET_PACKET_FLAG_RELIABLE);
            // enet_peer_send(serverPeer, 0, packet);
            // enet_host_flush(client);

            // //Rei TODO - For Testing keep here, later, when player chooses lobby id X, send "JOIN X" and change state
            // std::string joinMessage = "JOIN 1";
            // packet = enet_packet_create(joinMessage.c_str(), joinMessage.size()+1, ENET_PACKET_FLAG_RELIABLE);
            // enet_peer_send(serverPeer, 0, packet);
            // enet_host_flush(client);
            // currState->nextStateVal = CHAR_SELECT;
    
        }
    }else if((gd.mouseCoords.x >= 589 && gd.mouseCoords.x <= 767) && (gd.mouseCoords.y >= 368 && gd.mouseCoords.y <= 436)){         //Settings
        currState->nextStateVal = SETTINGS;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
}
