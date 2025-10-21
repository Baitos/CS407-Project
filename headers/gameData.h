#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "../ext/glm/glm.hpp"
#include "initState.h"
#include "player.h"
#include "menu.h"
#include "createCheckpoints.h"
struct GameData {
    std::vector<Player> players_;
    int numPlayers; 

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

    std::vector<charIconObject> charIcons_;
    std::vector<AnimatedObject> previews_;
    glm::vec2 ExitPortal, 
              EntrancePortal, 
              mouseCoords, 
              clickCoords;
    ItemStorage itemStorage_;
    Object settingsBorder;
    std::vector<Object> settingsDials_;
    //Note that volume ratio is dial.pos.x / (290-84)
    Object * updatedDial;
    int playerIndex = -1;
    SDL_FRect mapViewport;
    bool debugMode = false;
    
    GameData(const SDLState &state) {
        playerIndex = -1; // will change when map is loaded
        numPlayers = 8;
        mapViewport = SDL_FRect {
            .x = 0,
            .y = 0,
            .w = static_cast<float>(state.logW),
            .h = static_cast<float>(state.logH)
        };
        debugMode = false;
    }
};