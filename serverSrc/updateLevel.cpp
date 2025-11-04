#include "../serverHeaders/updateLevel.h"
#include "../serverHeaders/collision.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/helper.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/resources.h"
#include "../serverHeaders/state.h"
#include "../serverHeaders/playerState.h"

using namespace std;

extern GameState * currState;

//
//UPDATE FUNCTIONS
//

//Update Function for level Spaceship
void levelUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    // update portals
    for (Portal &portal : gd.portals_) {
        portal.update(state, gd, res, deltaTime);
    }

    // update lasers
    for (Laser &laser : gd.lasers_) {
        laser.update(state, gd, res, deltaTime);
    }

    for (ItemBox &box : gd.itemBoxes_) {
        if (!box.itemBoxActive) {
            box.update(state, gd, res, deltaTime);
        }
    }

    for (Player &p : gd.players_) {
        p.update(state, gd, res, deltaTime);

        if (p.usingSugar) { // TODO: this currently draws below the screen, and this should probably be in draw
            //Draw sugar effect
            if(p.dir == 1) {
                glm::vec2 pos = glm::vec2(p.pos.x - 30.f, p.pos.y);
                SDL_FRect collider = {
                    .x = 28 * (p.dir),
                    .y = 0,
                    .w = 0.f,
                    .h = 0.f
                    };
                Object sugarEffectObject(pos, collider, res.texSugarEffectL);
                //sugarEffectObject.draw(state,gd,32,32);
            } else {
                glm::vec2 pos = glm::vec2(p.pos.x + 30.f, p.pos.y);
                SDL_FRect collider = {
                    .x = 28 * (p.dir),
                    .y = 0,
                    .w = 0.f,
                    .h = 0.f
                    };
                Object sugarEffectObject(pos, collider, res.texSugarEffectR);
                //sugarEffectObject.draw(state,gd,32,32);
            }
        }
    }

    //printf("%d\n", gd.player.state_->currStateVal);

    gd.itemStorage_.pos.x = gd.players_[0].pos.x - 368;
    gd.itemStorage_.pos.y = gd.players_[0].pos.y - 200;

}

//
//INPUT FUNCTIONS
//

//Input Function for level Spaceship
void levelInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    SDL_Event event { 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: 
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    //printf("Width = %d, Height = %d", state.width, state.height);
                    break;
                }
                case SDL_EVENT_KEY_DOWN:
                {
                    //handleKeyInput(state, gd, res, event.key, true, deltaTime);
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    //handleKeyInput(state, gd, res, event.key, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    for (Player &p : gd.players_) {
                        //handleLevelClick(state, gd, res, p, deltaTime, event, true);
                    }
                    break;
                } 
                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    for (Player &p : gd.players_) {
                        //handleLevelClick(state, gd, res, p, deltaTime, event, false);
                    }
                    break;
                } 
            }
        }
}


//
//INPUT HANDLERS
//

//Crosshair for in level


//Key Input Handler for Level
void handleKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    int key, bool keyDown, float deltaTime) {

    if (key == SDL_SCANCODE_F11 && keyDown) { // tp to entrance portal
        gd.players_[0].pos = gd.EntrancePortal;
        gd.players_[0].pos.x -= TILE_SIZE;
    }
    if (key == SDL_SCANCODE_F10 && keyDown) { // tp to exit portal
        gd.players_[0].pos = gd.ExitPortal;
    }
    /*if (key.scancode == SDL_SCANCODE_F2 && keyDown && !key.repeat) { // anti gravity
        gd.player().flip = -1 * gd.player().flip;
    }*/
    if (key == SDL_SCANCODE_F1) {
        printf("Player Left\n");
    }
    if (key == SDL_SCANCODE_F2) {
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }
    if (key == SDL_SCANCODE_F3) {
        
        gd.players_[0].pos.x = 950;
        gd.players_[0].pos.y = -654;
    }
    // for (Player &p : gd.players_) {
    //     p.handleInput(state, gd, res, key, deltaTime); 
    //     if (key.scancode == SDL_SCANCODE_Q && p.hasItem) {
    //         Item item = p.item;
    //         item.useItem(state, gd, res, p);
    //         p.hasItem = false;
    //         clearItem(state, gd, res);
    //     }
    // }  
}