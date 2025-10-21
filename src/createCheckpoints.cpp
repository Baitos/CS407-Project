#include "../headers/createCheckpoints.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/player.h"
#include "../headers/globals.h"
#include "../headers/state.h"
#include "../headers/playerState.h"



extern GameState* currState;

std::vector<Checkpoint> courseCheckpointsGrassland;

void createCheckpointsGrassland(const SDLState &state, GameData &gd, const Resources &res) {
    float tileSize = TILE_SIZE;
    //initialize the Rects for each checkpoint in form {{x*tileSize, state.logH - (MAP_ROWS - y)*tileSize, w, h*tileSize}, indx}
    courseCheckpointsGrassland = {
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
    gd.checkpoints_ = courseCheckpointsGrassland;
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

        // printf("Rendering checkpoint %d: %d,%d %d x %d\n",
        //        cp.index, screenRect.x, screenRect.y, screenRect.w, screenRect.h);

        SDL_RenderFillRect(renderer, &screenRect);
    }
}