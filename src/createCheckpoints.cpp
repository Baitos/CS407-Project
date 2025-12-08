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
    //initialize the Rects for each checkpoint in form {{x*tileSize, y*tileSize, w, h*tileSize}, indx}
    courseCheckpoints = {
        { {31*tileSize, 15*tileSize, 16, 32*tileSize}, 0 },
        { {66*tileSize, 14*tileSize, 16, 28*tileSize}, 1 },
        { {130*tileSize, 19*tileSize, 16, 28*tileSize}, 2 },
        { {199*tileSize, 16*tileSize, 16, 31*tileSize}, 3 },
        { {276*tileSize, 43*tileSize, 16, 4*tileSize}, 4 },
        { {283*tileSize, 18*tileSize, 16, 4*tileSize}, 5 },
        { {382*tileSize, 14*tileSize, 16, 29*tileSize}, 6 },
        { {391*tileSize, 15*tileSize, 16, 4*tileSize}, 7 },
        { {398*tileSize, 43*tileSize, 16, 4*tileSize}, 8 },
        { {425*tileSize, 16*tileSize, 16, 14*tileSize}, 9 },
        { {489*tileSize, 16*tileSize, 16, 31*tileSize}, 10 },
        { {488*tileSize, 74*tileSize, 16, 7*tileSize}, 11 },
        { {466*tileSize, 52*tileSize, 16, 8*tileSize}, 12 },
        { {456*tileSize, 76*tileSize, 16, 5*tileSize}, 13 },
        { {392*tileSize, 75*tileSize, 16, 6*tileSize}, 14 },
        { {353*tileSize, 75*tileSize, 16, 6*tileSize}, 15 },
        { {315*tileSize, 62*tileSize, 16, 19*tileSize}, 16 },
        { {255*tileSize, 75*tileSize, 16, 5*tileSize}, 17 },
        { {182*tileSize, 69*tileSize, 16, 12*tileSize}, 18 },
        { {130*tileSize, 74*tileSize, 16, 5*tileSize}, 19 },
        { {98*tileSize, 61*tileSize, 16, 5*tileSize}, 20 },
        { {42*tileSize, 66*tileSize, 16, 4*tileSize}, 21 },
        { {31*tileSize, 78*tileSize, 16, 3*tileSize}, 22 }
    };
    gd.checkpoints_ = courseCheckpoints;
}

void createCheckpointsSpaceship(const SDLState &state, GameData &gd, const Resources &res) {
    float tileSize = TILE_SIZE;
    //initialize the Rects for each checkpoint in form {{x*tileSize, y*tileSize, w, h*tileSize}, indx}
    courseCheckpoints = {
        { {3*tileSize, 55*tileSize, 16, 2*tileSize}, 0 },
        { {16*tileSize, 50*tileSize, 16, 4*tileSize}, 1 },
        { {18*tileSize, 32*tileSize, 16, 7*tileSize}, 2 },
        { {9*tileSize, 32*tileSize, 16, 7*tileSize}, 3 },
        { {8*tileSize, 18*tileSize, 16, 11*tileSize}, 4 },
        { {36*tileSize, 18*tileSize, 16, 11*tileSize}, 5 },
        { {43*tileSize, 47*tileSize, 16, 6*tileSize}, 6 },
        { {72*tileSize, 46*tileSize, 16, 7*tileSize}, 7 },
        { {98*tileSize, 34*tileSize, 16, 6*tileSize}, 8 },
        { {112*tileSize, 34*tileSize, 16, 6*tileSize}, 9 },
        { {119*tileSize, 7*tileSize, 16, 7*tileSize}, 10 },
        { {158*tileSize, 8*tileSize, 16, 45*tileSize}, 11 },
        { {200*tileSize, 8*tileSize, 16, 45*tileSize}, 12 },
        { {225*tileSize, 8*tileSize, 16, 45*tileSize}, 13 },
        { {237*tileSize, 26*tileSize, 16, 4*tileSize}, 14 },
    };
    gd.checkpoints_ = courseCheckpoints;

    /*for (const auto& cp : gd.checkpoints_) {
        printf("Checkppoint %d - x:%f y:%f h:%f\n", cp.index, cp.collider.x, cp.collider.y, cp.collider.h);
    }*/
}

void createCheckpointsSnow(const SDLState &state, GameData &gd, const Resources &res) {
    float tileSize = TILE_SIZE;
    //initialize the Rects for each checkpoint in form {{x*tileSize, y*tileSize, w, h*tileSize}, indx}
    courseCheckpoints = {
        { {21*tileSize, 84*tileSize, 16, 2*tileSize}, 0 },
        { {88*tileSize, 6*tileSize, 16, 80*tileSize}, 1 },
        { {127*tileSize, 12*tileSize, 16, 74*tileSize}, 2 },
        { {169*tileSize, 9*tileSize, 16, 77*tileSize}, 3 },
        { {208*tileSize, 4*tileSize, 16, 45*tileSize}, 4 },
        { {274*tileSize, 8*tileSize, 16, 30*tileSize}, 5 },
        { {326*tileSize, 6*tileSize, 16, 69*tileSize}, 6 },
        { {392*tileSize, 4*tileSize, 16, 71*tileSize}, 7 },
        { {458*tileSize, 7*tileSize, 16, 62*tileSize}, 8 },
        { {493*tileSize, 67*tileSize, 16, 2*tileSize}, 9 },
    };
    gd.checkpoints_ = courseCheckpoints;

    /*for (const auto& cp : gd.checkpoints_) {
        printf("Checkppoint %d - x:%f y:%f h:%f\n", cp.index, cp.collider.x, cp.collider.y, cp.collider.h);
    }*/
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