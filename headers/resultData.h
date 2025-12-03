#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "../ext/glm/glm.hpp"
#include "initState.h"
#include "player.h"
#include "menu.h"
#include "createCheckpoints.h"

struct ResultData {
    enum ResultsPhase {RESULTS_NONE = 0, RESULTS_SHOW_ROUND, RESULTS_SHOW_CUMULATIVE };
    ResultsPhase resultsPhase = RESULTS_NONE;
    float resultsTimer = 0.0f;
    float resultsRoundDuration = 5.0f;
    struct ResultEntry{
        Player* player;
        int placement;
        int pointsEarned;
    };
    std::vector<ResultEntry> roundResults;
    std::vector<ResultEntry> cumulativeResults;
    //font for drawing on screen
    TTF_Font* font = nullptr;

    ResultData(const SDLState &state) {

    }
};