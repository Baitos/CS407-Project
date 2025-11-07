#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "../ext/glm/glm.hpp"
#include "initState.h"
#include "player.h"
#include "menu.h"
#include "createCheckpoints.h"

struct Lobby{
    bool isGrandPrix;
    int numLaps;
    int id;
    int port;
    int playerCount;
    size_t passwordHash; // hash = 0 means no password
    std::string hostName;
    // Lobby info in comma delimited string format
    std::string to_string() {
        //HOSTNAME,PASSHASH,isGrandPrix[0/1],numLaps
        std::string lobbyStr;
        std::string grandPrix = isGrandPrix ? "1" : "0";
        lobbyStr = hostName + "," + std::to_string(passwordHash) + "," +
            grandPrix + "," + std::to_string(numLaps);
        return lobbyStr;
    }
};

struct MenuData {
    bool isPrivate = false;
    bool stringEditing = false;
    bool showCursor = true;
    int startLobbyIndex = 0; // index of top lobby in join screen 
    int lobbyPicked;
    int lastCursorToggle = 0;
    int currMapVote = 0;
    std::string tempStr = " ";
    std::string password;
    std::string displayName;
    Object border;
    Object * lobbySelectBorder = nullptr;
    Object * verticalDial = nullptr;
    Object * updatedDial = nullptr;
    TTF_Font* font; //font for drawing on screen

    std::vector<Object> settingsDials_;
    std::vector<Object> gameplaySettingsBrackets1_;
    std::vector<Object> gameplaySettingsBrackets2_;
    std::vector<Object> gameplaySettingsNumLaps_;
    std::vector<Object> gameplaySettingsModeHighlights_;

    std::vector<AnimatedObject> previews_;
    std::vector<AnimatedObject> map_previews_;
    std::vector<AnimatedObject> map_previews_text_;
    std::vector<AnimatedObject> arrows_;
    std::vector<charIconObject> charIcons_;

    std::vector<Lobby> publicLobbies_;
    std::vector<Lobby> privateLobbies_;
    MenuData(const SDLState &state) {
    }
};