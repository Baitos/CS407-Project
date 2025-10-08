#include <SDL3/SDL.h>

#include "../headers/gameData.h"
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/helper.h"
#include "../headers/resources.h"
#include "../headers/state.h"
#include <vector>

extern GameState * currState;
const float JUMP_FORCE = -450.f;


//Handle Input Functions
void handleInputIdle(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    handleJumping(gd,res,key);
    handleRunning(gd,res,key);
}

void handleInputWalk(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    handleJumping(gd,res,key);
    handleRunning(gd,res,key);
    handleFalling(gd,res,key);
}

void handleInputRun(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    handleJumping(gd,res,key);
    handleRunning(gd,res,key);
    handleFalling(gd,res,key);
}

void handleInputJump(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    handleRunning(gd,res,key);
    handleJumping(gd,res,key);
    handleFalling(gd,res,key);
}

void handleInputLaunch(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    handleRunning(gd,res,key);
    handleJumping(gd,res,key);
    handleFalling(gd,res,key);
}

void handleInputRoll(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    handleRunning(gd,res,key);
    handleJumping(gd,res,key);
    handleFalling(gd,res,key);
}

void handleInputSprint(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    handleSprinting(gd,res,key);
    handleJumping(gd,res,key);
    handleFalling(gd,res,key);
}

//Might not need, idk how it would work with a nullptr as a function in the loop
void dummyInput(GameData &gd, Resources &res, SDL_KeyboardEvent key){
}

void handleInputJetpackDeploy(GameData &gd, Resources &res, SDL_KeyboardEvent key) {
    handleJumping(gd,res,key);
}

void handleInputShotgunDeploy(GameData &gd, Resources &res, SDL_KeyboardEvent key) {
    handleJumping(gd,res,key);
}

void handleInputSwordDeploy(GameData &gd, Resources &res, SDL_KeyboardEvent key) {
    handleJumping(gd,res,key);
}

//Update Functions
void updateIdle(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    //gd.player.vel.x = 0;    
    sharedUpdate(state, gd,res,deltaTime);
    if(gd.player.currentDirection) { // if moving change to running
        gd.player.state_->nextStateVal = WALK;
        PlayerState * newState = changePlayerState(gd.player.state_);
        //PlayerState * oldState = gd.player.state_;
        delete gd.player.state_;
        gd.player.state_ = newState;
        //delete oldState;
        gd.player.state_->enter(gd.player, gd, res);
    }
}

void updateWalk(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    
    sharedUpdate(state, gd,res,deltaTime);
    //printf("currentDirection: %d\n", gd.player.currentDirection);
    //printf("Velocity: %f\n", gd.player.vel.x);
    if (!gd.player.currentDirection && gd.player.grounded) { // if not moving, slow down
        //printf("Slowing Walk\n");
        //printf("currentDirection: %d\n", gd.player.currentDirection);
        const float factor = gd.player.vel.x > 0 ? -1.0f : 1.0f;
        float amount = factor * gd.player.acc.x * deltaTime;
        
        if (std::abs(gd.player.vel.x) < std::abs(amount)) {
            //printf("Walk to Idle\n");
            gd.player.vel.x = 0;
            // once stopped, set player to idle
            gd.player.state_->nextStateVal = IDLE;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);
        }
        else {
            //printf("Velocity before: %f\n", gd.player.vel.x);
            gd.player.vel.x += amount;
            //printf("Velocity After: %f\n", gd.player.vel.x);
        }
    }

    if (isSliding(gd.player) && gd.player.grounded) { // moving in different direction of vel and pressing a direction, sliding
        //TO DO, FIGURE OUT SLIDING MAYBE?
        //gd.player.state_->nextStateVal = TURNING;
        //PlayerState * newState = changePlayerState(gd.player.state_);
        //delete gd.player.state_;
        //gd.player.sprinting = newState;
    } else {
        /*gd.player.state_->nextStateVal = RUN;
        PlayerState * newState = changePlayerState(gd.player.state_);
        delete gd.player.state_;
        gd.player.state_ = newState;
        gd.player.state_->enter(gd.player, gd, res);
        */
    }

    if (state.keys[SDL_SCANCODE_LSHIFT]) { // if not pressing then reset
        float LEEWAY = 20;
        if (gd.player.grounded && std::abs(gd.player.vel.x) >= (gd.player.maxRunX - LEEWAY)) { // if grounded and moving fast enter sprint (eventually)                
            if (!gd.player.sprintTimer.isTimeOut()) {
                gd.player.sprintTimer.step(deltaTime);
            } else {

                gd.player.maxSpeedX = gd.player.maxSprintX;
                gd.player.state_->nextStateVal = SPRINT;
                PlayerState * newState = changePlayerState(gd.player.state_);
                delete gd.player.state_;
                gd.player.state_ = newState;
                gd.player.state_->enter(gd.player, gd, res);
            }
        } 
    } else {
        gd.player.sprintTimer.reset();
    }
}

void updateSprint(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    sharedUpdate(state, gd,res,deltaTime);
    float LEEWAY = 20;
    if (gd.player.grounded && // if on ground and sliding or too slow reset sprint
        (isSliding(gd.player) || std::abs(gd.player.vel.x) < (gd.player.maxRunX - LEEWAY))) {       
        gd.player.sprintTimer.reset();
        gd.player.maxSpeedX = gd.player.maxRunX;
        gd.player.state_->nextStateVal = WALK;
        PlayerState * newState = changePlayerState(gd.player.state_);
        delete gd.player.state_;
        gd.player.state_ = newState;
        gd.player.state_->enter(gd.player, gd, res);
    }
}

void updateJump(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    sharedUpdate(state, gd,res,deltaTime);
    if(gd.player.fastFalling && gd.player.grounded){
        gd.player.fastFalling = false;
        gd.player.state_->nextStateVal = ROLL;
        PlayerState * newState = changePlayerState(gd.player.state_);
        delete gd.player.state_;
        gd.player.state_ = newState;
        gd.player.state_->enter(gd.player, gd, res);
        gd.player.vel.y = changeVel(0, gd.player);
        
    } else {
        if(gd.player.grounded && gd.player.dir == 0){
            gd.player.state_->nextStateVal = IDLE;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);
        } else if(gd.player.grounded){
            gd.player.state_->nextStateVal = WALK;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);  
        }
    }
        
    
}

void updateFalling(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    sharedUpdate(state, gd,res,deltaTime);

    // gd.player.state_->nextStateVal = FALL;
    // PlayerState * newState = changePlayerState(gd.player.state_);
    // delete gd.player.state_;
    // gd.player.state_ = newState;
    // gd.player.state_->enter(gd.player, gd, res);
}
      
void updateLaunch(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    sharedUpdate(state, gd,res,deltaTime);
        
    // gd.player.state_->nextStateVal = LAUNCH;
    // PlayerState * newState = changePlayerState(gd.player.state_);
    // delete gd.player.state_;
    // gd.player.state_ = newState;
    // gd.player.state_->enter(gd.player, gd, res);

    if (gd.player.animations[gd.player.curAnimation].isDone()) {
        gd.player.state_->nextStateVal = JUMP;
        PlayerState * newState = changePlayerState(gd.player.state_);
        delete gd.player.state_;
        gd.player.state_ = newState;
        gd.player.state_->enter(gd.player, gd, res);
        gd.player.animations[gd.player.curAnimation].reset();
    }
}

void updateRoll(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    sharedUpdate(state, gd,res,deltaTime);
    // gd.player.state_->nextStateVal = ROLL;
    // PlayerState * newState = changePlayerState(gd.player.state_);
    // delete gd.player.state_;
    // gd.player.state_ = newState;
    // gd.player.state_->enter(gd.player, gd, res);

    // when roll animation finishes, switch to moving
    if (gd.player.animations[gd.player.curAnimation].isDone()) {
        gd.player.state_->nextStateVal = RUN;
        PlayerState * newState = changePlayerState(gd.player.state_);
        delete gd.player.state_;
        gd.player.state_ = newState;
        gd.player.state_->enter(gd.player, gd, res);
        gd.player.animations[gd.player.curAnimation].reset();
    }
}

void dummyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    sharedUpdate(state, gd,res,deltaTime);
    if(gd.player.grounded) { // if moving change to running
        gd.player.state_->nextStateVal = ROLL;
        PlayerState * newState = changePlayerState(gd.player.state_);
        delete gd.player.state_;
        gd.player.state_ = newState;
        gd.player.state_->enter(gd.player, gd, res);
    }
}

void updateJetpackDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    sharedUpdate(state, gd,res,deltaTime);
    int vertDir = 0;
    //calculate direction
     if (state.keys[SDL_SCANCODE_W]) {
        vertDir += -1.f;
    }
    if (state.keys[SDL_SCANCODE_S]) {
        vertDir += 1.f;
    }
    if(gd.player.currentDirection == 1 && vertDir == 0) {
        gd.player.vel.x += 5.f;
    } else if (gd.player.currentDirection == -1 && vertDir == 0) {
        gd.player.vel.x -= 5.f;
    } else if (gd.player.currentDirection == 0 && vertDir == 1) {
        gd.player.vel.y += 5.f;
    } else if (gd.player.currentDirection == 0 && vertDir == -1) {
        gd.player.vel.y -= 5.f;
    } else if (gd.player.currentDirection == 1 && vertDir == 1) {
        gd.player.vel.x += (5/sqrt(2));
        gd.player.vel.y += (5/sqrt(2));
    } else if (gd.player.currentDirection == 1 && vertDir == -1) {
        gd.player.vel.x += (5/sqrt(2));
        gd.player.vel.y -= (5/sqrt(2));
    } else if (gd.player.currentDirection == -1 && vertDir == 1) {
        gd.player.vel.x -= (5/sqrt(2));
        gd.player.vel.y += (5/sqrt(2));
    } else if (gd.player.currentDirection == -1 && vertDir == -1) {
        gd.player.vel.x -= (5/sqrt(2));
        gd.player.vel.y -= (5/sqrt(2));
    }

    //Check timer for state change
    if (!gd.player.jetpackTimer.isTimeOut()) {
        gd.player.jetpackTimer.step(deltaTime);
    } else {
        gd.player.cooldownTimer.reset();
        if(gd.player.vel.x != 0) {
            gd.player.state_->nextStateVal = WALK;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);
        } else {
            gd.player.state_->nextStateVal = IDLE;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);
        }
    }
}

void updateShotgunDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    sharedUpdate(state, gd, res, deltaTime);

    if(gd.player.blast->animations[gd.player.blast->curAnimation].isDone()) { 
        delete gd.player.blast;
        gd.player.blast = nullptr;
        gd.player.cooldownTimer.reset();
        if(gd.player.vel.x != 0) {
            gd.player.state_->nextStateVal = WALK;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);
        } else {
            gd.player.state_->nextStateVal = IDLE;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);
        }
    } else {
        if(gd.player.dir == 1) {
            gd.player.blast->pos = glm::vec2(gd.player.pos.x + 32, gd.player.pos.y - 4);
            gd.player.blast->dir = 1;
        } else if (gd.player.dir == -1) {
            gd.player.blast->pos = glm::vec2(gd.player.pos.x - 80, gd.player.pos.y - 4);
            gd.player.blast->dir = -1;
        }
    }
}

void updateSwordDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    sharedUpdate(state, gd, res, deltaTime);

     if(gd.player.animations[gd.player.curAnimation].isDone()) { 
        gd.player.cooldownTimer.reset();
        if(gd.player.vel.x != 0) {
            gd.player.state_->nextStateVal = WALK;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);
        } else {
            gd.player.state_->nextStateVal = IDLE;
            PlayerState * newState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = newState;
            gd.player.state_->enter(gd.player, gd, res);
        }
    }
}

//Enter Functions
void enterIdle(Player& player, GameData &gd, Resources &res){
    if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texIdleS;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texIdleG;
    } else {
        player.texture = res.texIdleJ;
    }
    player.curAnimation = res.ANIM_PLAYER_IDLE; 
    player.animations[gd.player.curAnimation].reset();
    player.vel.x = 0;
}

void enterWalk(Player& player, GameData &gd, Resources &res){
    if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texRunS;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texRunG;
    } else {
        player.texture = res.texRunJ;
    }
    player.curAnimation = res.ANIM_PLAYER_WALK; 
    player.animations[gd.player.curAnimation].reset();
}

void enterRun(Player& player, GameData &gd, Resources &res){
    if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texRunS;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texRunG;
    } else {
        player.texture = res.texRunJ;
    }
    player.curAnimation = res.ANIM_PLAYER_RUN; 
    player.animations[gd.player.curAnimation].reset();
}

void enterSprint(Player& player, GameData &gd, Resources &res){
    if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texRunS;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texRunG;
    } else {
        player.texture = res.texRunJ;
    }
    //Change?
    player.curAnimation = res.ANIM_PLAYER_RUN; 
    player.animations[gd.player.curAnimation].reset();
}

void enterLaunch(Player& player, GameData &gd, Resources &res){
    
    if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texLaunchS;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texLaunchG;
    } else {
        player.texture = res.texLaunchJ;
    }
    player.curAnimation = res.ANIM_PLAYER_LAUNCH; 
    player.animations[gd.player.curAnimation].reset();
}

void enterJump(Player& player, GameData &gd, Resources &res){
    if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texJumpS;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texJumpG;
    } else {
        player.texture = res.texJumpJ;
    }
    player.curAnimation = res.ANIM_PLAYER_JUMP; 
    player.animations[gd.player.curAnimation].reset();
}

void enterRoll(Player& player, GameData &gd, Resources &res){
    if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texRollS;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texRollG;
    } else {
        player.texture = res.texRollJ;
    }
    player.curAnimation = res.ANIM_PLAYER_ROLL; 
    player.animations[gd.player.curAnimation].reset();
}

void enterFall(Player& player, GameData &gd, Resources &res){
    /*if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texShot;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texLaunchG;
    } else {
        player.texture = res.texLaunchJ;
    }
    player.curAnimation = res.ANIM_PLAYER_LAUNCH; 
    player.animations[gd.player.curAnimation].reset();*/
}

void enterDead(Player& player, GameData &gd, Resources &res){
    if(((LevelState * )(currState))->character == SWORD){
        player.texture = res.texDieS;
    } else if(((LevelState * )(currState))->character == SHOTGUN){
        player.texture = res.texDieG;
    } else {
        player.texture = res.texDieJ;
    }
    player.curAnimation = res.ANIM_PLAYER_DIE; 
    player.animations[gd.player.curAnimation].reset();
}

void enterJetpackDeploy(Player& player, GameData &gd, Resources &res) {

    player.texture = res.texJDeploy;
    player.curAnimation = res.ANIM_PLAYER_JETPACK_DEPLOY;
    player.animations[gd.player.curAnimation].reset();

    gd.player.jetpackTimer.reset();
}

void enterShotgunDeploy(Player& player, GameData &gd, Resources &res) {

    player.texture = res.texGDeploy;
    player.curAnimation = res.ANIM_PLAYER_SHOOT;
    player.animations[gd.player.curAnimation].reset();
    //load in shotgun
    SDL_FRect collider = { 
                            .x = 0,
                            .y = 12,
                            .w = 80,
                            .h = 24
                        };
    glm::vec2 pos;
    AnimatedObject* blast = new AnimatedObject(pos, collider, res.texShotgunBlast);
    if(player.dir == 1) {
        blast->pos = glm::vec2(player.pos.x + 32, player.pos.y - 4);
        blast->dir = 1;
    } else if (player.dir == -1) {
        blast->pos = glm::vec2(player.pos.x - 80, player.pos.y - 4);
        blast->dir = -1;
    }
    blast->animations = res.shotgunAnims;
    blast->curAnimation = res.SHOTGUN_BLAST;
    gd.player.blast = blast;
}

void enterSwordDeploy(Player& player, GameData &gd, Resources &res) {
    player.texture = res.texSDeploy;
    player.curAnimation = res.ANIM_PLAYER_SWORD_DEPLOY;
    player.animations[gd.player.curAnimation].reset();
}

//Handlers
void handleJumping (GameData &gd, Resources &res, SDL_KeyboardEvent key) {
    if (key.scancode == SDL_SCANCODE_SPACE && key.down && !key.repeat) {
        //printf("Jump handled");
        //printf("%d", gd.player.grounded);
        if (gd.player.grounded) { // single jump
            gd.player.state_->nextStateVal = LAUNCH;
            PlayerState * tempState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = tempState;
            gd.player.state_->enter(gd.player, gd, res);
            gd.player.vel.y = changeVel(JUMP_FORCE, gd.player); 
        } else if (gd.player.canDoubleJump) { // double jump
            gd.player.state_->nextStateVal = JUMP;
            PlayerState * tempState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = tempState;
            gd.player.state_->enter(gd.player, gd, res);
            gd.player.vel.y = changeVel(JUMP_FORCE, gd.player);  
            gd.player.canDoubleJump = false;
            gd.player.gravityScale = 1.0f; // reset gravity
        }
    } else if (!key.down && key.scancode == SDL_SCANCODE_SPACE) { // letting go of jump
            float termVel = -200.0f; // option 2: Set velocity to predefined amount when you let go. makes less sharp jumps
            float shouldFlip = gd.player.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
            if (shouldFlip * gd.player.vel.y < shouldFlip * termVel) { 
                gd.player.vel.y = changeVel(termVel, gd.player);
            }
        }
}

void handleRunning (GameData &gd, Resources &res, SDL_KeyboardEvent key){
    if (key.scancode == SDL_SCANCODE_LSHIFT) {
        //printf("running handled");
            if (key.down) { // if held down, increase speed
                gd.player.maxSpeedX = gd.player.maxRunX;
                gd.player.state_->nextStateVal = RUN;
                PlayerState * tempState = changePlayerState(gd.player.state_);
                delete gd.player.state_;
                gd.player.state_ = tempState;
                gd.player.state_->enter(gd.player, gd, res);
            } else {
                gd.player.maxSpeedX = gd.player.maxWalkX;
                gd.player.state_->nextStateVal = WALK;
                PlayerState * tempState = changePlayerState(gd.player.state_);
                delete gd.player.state_;
                gd.player.state_ = tempState;
                gd.player.state_->enter(gd.player, gd, res);
                gd.player.sprintTimer.reset();
            }
        }
}

void handleSprinting(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    if (key.scancode == SDL_SCANCODE_LSHIFT && !key.down) {
        //printf("Sprinting handled");
        gd.player.maxSpeedX = gd.player.maxWalkX;
        gd.player.curAnimation = res.ANIM_PLAYER_WALK;
        gd.player.sprintTimer.reset();

        gd.player.state_->nextStateVal = WALK;
        PlayerState *newState = changePlayerState(gd.player.state_);
        delete gd.player.state_;
        gd.player.state_ = newState;
        gd.player.state_->enter(gd.player, gd, res);
    }
}

void handleFalling(GameData &gd, Resources &res, SDL_KeyboardEvent key){
    if (key.scancode == SDL_SCANCODE_S && key.down && !gd.player.grounded) { // fastfall
        //printf("Falling handled");
        if (!key.repeat && !gd.player.fastFalling) {
            gd.player.vel.y = changeVel(-250.0f, gd.player);
            gd.player.fastFalling = true;

            //Enter Jumping State
            /*gd.player.state_->nextStateVal = JUMP;
            PlayerState * tempState = changePlayerState(gd.player.state_);
            delete gd.player.state_;
            gd.player.state_ = tempState;
            gd.player.state_->enter(gd.player, gd, res);*/
            //Call enter on jumping state
        }
        gd.player.gravityScale = 3.0f;
    }
}

void sharedUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
    gd.player.currentDirection = 0;
    if (gd.player.curAnimation != -1) {
        gd.player.animations[gd.player.curAnimation].step(deltaTime);
    }
    if (!gd.player.grounded) {
        gd.player.vel.y += changeVel(700 * gd.player.gravityScale * deltaTime, gd.player); // gravity
    }
    if (state.keys[SDL_SCANCODE_A]) {
        gd.player.currentDirection += -1.f;
        //printf("set curDirection -1\n");
    }
    if (state.keys[SDL_SCANCODE_D]) {
        gd.player.currentDirection += 1.f;
        //printf("set curDirection 1\n");
        //printf("currentDirection in sharedUpdate while pressing D: %f\n", gd.player.currentDirection);
    }
    //printf("currentDirection in sharedUpdate: %f\n", gd.player.currentDirection);

    gd.player.cooldownTimer.step(deltaTime);
}
//Use tempPlayer->nextStateVal to return the new state of the player
PlayerState * changePlayerState(PlayerState * tempPlayer){
    PlayerState * newPlayer;
    switch(tempPlayer->nextStateVal){
        case IDLE:
        {
            newPlayer = new IdleState();
            newPlayer->enter = enterIdle;
            newPlayer->update = updateIdle;
            newPlayer->handleInput = handleInputIdle;
            newPlayer->currStateVal = IDLE;
            break;
        }
        case WALK:
        {
            newPlayer = new WalkState();
            newPlayer->enter = enterWalk;
            newPlayer->update = updateWalk;
            newPlayer->handleInput = handleInputWalk;
            newPlayer->currStateVal = WALK;
            break;
        }
        case RUN:
        {
            newPlayer = new RunState();
            newPlayer->enter = enterRun;
            newPlayer->update = updateWalk;
            newPlayer->handleInput = handleInputRun;
            newPlayer->currStateVal = RUN;
            break;
        }
        case SPRINT:
        {
            newPlayer = new SprintState();
            newPlayer->enter = enterSprint;
            newPlayer->update = updateSprint;
            newPlayer->handleInput = handleInputSprint;
            newPlayer->currStateVal = SPRINT;
            break;
        }
        case LAUNCH:
        {
            newPlayer = new JumpLaunchState();
            newPlayer->enter = enterLaunch;
            newPlayer->update = updateLaunch;
            newPlayer->handleInput = handleInputLaunch;
            newPlayer->currStateVal = LAUNCH;
            break;
        }
        case JUMP:
        {
            newPlayer = new JumpState();
            newPlayer->enter = enterJump;
            newPlayer->update = updateJump;
            newPlayer->handleInput = handleInputJump;
            newPlayer->currStateVal = JUMP;
            break;
        }
        case ROLL:
        {
            newPlayer = new RollState();
            newPlayer->enter = enterRoll;
            newPlayer->update = updateRoll;
            newPlayer->handleInput = handleInputRoll;
            newPlayer->currStateVal = ROLL;
            break;
        }
        case FALL:
        {
            newPlayer = new FallState();
            newPlayer->enter = enterFall;
            newPlayer->update = dummyUpdate;
            newPlayer->handleInput = dummyInput;
            newPlayer->currStateVal = FALL;
            break;
        }
        case DEAD:
        {
            newPlayer = new DeadState();
            newPlayer->enter = enterDead;
            newPlayer->update = dummyUpdate;
            newPlayer->handleInput = dummyInput;
            newPlayer->currStateVal = DEAD;
            break;
        }
        case SWORD_DEPLOY:
        {
            newPlayer = new ShotgunDeployState();
            newPlayer->enter = enterSwordDeploy;
            newPlayer->update = updateSwordDeploy;
            newPlayer->handleInput = handleInputSwordDeploy;
            newPlayer->currStateVal = SWORD_DEPLOY;
            break;
        }
        case SHOTGUN_DEPLOY:
        {
            newPlayer = new ShotgunDeployState();
            newPlayer->enter = enterShotgunDeploy;
            newPlayer->update = updateShotgunDeploy;
            newPlayer->handleInput = handleInputShotgunDeploy;
            newPlayer->currStateVal = SHOTGUN_DEPLOY;
            break;
        }
        case JETPACK_DEPLOY:
        {
            newPlayer = new JetpackDeployState();
            newPlayer->enter = enterJetpackDeploy;
            newPlayer->update = updateJetpackDeploy;
            newPlayer->handleInput = handleInputJetpackDeploy;
            newPlayer->currStateVal = JETPACK_DEPLOY;
            break;
        }
    }

    //newPlayer->currStateVal = tempPlayer->nextStateVal;
    return newPlayer;
}