#include "../serverHeaders/updateMenu.h"
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

//Update for Character Select Screen
void charSelectUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
        

}

//Update for Settings Screen
void settingsUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    
}

//
//INPUT FUNCTIONS
//

//Input Function for Character Select Screen
void charSelectInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime){
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
                    //handleCharSelectKeyInput(state, gd, res, event.key, true, deltaTime);
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    //handleCharSelectKeyInput(state, gd, res, event.key, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    //handleCharSelectClick(state, gd, res, deltaTime);
                    break;
                    
                } 
            }
        }
}

//
//INPUT HANDLERS
//

//Mouse Cursor for Title/Settings/Char Select/Etc.

//Key Input Handler for Char Select
void handleCharSelectKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime) {

   
    if (key.scancode == SDL_SCANCODE_F1) {
        running = false;
    }
    if(key.scancode == SDL_SCANCODE_F2){
        currState = changeState(currState, gd);
        currState->init(state,gd, res);
    }
}
