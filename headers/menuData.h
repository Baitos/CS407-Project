#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "../ext/glm/glm.hpp"
#include "initState.h"
#include "player.h"
#include "menu.h"
#include "createCheckpoints.h"
struct MenuData {
    std::vector<charIconObject> charIcons_;
    std::vector<AnimatedObject> previews_;
    std::vector<AnimatedObject> map_previews_;
    std::vector<AnimatedObject> map_previews_text_;
    std::vector<AnimatedObject> arrows_;

    Object settingsBorder;
    Object * updatedDial;
    std::vector<Object> settingsDials_;
    std::vector<Object> gameplaySettingsBrackets1_;
    std::vector<Object> gameplaySettingsBrackets2_;
    std::vector<Object> gameplaySettingsNumLaps_;
    std::vector<Object> gameplaySettingsModeHighlights_;

    
    bool usernameEditing = false;
    std::string tempUsername = " ";
    std::string displayName;
    int lastCursorToggle = 0;
    bool showCursor = true;
    //font for drawing on screen
    TTF_Font* font;

    MenuData(const SDLState &state) {

    }
};