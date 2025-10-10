#pragma once
#include "gameData.h"

enum typeAction {
    ACTION_SPRINT,
    ACTION_GRAPPLE,
    ACTION_ABILITY,
    ACTION_JUMP,
    ACTION_USEITEM,
    ACTION_PAUSE,
    ACTION_FASTFALL
};
struct controlStruct {
    int key;
    bool isMouseButton;
};
class Controls {
    public:
    std::vector<controlStruct> keyboardControls;
    controlStruct nextKey;
    std::string keyText;
    typeAction currActionRebind;
    Controls() {
        keyboardControls = {

            {SDL_SCANCODE_LSHIFT, false},
            {SDL_BUTTON_RIGHT, true},
            {SDL_BUTTON_LEFT, true},
            {SDL_SCANCODE_SPACE, false},
            {SDL_SCANCODE_Q, false},
            {SDL_SCANCODE_ESCAPE, false},
            {SDL_SCANCODE_S, false}
        };
    }

    void setControls(typeAction action, controlStruct newKey) {
        this->keyboardControls[action] = newKey;
    }
    int getActionKey(typeAction action) {
        return this->keyboardControls[action].key;
    }
};

