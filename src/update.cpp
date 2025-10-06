#include "../headers/update.h"
#include "../headers/collision.h"
#include "../headers/gameData.h"
#include "../headers/initState.h"
#include "../headers/helper.h"
#include "../headers/globals.h"
#include "../headers/resources.h"
#include "../headers/state.h"

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
}
//Update for Character Select Screen
void charSelectUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
        for (AnimatedObject &preview : gd.previews_) {
            preview.update(state, gd, res, deltaTime);
        }

}

//
//INPUT FUNCTIONS
//

//Input Function for level Spaceship
void levelInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime){
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
                    handleKeyInput(state, gd, res, event.key, true, deltaTime);
                    
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    handleKeyInput(state, gd, res, event.key, false, deltaTime);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    //handleClick(state, gd, res, gd.player(), deltaTime);
                    break;
                } 
            }
        }
}
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

//
//INPUT HANDLERS
//

//Crosshair for in level
void handleCrosshair(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
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
        .w = CROSSHAIR_SIZE,
        .h = CROSSHAIR_SIZE
    };
    SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255); // draw line to crosshair
    glm::vec2 pOffset = findCenterOfSprite(gd.player);
    //printf("x: %d, y: %d\n", pOffset.x, pOffset.y);
    SDL_RenderLine(state.renderer, gd.player.pos.x - gd.mapViewport.x + pOffset.x, gd.player.pos.y - gd.mapViewport.y + pOffset.y, gd.mouseCoords.x, gd.mouseCoords.y);
    SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
    
    //printf("mouseX: %f, mouseY: %f\n", gd.mouseCoords.x, gd.mouseCoords.y);
    SDL_RenderTexture(state.renderer, res.texCrosshair, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
}
//Mouse Cursor for Title/Settings/Char Select/Etc.
void handleMousePointer(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
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
        .w = 32,
        .h = 32
    };
    SDL_RenderTexture(state.renderer, res.texCursor, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/ 
}
//Key Input Handler for Level
void handleKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime) {

    if (key.scancode == SDL_SCANCODE_F12 && key.down && !key.repeat) { // debug
            gd.debugMode = !gd.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F11 && key.down && !key.repeat) { // tp to entrance portal
        gd.player.pos = gd.EntrancePortal;
        gd.player.pos.x -= TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_F10 && key.down && !key.repeat) { // tp to exit portal
        gd.player.pos = gd.ExitPortal;
    }
    /*if (key.scancode == SDL_SCANCODE_F2 && keyDown && !key.repeat) { // anti gravity
        gd.player().flip = -1 * gd.player().flip;
    }*/
    if (key.scancode == SDL_SCANCODE_F1) {
        running = false;
    }
    if(key.scancode == SDL_SCANCODE_F2){
        printf("F2 key clicked");
        currState = changeState(currState, gd);
        currState->init(state,gd, res);
    }
    if (key.scancode == SDL_SCANCODE_D && key.down) {
        gd.player.pos.x += 2 * TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_A && key.down) {
        gd.player.pos.x -= 2 * TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_W && key.down) {
        gd.player.pos.y -= 2 * TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_S && key.down) {
        gd.player.pos.y += 2 * TILE_SIZE;
    }

    // if (obj.type == ObjectType::player) {
    //     const float JUMP_FORCE = -450.f;
    //     const auto handleJumping = [&state, &gd, &obj, res, key, keyDown, JUMP_FORCE]() {
    //         if (key.scancode == SDL_SCANCODE_SPACE && keyDown && !key.repeat) { // jumping
    //             if (obj.grounded) { // single jump
    //                 //add something for jump animation before going up
    //                 //obj.data.player.state = PlayerState::jumping;
    //                  obj.data.player.state = PlayerState::jumpLaunch; // <-- use launch state
    //                  obj.texture = res.texLaunch;
    //                  obj.curAnimation = res.ANIM_PLAYER_LAUNCH; 
    //                  obj.animations[obj.curAnimation].reset();
    //                  obj.vel.y = changeVel(JUMP_FORCE, obj); 
    //             } else if (obj.data.player.canDoubleJump) { // double jump
    //                 obj.data.player.state = PlayerState::jumping;
    //                 obj.vel.y = changeVel(JUMP_FORCE, obj);  
    //                 obj.data.player.canDoubleJump = false;
    //                 obj.gravityScale = 1.0f; // reset gravity
    //             }
    //             //printf("canDoubleJump = %d\n" , obj.data.player.canDoubleJump);
    //         }
    //         else if (!keyDown && key.scancode == SDL_SCANCODE_SPACE) { // letting go of jump
    //             /*if (obj.vel.y < 0) { // OPTION 1: Set velocity to 0 when you let go. makes sharp but precise jumps
    //                 obj.vel.y = 0;
    //             }*/
    //             float termVel = -200.0f; // option 2: Set velocity to predefined amount when you let go. makes less sharp jumps
    //             float shouldFlip = obj.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
    //             if (shouldFlip * obj.vel.y < shouldFlip * termVel) { 
    //                 obj.vel.y = changeVel(termVel, obj);
    //             }
    //             //obj.gravityScale = 2.0f; // option 3; double their gravity until they land
    //         }
    //     };
    //     const auto handleRunning = [&state, &gd, &obj, &res, key, keyDown, deltaTime]() {
    //         if (key.scancode == SDL_SCANCODE_LSHIFT) {
    //             if (keyDown) { // if held down, increase speed
    //                 obj.maxSpeedX = obj.data.player.maxRunX;
    //                 obj.curAnimation = res.ANIM_PLAYER_RUN;
    //             } else {
    //                 obj.maxSpeedX = obj.data.player.maxWalkX;
    //                 obj.curAnimation = res.ANIM_PLAYER_WALK; 
    //                 obj.data.player.sprintTimer.reset();
    //             }
    //         }
    //     };
    //     const auto handleSprinting = [&state, &gd, &obj, &res, key, keyDown]() {
    //         if (key.scancode == SDL_SCANCODE_LSHIFT && !keyDown) {
    //             obj.maxSpeedX = obj.data.player.maxWalkX;
    //             obj.curAnimation = res.ANIM_PLAYER_WALK;
    //             obj.data.player.sprintTimer.reset();
    //             obj.data.player.state = PlayerState::moving;
    //         }
    //     };
    //     const auto handleFalling = [&state, &gd, &obj, &res, key, keyDown, deltaTime]() {
    //         if (key.scancode == SDL_SCANCODE_S && keyDown && !obj.grounded) { // fastfall
    //             if (!key.repeat && !obj.data.player.fastfalling) {
    //                 obj.vel.y = changeVel(-250.0f, obj);
    //                 obj.data.player.fastfalling = true;
    //                 obj.data.player.state = PlayerState::jumping;
    //                 obj.curAnimation = res.ANIM_PLAYER_JUMP;
    //                 //obj.data.player.canDoubleJump = false;
    //             }
    //             obj.gravityScale = 3.0f;
    //         }
    //     };
    //     switch (obj.data.player.state) {
    //         case PlayerState::idle:
    //         {
    //             handleJumping();
    //             handleRunning();
    //             break;
    //         }
    //         case PlayerState::moving:
    //         {
    //             handleJumping();
    //             handleRunning();
    //             handleFalling();
    //             break;
    //         }
    //         case PlayerState::jumping:
    //         {
    //             handleRunning();
    //             handleJumping();
    //             handleFalling();
    //             break;               
    //         }
    //         case PlayerState::jumpLaunch:
    //         {
    //             handleRunning();
    //             handleJumping();
    //             handleFalling();
    //             break;               
    //         }
    //         case PlayerState::roll:
    //         {
    //             handleRunning();
    //             handleJumping();
    //             handleFalling();
    //             break;               
    //         }
    //         case PlayerState::sprinting:
    //         {
    //             handleSprinting();
    //             handleJumping();
    //             handleFalling();
    //             break;
    //         }
    //     }
    //     //printf("velX = %f, velY = %f\n", obj.vel.x, obj.vel.y);
    // }
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
    } else if ((gd.mouseCoords.x >= 35 && gd.mouseCoords.x <= 218) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){
        //Enter Stage
        //TO DO - ONLY DO WHEN PLAYERS AGREE TO READY UP
        int character = ((CharSelectState*) currState)->character;
        currState = changeState(currState, gd);
        ((LevelState*) currState)->character = character;
        currState->init(state,gd, res);
    } else if ((gd.mouseCoords.x >= 583 && gd.mouseCoords.x <= 766) && (gd.mouseCoords.y >= 363 && gd.mouseCoords.y <= 434)){
        //Exit to Title
        currState->nextStateVal = TITLE;
        currState = changeState(currState, gd);
        currState->init(state,gd, res);
    }
    
}

