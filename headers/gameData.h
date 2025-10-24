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
    std::vector<AnimatedObject> map_previews_;
    std::vector<AnimatedObject> map_previews_text_;
    std::vector<AnimatedObject> arrows_;

    glm::vec2 ExitPortal, 
              EntrancePortal, 
              mouseCoords, 
              clickCoords;
    ItemStorage itemStorage_;

    Object settingsBorder;
    std::vector<Object> settingsDials_;
    std::vector<Object> gameplaySettingsBrackets1_;
    std::vector<Object> gameplaySettingsBrackets2_;
    std::vector<Object> gameplaySettingsNumLaps_;
    std::vector<Object> gameplaySettingsModeHighlights_;

    //Note that volume ratio is dial.pos.x / (290-84)
    Object * updatedDial;
    int playerIndex = -1;
    SDL_FRect mapViewport;
    bool debugMode = false;

    bool isGrandPrix = false;
    int laps_per_race = 3;
    
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

        int gameplay_mode;
    }
};