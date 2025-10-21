#include "../headers/createCheckpoints.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/player.h"
#include "../headers/globals.h"
#include "../headers/state.h"
#include "../headers/playerState.h"



extern GameState* currState;

std::vector<Checkpoint> courseCheckpoints;

void createCheckpointsGrassland(const SDLState &state, GameData &gd, const Resources &res) {
    float tileSize = TILE_SIZE;
    //initialize the Rects for each checkpoint in form {{x*tileSize, state.logH - (MAP_ROWS - y)*tileSize, w, h*tileSize}, indx}
    courseCheckpoints = {
        { {31*tileSize, state.logH - (100 - 15)*tileSize, 16, 32*tileSize}, 0 },
        { {66*tileSize, state.logH - (100 - 14)*tileSize, 16, 28*tileSize}, 1 },
        { {130*tileSize, state.logH - (100 - 19)*tileSize, 16, 28*tileSize}, 2 },
        { {199*tileSize, state.logH - (100 - 16)*tileSize, 16, 31*tileSize}, 3 },
        { {276*tileSize, state.logH - (100 - 43)*tileSize, 16, 4*tileSize}, 4 },
        { {283*tileSize, state.logH - (100 - 18)*tileSize, 16, 4*tileSize}, 5 },
        { {382*tileSize, state.logH - (100 - 14)*tileSize, 16, 29*tileSize}, 6 },
        { {391*tileSize, state.logH - (100 - 15)*tileSize, 16, 4*tileSize}, 7 },
        { {398*tileSize, state.logH - (100 - 43)*tileSize, 16, 4*tileSize}, 8 },
        { {425*tileSize, state.logH - (100 - 16)*tileSize, 16, 14*tileSize}, 9 },
        { {489*tileSize, state.logH - (100 - 16)*tileSize, 16, 31*tileSize}, 10 },
        { {488*tileSize, state.logH - (100 - 74)*tileSize, 16, 7*tileSize}, 11 },
        { {466*tileSize, state.logH - (100 - 52)*tileSize, 16, 8*tileSize}, 12 },
        { {456*tileSize, state.logH - (100 - 76)*tileSize, 16, 5*tileSize}, 13 },
        { {392*tileSize, state.logH - (100 - 75)*tileSize, 16, 6*tileSize}, 14 },
        { {353*tileSize, state.logH - (100 - 75)*tileSize, 16, 6*tileSize}, 15 },
        { {315*tileSize, state.logH - (100 - 62)*tileSize, 16, 19*tileSize}, 16 },
        { {255*tileSize, state.logH - (100 - 75)*tileSize, 16, 5*tileSize}, 17 },
        { {182*tileSize, state.logH - (100 - 69)*tileSize, 16, 12*tileSize}, 18 },
        { {130*tileSize, state.logH - (100 - 74)*tileSize, 16, 6*tileSize}, 19 },
        { {98*tileSize, state.logH - (100 - 61)*tileSize, 16, 5*tileSize}, 20 },
        { {42*tileSize, state.logH - (100 - 66)*tileSize, 16, 4*tileSize}, 21 },
        { {31*tileSize, state.logH - (100 - 78)*tileSize, 16, 3*tileSize}, 22 }
    };
    gd.checkpoints_ = courseCheckpoints;
}

void createCheckpointsSpaceship(const SDLState &state, GameData &gd, const Resources &res) {
    printf("getting here");
    float tileSize = TILE_SIZE;
    //initialize the Rects for each checkpoint in form {{x*tileSize, state.logH - (MAP_ROWS - y)*tileSize, w, h*tileSize}, indx}
    courseCheckpoints = {
        { {3*tileSize, state.logH - (60 - 55)*tileSize, 16, 2*tileSize}, 0 },
        { {16*tileSize, state.logH - (60 - 50)*tileSize, 16, 4*tileSize}, 1 },
        { {18*tileSize, state.logH - (60 - 32)*tileSize, 16, 7*tileSize}, 2 },
        { {9*tileSize, state.logH - (60 - 32)*tileSize, 16, 7*tileSize}, 3 },
        { {8*tileSize, state.logH - (60 - 18)*tileSize, 16, 11*tileSize}, 4 },
        { {36*tileSize, state.logH - (60 - 18)*tileSize, 16, 11*tileSize}, 5 },
        { {43*tileSize, state.logH - (60 - 47)*tileSize, 16, 6*tileSize}, 6 },
        { {72*tileSize, state.logH - (60 - 46)*tileSize, 16, 7*tileSize}, 7 },
        { {98*tileSize, state.logH - (60 - 34)*tileSize, 16, 6*tileSize}, 8 },
        { {112*tileSize, state.logH - (60 - 34)*tileSize, 16, 6*tileSize}, 9 },
        { {119*tileSize, state.logH - (60 - 7)*tileSize, 16, 7*tileSize}, 10 },
        { {158*tileSize, state.logH - (60 - 8)*tileSize, 16, 45*tileSize}, 11 },
        { {200*tileSize, state.logH - (60 - 8)*tileSize, 16, 45*tileSize}, 12 },
        { {225*tileSize, state.logH - (60 - 8)*tileSize, 16, 45*tileSize}, 13 },
        { {237*tileSize, state.logH - (60 - 26)*tileSize, 16, 4*tileSize}, 14 },
    };
    gd.checkpoints_ = courseCheckpoints;

    for (const auto& cp : gd.checkpoints_) {
        printf("Checkppoint %d - x:%f y:%f h:%f\n", cp.index, cp.collider.x, cp.collider.y, cp.collider.h);
    }
}

void renderCheckpoints(SDL_Renderer* renderer, const std::vector<Checkpoint>& checkpoints, GameData &gd){
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // opaque green

    for (const auto& cp : checkpoints) {
        SDL_FRect screenRect = {
            (cp.collider.x - gd.mapViewport.x),
            (cp.collider.y - gd.mapViewport.y),
            (cp.collider.w),
            (cp.collider.h)
        };

        SDL_RenderFillRect(renderer, &screenRect);
    }
}