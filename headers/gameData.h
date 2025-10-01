#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "../ext/glm/glm.hpp"
#include "gameobject.h"
#include "initState.h"
#include "player.h"

struct GameData {
    std::vector<GameObject> bgTiles;
    std::vector<GameObject> fgTiles;
    std::vector<GameObject> mapTiles;
    std::vector<GameObject> characters;
    std::vector<GameObject> lasers;
    
    
    Player player;
    std::vector<BackgroundObject> bgTiles_;
    std::vector<Level> mapTiles_;
    std::vector<Laser> lasers_;
    std::vector<Portal> portals_;
    glm::vec2 ExitPortal, 
              EntrancePortal, 
              mouseCoords, 
              clickCoords;
    
    std::vector<GameObject> bullets;
    int playerIndex = -1;
    SDL_FRect mapViewport;
    bool debugMode = false;
    
    GameData(const SDLState &state) {
        playerIndex = -1; // will change when map is loaded
        mapViewport = SDL_FRect {
            .x = 0,
            .y = 0,
            .w = static_cast<float>(state.logW),
            .h = static_cast<float>(state.logH)
        };
        debugMode = false;
    }
};