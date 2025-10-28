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
        for (AnimatedObject &preview : gd.previews_) {
            preview.update(state, gd, res, deltaTime);
        }

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
                    handleCharSelectKeyInput(state, gd, res, event.key, true, deltaTime);
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    handleCharSelectKeyInput(state, gd, res, event.key, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    handleCharSelectClick(state, gd, res, deltaTime);
                    break;
                    
                } 
            }
        }
}
//Input function for Settings Screen
void settingsInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime){
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
                    
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    handleSettingsClick(state,gd,res,deltaTime);
                    break;
                    
                }  case SDL_EVENT_MOUSE_BUTTON_UP:

                {
                    gd.updatedDial = NULL;
                }
            }
        }
}

//
//INPUT HANDLERS
//

//Mouse Cursor for Title/Settings/Char Select/Etc.
void handleMousePointerCharSelect(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    gd.mouseCoords.x = gd.mouseCoords.x * xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };
    if((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big Border on Exit
        //printf("1\n");
        gd.settingsBorder.pos = glm::vec2(38,366);
        gd.settingsBorder.texture = res.texBigBorder;
    } else if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big border on Save
        //printf("2\n");
        gd.settingsBorder.pos = glm::vec2(586,366); 
        gd.settingsBorder.texture = res.texBigBorder;
    } else {
        //printf("3\n");
        gd.settingsBorder.pos = glm::vec2(500,500);
    }

    SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
    
}

void handleMousePointerSettings(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    gd.mouseCoords.x = gd.mouseCoords.x * xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * yRatio;
    SDL_FRect dst { 
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = (float)TILE_SIZE,
        .h = (float)TILE_SIZE
    };
    if(gd.updatedDial != NULL){
        gd.updatedDial->pos.x = gd.mouseCoords.x;
        if (gd.updatedDial->pos.x  > 290.f){ 
            gd.updatedDial->pos.x  = 290.f;
        } else if(gd.updatedDial->pos.x < 84.f) {
            gd.updatedDial->pos.x  = 84.f;
        }
        gd.updatedDial->draw(state,gd,static_cast<float>( gd.updatedDial->texture->w) * 2, static_cast<float>( gd.updatedDial->texture->h) * 2);

    } else {
        if((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big Border on Exit
            //printf("1\n");
            gd.settingsBorder.pos = glm::vec2(38,366);
            gd.settingsBorder.texture = res.texBigBorder;
        } else if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){ //Big border on Save
            //printf("2\n");
            gd.settingsBorder.pos = glm::vec2(585,366);
            gd.settingsBorder.texture = res.texBigBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 106 && gd.mouseCoords.y <= 126)){ //Sprint
            gd.settingsBorder.pos = glm::vec2(576,104);
            gd.settingsBorder.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 130 && gd.mouseCoords.y <= 150)){ //Grapple
            gd.settingsBorder.pos = glm::vec2(576,130);
            gd.settingsBorder.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 156 && gd.mouseCoords.y <= 174)){ //Ability
            gd.settingsBorder.pos = glm::vec2(576,156);
            gd.settingsBorder.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 182 && gd.mouseCoords.y <= 202)){ //Jump
            gd.settingsBorder.pos = glm::vec2(576,182);
            gd.settingsBorder.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 208 && gd.mouseCoords.y <= 228)){ //Use Item
            gd.settingsBorder.pos = glm::vec2(576,208);
            gd.settingsBorder.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 234 && gd.mouseCoords.y <= 254)){ //Pause
            gd.settingsBorder.pos = glm::vec2(576,234);
            gd.settingsBorder.texture = res.texSmallBorder;
        } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 260 && gd.mouseCoords.y <= 280)){ //Fast-Fall
            gd.settingsBorder.pos = glm::vec2(576,260);
            gd.settingsBorder.texture = res.texSmallBorder;
        } else {
            //printf("3\n");
            gd.settingsBorder.pos = glm::vec2(500,500);
        }
    }
    


    SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
    
}

//Key Input Handler for Char Select
void handleCharSelectKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime) {

    if (key.scancode == SDL_SCANCODE_F12 && key.down && !key.repeat) { // debug
            gd.debugMode = !gd.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F1) {
        running = false;
    }
    if(key.scancode == SDL_SCANCODE_F2){
        currState = changeState(currState, gd);
        currState->init(state,gd, res);
    }
}

//Handles Clicking for Character Select Screen
void handleCharSelectClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 156 && gd.mouseCoords.y <= 156+36)){
        //Set Icons and Player to Sword
        gd.previews_[0].texture = res.texSword;
        gd.previews_[0].pos = glm::vec2(530,200);
        gd.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = SWORD;
        for (charIconObject &ci : gd.charIcons_){
            ci.spriteFrame = ci.spriteFrame % 4;
            if (ci.spriteFrame == 3){
                ci.spriteFrame +=4;
                gd.charIcons_[0].spriteFrame = ci.spriteFrame;
            }
        }
    } else if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 220 && gd.mouseCoords.y <= 220+36)){
        //Set Icons and Player to Jetpack
        gd.previews_[0].texture = res.texJetpack;
        gd.previews_[0].pos = glm::vec2(510,200);
        gd.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = JETPACK;
        for (charIconObject &ci : gd.charIcons_){
            ci.spriteFrame = ci.spriteFrame % 4;
            if (ci.spriteFrame == 2){
                ci.spriteFrame +=4;
                gd.charIcons_[0].spriteFrame = ci.spriteFrame;
            }
        }
    } else if ((gd.mouseCoords.x >= 658 && gd.mouseCoords.x <= 658+34) && (gd.mouseCoords.y >= 284 && gd.mouseCoords.y <= 284+36)){
        //Set Icons and Player to Shotgun
        gd.previews_[0].texture = res.texShotgun;
        gd.previews_[0].pos = glm::vec2(520,200);
        gd.previews_[0].draw(state, gd, 96,96);
        gd.bgTiles_[0].draw(state, gd, 800,450);
        ((CharSelectState*) currState)->character = SHOTGUN;
        for (charIconObject &ci : gd.charIcons_){
            ci.spriteFrame = ci.spriteFrame % 4;
            if (ci.spriteFrame == 1){
                ci.spriteFrame +=4;
                gd.charIcons_[0].spriteFrame = ci.spriteFrame;
            }
        }
    } else if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) {
        //Enter Stage
        //TO DO - ONLY DO WHEN PLAYERS AGREE TO READY UP
        characterType character = ((CharSelectState*) currState)->character;
        currState = changeState(currState, gd);
        ((LevelState*) currState)->character = character;
        currState->init(state, gd, res);
    } else if ((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) {
        currState->nextStateVal = TITLE;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    }   
}

//Handles Clicking for Settings
void handleSettingsClick(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) {
        printf("Save\n");
    } else if ((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)) {
        currState->nextStateVal = CHAR_SELECT;
        currState = changeState(currState, gd);
        currState->init(state, gd, res);
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 106 && gd.mouseCoords.y <= 126)){ //Sprint
        printf("Sprint\n");
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 130 && gd.mouseCoords.y <= 150)){ //Grapple
        printf("Grapple\n");
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 156 && gd.mouseCoords.y <= 174)){ //Ability
        printf("Ability\n");
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 182 && gd.mouseCoords.y <= 202)){ //Jump
        printf("Jump\n");
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 208 && gd.mouseCoords.y <= 228)){ //Use Item
        printf("Use Item\n");
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 234 && gd.mouseCoords.y <= 254)){ //Pause
        printf("Pause\n");
    } else if((gd.mouseCoords.x >= 578 && gd.mouseCoords.x <= 690) && (gd.mouseCoords.y >= 260 && gd.mouseCoords.y <= 280)){ //Fast-Fall
        printf("Fast-fall\n");
    }   
    for(Object &o : gd.settingsDials_){
        
        if((gd.mouseCoords.x >= o.pos.x && gd.mouseCoords.x <= o.pos.x + (static_cast<float>(o.texture->w) * 2)) && (gd.mouseCoords.y >= o.pos.y && gd.mouseCoords.y <= o.pos.y + (static_cast<float>(o.texture->h) * 2))){
           gd.updatedDial = &o; 
        }
    }
}
