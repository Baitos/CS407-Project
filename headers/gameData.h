#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "../ext/glm/glm.hpp"
#include "initState.h"
#include "player.h"
#include "menu.h"
#include "minimap.h"
#include "createCheckpoints.h"
#include "menuData.h"

struct GameData {
    std::vector<Player> players_;
    int numPlayers; 

    glm::vec2 mapSize; // gets x and y size of box used to create map;

    std::vector<BackgroundObject> bgTiles_;
    std::vector<Level> mapTiles_;
    std::vector<Laser> lasers_;
    std::vector<ItemBox> itemBoxes_;
    std::vector<Item> items_;
    std::vector<Portal> portals_;
    std::vector<Sign> signs_;
    std::vector<Water> water_;
    std::vector<Lava> lava_;

    std::vector<Checkpoint> checkpoints_;

    MenuData md;

    Minimap minimap; // the minimap for the current map
    glm::vec2 ExitPortal, 
              EntrancePortal, 
              mouseCoords, 
              clickCoords;
    ItemStorage itemStorage_;

    //Note that volume ratio is dial.pos.x / (290-84)
    Object * updatedDial;
    int playerIndex = -1;
    SDL_FRect mapViewport;
    bool debugMode = false;

    bool isGrandPrix = false;
    int laps_per_race = 1;
    bool round_is_over = false;
    
    GameData(const SDLState &state): md(state) {
        playerIndex = -1; // will change when map is loaded
        numPlayers = 8;
        mapViewport = SDL_FRect {
            .x = 0,
            .y = 0,
            .w = static_cast<float>(state.logW),
            .h = static_cast<float>(state.logH)
        };
        debugMode = false;

        int gameplay_mode;
    }
};