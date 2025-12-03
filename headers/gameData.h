#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "../ext/glm/glm.hpp"
#include "initState.h"
#include "player.h"
#include "menu.h"
#include "controls.h"
#include "minimap.h"
#include "createCheckpoints.h"
#include "menuData.h"
#include "resultData.h"

struct GameData {
    std::vector<Player> players_;
    int numPlayers; 
    std::vector<Player> player_placement_;

    glm::vec2 mapSize; // gets x and y size of box used to create map;

    std::vector<BackgroundObject> bgTiles_;
    std::vector<Level> mapTiles_;
    std::vector<Laser> lasers_;
    std::vector<ItemBox> itemBoxes_;
    std::vector<Portal> portals_;
    std::vector<Sign> signs_;
    std::vector<Water> water_;
    std::vector<Lava> lava_;
    std::vector<Wire> wire_;
    
    std::vector<std::vector<Object*>> grid_; // all level tiles as pointers

    std::vector<Checkpoint> checkpoints_;

    MenuData md;

    ResultData rd;

    Minimap minimap; // the minimap for the current map
    glm::vec2 ExitPortal, 
              EntrancePortal, 
              mouseCoords, 
              clickCoords;
    ItemStorage itemStorage_;
    Controls *controls;
    // Object border;
    // std::vector<Object> gameplaySettingsBrackets1_;
    // std::vector<Object> gameplaySettingsBrackets2_;
    // std::vector<Object> gameplaySettingsNumLaps_;
    // std::vector<Object> gameplaySettingsModeHighlights_;

    // bool stringEditing = false;
    // std::string tempStr = " ";
    // std::string displayName;
    // int lastCursorToggle = 0;
    // bool showCursor = true;
    // //font for drawing on screen
    // TTF_Font* font;

    //Note that volume ratio is dial.pos.x / (290-84)
    int playerIndex = -1;
    SDL_FRect mapViewport;
    
    bool debugMode = false;
    int playerTypes[8];

    bool isGrandPrix = false;
    int laps_per_race = 1;
    int num_finished = 0;
    bool round_is_over = false;
    
    GameData(const SDLState &state): md(state), rd(state) {
        for(int i = 0; i < 8; i++){
            playerTypes[i] = -1;
        }
        playerIndex = -1; // will change when map is loaded
        numPlayers = 0;
        mapViewport = SDL_FRect {
            .x = 0,
            .y = 0,
            .w = static_cast<float>(state.logW),
            .h = static_cast<float>(state.logH)
        };
        debugMode = false;

        int gameplay_mode;
        controls = new Controls();
    }
};