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

std::string getStateFromEnum(PlayerStateValue ps) {
    switch (ps) {
        case NONE:             return "NONE";
        case IDLE:             return "IDLE";
        case WALK:             return "WALK";
        case RUN:              return "RUN";
        case SPRINT:           return "SPRINT";
        case SLIDE:            return "SLIDE";
        case FASTFALL:         return "FASTFALL";
        case LAUNCH:           return "LAUNCH";
        case JUMP:             return "JUMP";
        case ROLL:             return "ROLL";
        case FALL:             return "FALL";
        case DEAD:             return "DEAD";
        case SWORD_DEPLOY:     return "SWORD_DEPLOY";
        case SHOTGUN_DEPLOY:   return "SHOTGUN_DEPLOY";
        case JETPACK_DEPLOY:   return "JETPACK_DEPLOY";
        case GRAPPLE:          return "GRAPPLE";
        default:               return "ERROR";
    }        
}

//Handlers
PlayerState* handleJumping(GameData &gd, Resources &res, SDL_KeyboardEvent key) {
    // if (key.scancode == SDL_SCANCODE_SPACE && key.down && !key.repeat) {
    //     //printf("Jump handled");
    //     //printf("%d", gd.player.grounded);
    //     if (gd.player.grounded) { // single jump
    //         return new LaunchState();

    //         gd.player.vel.y = changeVel(JUMP_FORCE, gd.player); 
    //     } else if (gd.player.canDoubleJump) { // double jump
    //         return new JumpState();
            
    //         // gd.player.vel.y = changeVel(JUMP_FORCE, gd.player);  
    //         // gd.player.canDoubleJump = false;
    //         // gd.player.gravityScale = 1.0f; // reset gravity
    //     }
    // } else if (!key.down && key.scancode == SDL_SCANCODE_SPACE) { // letting go of jump
    //     float termVel = -200.0f; // option 2: Set velocity to predefined amount when you let go. makes less sharp jumps
    //     float shouldFlip = gd.player.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
    //     if (shouldFlip * gd.player.vel.y < shouldFlip * termVel) { 
    //         gd.player.vel.y = changeVel(termVel, gd.player);
    //     }
    // }
    return nullptr;
}

PlayerState* handleRunning(GameData &gd, Resources &res, SDL_KeyboardEvent key) {
    // if (key.scancode == SDL_SCANCODE_LSHIFT) {
    //     //printf("running handled");
    //         if (key.down && gd.player.currentDirection) { // if held down, increase speed               
    //             gd.player.state_ = changePlayerState(gd, res, gd.player.state_, RUN);
                
    //         } else {
    //             gd.player.state_ = changePlayerState(gd, res, gd.player.state_, WALK);                
    //         }
    //     }
    return nullptr;
}

PlayerState* handleSprinting(GameData &gd, Resources &res, SDL_KeyboardEvent key) {
    // if (key.scancode == SDL_SCANCODE_LSHIFT && !key.down) {
    //     //printf("Sprinting handled");
    //     gd.player.state_ = changePlayerState(gd, res, gd.player.state_, WALK);
        
    // }
    return nullptr;
}

PlayerState* handleFalling(GameData &gd, Resources &res, SDL_KeyboardEvent key) {
    // if (key.scancode == SDL_SCANCODE_S && key.down && !gd.player.grounded) { // fastfall
    //     //printf("Falling handled");
    //     if (!key.repeat && !gd.player.fastFalling) {
    //         gd.player.vel.y = changeVel(-250.0f, gd.player);
    //         gd.player.fastFalling = true;

    //         //Enter Jumping State
    //         gd.player.state_ = changePlayerState(gd, res, gd.player.state_, JUMP);
    //         //Call enter on jumping state
    //     }
    //     gd.player.gravityScale = 3.0f;
    // }
    return nullptr;
}

void sharedMovement(const SDLState &state, Player &p) { // basic movement function
    p.currentDirection = 0;
    if (state.keys[SDL_SCANCODE_A]) {
        p.currentDirection += -1.f;
    }
    if (state.keys[SDL_SCANCODE_D]) { // don't set to else cuz the two keys cancel e/o out
        p.currentDirection += 1.f;
    }
}

void sharedUpdate(const SDLState &state, Player &p, float deltaTime) { // basic animation and cooldown function, should always be called pretty much
    if (p.curAnimation != -1) {
        p.animations[p.curAnimation].step(deltaTime);
    }
    p.cooldownTimer.step(deltaTime);

    if (p.currentDirection) { // update direction based on currentDirection
        p.dir = p.currentDirection;
    }

    if (std::abs(p.vel.x) > p.maxSpeedX) {
        if (!isSliding(p)) { // if not sliding slow down
            p.vel.x -= 1.5 * p.acc.x * deltaTime * p.currentDirection;
        }
    }

    if(p.usingSugar) {
        ((Sugar *) &p.item)->sugarTimer.step(deltaTime);
        p.vel.x += .5f * p.currentDirection;
        if(((Sugar *) &p.item)->sugarTimer.isTimeOut()){
            //printf("Stopped sugar\n");
            p.usingSugar = false;
        }
    }

    // add vel to pos 
    p.pos += updatePos(p, deltaTime);
}

void sharedGravity(Player &p, float deltaTime) { // call on airborne states
    p.vel.y += changeVel(700 * p.gravityScale * deltaTime, p); // gravity
}

// IDLE
PlayerState* IdleState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    //handleJumping(gd,res,key);
    printf("Idle Input\n");
    sharedMovement(state, p);
    if (p.currentDirection) { // if moving change to walking

        return new WalkState(); 
    }
    return nullptr;
}

PlayerState* IdleState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    //gd.player.vel.x = 0;    
    sharedUpdate(state, p, deltaTime);
    return nullptr;
}

void IdleState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texIdle[p.character];
    p.curAnimation = res.ANIM_PLAYER_IDLE; 
    p.animations[p.curAnimation].reset();
    p.vel.x = 0;
}

// WALK
PlayerState* WalkState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);

    return nullptr;
}

PlayerState* WalkState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    //gd.player.vel.x = 0;    
    if (!p.currentDirection && p.grounded) { // if not moving, slow down
        const float factor = p.vel.x > 0 ? -1.0f : 1.0f;
        float amount = factor * p.acc.x * deltaTime;
        
        if (std::abs(p.vel.x) < std::abs(amount)) {
            p.vel.x = 0;
            // once stopped, set player to idle
            return new IdleState();          
        }
        else {
            p.vel.x += amount;
        }
    }
    if (isSliding(p) && p.grounded) { // moving in different direction of vel and pressing a direction, sliding
        return new SlideState();
    }
    if (state.keys[SDL_SCANCODE_LSHIFT] && p.currentDirection) {
        return new RunState();
    }
    return nullptr;
}

void WalkState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRun[p.character];
    p.curAnimation = res.ANIM_PLAYER_WALK; 
    p.animations[p.curAnimation].reset();
    p.maxSpeedX = p.maxWalkX;
    p.sprintTimer.reset();
}

void WalkState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// RUN
PlayerState* RunState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key){
    handleJumping(gd,res,key);
    handleRunning(gd,res,key);
    handleFalling(gd,res,key);
    return nullptr;
}

PlayerState* RunState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {   
    sharedUpdate(state, gd,res,deltaTime);
    if (!state.keys[SDL_SCANCODE_LSHIFT] || !p.currentDirection) { // if not pressing then reset
        return new WalkState();
    }

    float LEEWAY = 20;
    if (p.grounded && std::abs(p.vel.x) >= (p.maxRunX - LEEWAY)) { // if grounded and moving fast enter sprint (eventually)                
        if (!p.sprintTimer.isTimeOut()) {
            p.sprintTimer.step(deltaTime);
        } else {
            return new SprintState();
        }
    } 
    if (isSliding(p) && p.grounded) { // moving in different direction of vel and pressing a direction, sliding
        return new SlideState();
    }
}

void RunState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRun[p.character];
    p.curAnimation = res.ANIM_PLAYER_RUN; 
    p.maxSpeedX = p.maxRunX;
    p.sprintTimer.reset();
}

void RunState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// JUMP 
PlayerState* JumpState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key){
    handleRunning(gd,res,key);
    handleJumping(gd,res,key);
    handleFalling(gd,res,key);
    return nullptr;
}

PlayerState* JumpState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {   
    sharedUpdate(state, gd,res,deltaTime);
    if(p.grounded && p.dir == 0) {
        return new IdleState();
    } else if(p.grounded) {
        return new WalkState();
    }
    return nullptr;
}

void JumpState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texJump[p.character];
    p.curAnimation = res.ANIM_PLAYER_JUMP; 
    p.animations[p.curAnimation].reset();
}


// LAUNCH
PlayerState* LaunchState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key){
    handleRunning(gd,res,key);
    handleJumping(gd,res,key);
    handleFalling(gd,res,key);
    return nullptr;
}

// ROLL
PlayerState* RollState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key){
    handleRunning(gd,res,key);
    handleJumping(gd,res,key);
    handleFalling(gd,res,key);
    return nullptr;
}

// SPRINT
PlayerState* SprintState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key){
    handleSprinting(gd,res,key);
    handleJumping(gd,res,key);
    handleFalling(gd,res,key);
    return nullptr;
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

// GRAPPLE
PlayerState* GrappleState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    glm::vec2 pOffset = findCenterOfSprite(p); 
    glm::vec2 hOffset = findCenterOfSprite(p.hook);
    // this will go in helper function later
    float xDist = (p.hook.pos.x - gd.mapViewport.x + hOffset.x) - (p.pos.x - gd.mapViewport.x + pOffset.x); // A
    float yDist = (p.hook.pos.y - gd.mapViewport.y + hOffset.y) - (p.pos.y - gd.mapViewport.y + pOffset.y); // O
    float dist = std::sqrt(xDist * xDist + yDist * yDist); // distance formula, H
    float aH = xDist / dist; // cos
    float oH = yDist / dist; // sin
    p.vel = 500.0f * glm::vec2(aH, oH);
    return nullptr;
}

void GrappleState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texJump[p.character];
    p.curAnimation = res.ANIM_PLAYER_JUMP; 
    p.animations[p.curAnimation].reset();
    p.canDoubleJump = true;
}

void GrappleState::exit(GameData &gd, Resources &res, Player &p) {
    p.hook.collided = false;
    p.hook.visible = false;
    p.hook.pos += glm::vec2(-10000.0f, -10000.0f); // maybe unnecessary
}

// void updateSprint(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
//     sharedUpdate(state, gd,res,deltaTime);
//     float LEEWAY = 20;
//     if (gd.player.grounded && // if on ground and sliding or too slow reset sprint
//         (isSliding(gd.player) || std::abs(gd.player.vel.x) < (gd.player.maxRunX - LEEWAY))) {       
//         gd.player.sprintTimer.reset();
//         gd.player.state_ = changePlayerState(gd, res, gd.player.state_, RUN);
//     }
// }
        
    
// }

// void updateFalling(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
//     sharedUpdate(state, gd,res,deltaTime);

//     // gd.player.state_->nextStateVal = FALL;
//     // gd.player.state_ = changePlayerState(gd, res, gd.player.state_);
//     // 
// }

// void handleInputSlide(GameData &gd, Resources &res, SDL_KeyboardEvent key) {;
//     handleRunning(gd,res,key);
//     handleJumping(gd,res,key);
// }
// void updateSlide(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
//     sharedUpdate(state, gd,res,deltaTime);
//     if (gd.player.currentDirection == 0) { // this entire function is awful and making me sad
//         gd.player.currentDirection = gd.player.dir;
//     } 
//     if (!isSliding(gd.player)) {
//         gd.player.state_ = changePlayerState(gd, res, gd.player.state_, RUN);
//     }
// }
// void enterSlide(Player& player, GameData &gd, Resources &res) {
//     if(((LevelState * )(currState))->character == SWORD){
//         player.texture = res.texSlideS;
//     } else if(((LevelState * )(currState))->character == SHOTGUN){
//         player.texture = res.texSlideG;
//     } else {
//         player.texture = res.texSlideJ;
//     }
//     player.curAnimation = res.ANIM_PLAYER_SLIDE; 
//     player.animations[gd.player.curAnimation].reset();
//     gd.player.sprintTimer.reset();
// }

// void updateLaunch(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
//     sharedUpdate(state, gd,res,deltaTime);
        
//     // gd.player.state_->nextStateVal = LAUNCH;
//     // gd.player.state_ = changePlayerState(gd, res, gd.player.state_); 

//     if (gd.player.animations[gd.player.curAnimation].isDone()) {
//         gd.player.state_ = changePlayerState(gd, res, gd.player.state_, JUMP);
//         gd.player.animations[gd.player.curAnimation].reset();
//     }
// }

// void updateRoll(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
//     sharedUpdate(state, gd,res,deltaTime);
//     // gd.player.state_->nextStateVal = ROLL;
//     // gd.player.state_ = changePlayerState(gd, res, gd.player.state_);
//     // 

//     // when roll animation finishes, switch to moving
//     if (gd.player.animations[gd.player.curAnimation].isDone()) {
//         gd.player.state_ = changePlayerState(gd, res, gd.player.state_, RUN);
        
//         gd.player.animations[gd.player.curAnimation].reset();
//     }
// }

// void dummyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
//     sharedUpdate(state, gd,res,deltaTime);
//     if(gd.player.grounded) { // if moving change to running
//         gd.player.state_ = changePlayerState(gd, res, gd.player.state_, ROLL);
        
//     }
// }

// void updateJetpackDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
//     sharedUpdate(state, gd,res,deltaTime);
//     gd.player.vel.y -= 3.f;

//             if (!gd.player.jetpackTimer.isTimeOut()) {
//                 gd.player.jetpackTimer.step(deltaTime);
//             } else {
//                 gd.player.cooldownTimer.reset();
//                 gd.player.state_ = changePlayerState(gd, res, gd.player.state_, IDLE);
//             }
//     int vertDir = 0;
//     //calculate direction
//      if (state.keys[SDL_SCANCODE_W]) {
//         vertDir += -1.f;
//     }
//     if (state.keys[SDL_SCANCODE_S]) {
//         vertDir += 1.f;
//     }
//     if(gd.player.currentDirection == 1 && vertDir == 0) {
//         gd.player.vel.x += 5.f;
//     } else if (gd.player.currentDirection == -1 && vertDir == 0) {
//         gd.player.vel.x -= 5.f;
//     } else if (gd.player.currentDirection == 0 && vertDir == 1) {
//         gd.player.vel.y += 5.f;
//     } else if (gd.player.currentDirection == 0 && vertDir == -1) {
//         gd.player.vel.y -= 5.f;
//     } else if (gd.player.currentDirection == 1 && vertDir == 1) {
//         gd.player.vel.x += (5/sqrt(2));
//         gd.player.vel.y += (5/sqrt(2));
//     } else if (gd.player.currentDirection == 1 && vertDir == -1) {
//         gd.player.vel.x += (5/sqrt(2));
//         gd.player.vel.y -= (5/sqrt(2));
//     } else if (gd.player.currentDirection == -1 && vertDir == 1) {
//         gd.player.vel.x -= (5/sqrt(2));
//         gd.player.vel.y += (5/sqrt(2));
//     } else if (gd.player.currentDirection == -1 && vertDir == -1) {
//         gd.player.vel.x -= (5/sqrt(2));
//         gd.player.vel.y -= (5/sqrt(2));
//     }

//     //Check timer for state change
//     if (!gd.player.jetpackTimer.isTimeOut()) {
//         gd.player.jetpackTimer.step(deltaTime);
//     } else {
//         gd.player.cooldownTimer.reset();
//         if(gd.player.vel.x != 0) {
//             gd.player.state_ = changePlayerState(gd, res, gd.player.state_, WALK);
//         } else {
//             gd.player.state_ = changePlayerState(gd, res, gd.player.state_, IDLE);
//         }
//     }
// }

// void updateShotgunDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
//     sharedUpdate(state, gd, res, deltaTime);

//     if(gd.player.blast->animations[gd.player.blast->curAnimation].isDone()) { 
//         delete gd.player.blast;
//         gd.player.blast = nullptr;
//         gd.player.cooldownTimer.reset();
//         gd.player.state_ = changePlayerState(gd, res, gd.player.state_, IDLE);
//         if(gd.player.vel.x != 0) {
//             gd.player.state_ = changePlayerState(gd, res, gd.player.state_, WALK);
//         } else {
//             gd.player.state_ = changePlayerState(gd, res, gd.player.state_, IDLE);
//         }
//     } else {
//         if(gd.player.dir == 1) {
//             gd.player.blast->pos = glm::vec2(gd.player.pos.x + 32, gd.player.pos.y - 4);
//             gd.player.blast->dir = 1;
//         } else if (gd.player.dir == -1) {
//             gd.player.blast->pos = glm::vec2(gd.player.pos.x - 80, gd.player.pos.y - 4);
//             gd.player.blast->dir = -1;
//         }
//     }
// }

// void updateSwordDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
//     sharedUpdate(state, gd, res, deltaTime);

//      if(gd.player.animations[gd.player.curAnimation].isDone()) { 
//         gd.player.cooldownTimer.reset();
//         if(gd.player.vel.x != 0) {
//             gd.player.state_ = changePlayerState(gd, res, gd.player.state_, WALK);
//         } else {
//             gd.player.state_ = changePlayerState(gd, res, gd.player.state_, IDLE);
//         }
//     }
// }



// void enterWalk(Player& player, GameData &gd, Resources &res){
//     if(((LevelState * )(currState))->character == SWORD){
//         player.texture = res.texRunS;
//     } else if(((LevelState * )(currState))->character == SHOTGUN){
//         player.texture = res.texRunG;
//     } else {
//         player.texture = res.texRunJ;
//     }
//     player.curAnimation = res.ANIM_PLAYER_WALK; 
//     player.animations[gd.player.curAnimation].reset();
//     gd.player.maxSpeedX = gd.player.maxWalkX;
//     gd.player.sprintTimer.reset();
// }

// void enterSprint(Player& player, GameData &gd, Resources &res){
//     if(((LevelState * )(currState))->character == SWORD){
//         player.texture = res.texRunS;
//     } else if(((LevelState * )(currState))->character == SHOTGUN){
//         player.texture = res.texRunG;
//     } else {
//         player.texture = res.texRunJ;
//     }
//     //Change?
//     player.curAnimation = res.ANIM_PLAYER_RUN; 
//     player.animations[gd.player.curAnimation].reset();
//     gd.player.maxSpeedX = gd.player.maxSprintX;
// }

// void enterLaunch(Player& player, GameData &gd, Resources &res){
    
//     if(((LevelState * )(currState))->character == SWORD){
//         player.texture = res.texLaunchS;
//     } else if(((LevelState * )(currState))->character == SHOTGUN){
//         player.texture = res.texLaunchG;
//     } else {
//         player.texture = res.texLaunchJ;
//     }
//     player.curAnimation = res.ANIM_PLAYER_LAUNCH; 
//     player.animations[gd.player.curAnimation].reset();
// }



// void enterRoll(Player& player, GameData &gd, Resources &res){
//     if(((LevelState * )(currState))->character == SWORD){
//         player.texture = res.texRollS;
//     } else if(((LevelState * )(currState))->character == SHOTGUN){
//         player.texture = res.texRollG;
//     } else {
//         player.texture = res.texRollJ;
//     }
//     player.curAnimation = res.ANIM_PLAYER_ROLL; 
//     player.animations[gd.player.curAnimation].reset();
// }

// void enterFall(Player& player, GameData &gd, Resources &res){
//     /*if(((LevelState * )(currState))->character == SWORD){
//         player.texture = res.texShot;
//     } else if(((LevelState * )(currState))->character == SHOTGUN){
//         player.texture = res.texLaunchG;
//     } else {
//         player.texture = res.texLaunchJ;
//     }
//     player.curAnimation = res.ANIM_PLAYER_LAUNCH; 
//     player.animations[gd.player.curAnimation].reset();*/
// }

// void enterDead(Player& player, GameData &gd, Resources &res){
//     if(((LevelState * )(currState))->character == SWORD){
//         player.texture = res.texDieS;
//     } else if(((LevelState * )(currState))->character == SHOTGUN){
//         player.texture = res.texDieG;
//     } else {
//         player.texture = res.texDieJ;
//     }
//     player.curAnimation = res.ANIM_PLAYER_DIE; 
//     player.animations[gd.player.curAnimation].reset();
// }

// void enterJetpackDeploy(Player& player, GameData &gd, Resources &res) {

//     player.texture = res.texJDeploy;
//     player.curAnimation = res.ANIM_PLAYER_JETPACK_DEPLOY;
//     player.animations[gd.player.curAnimation].reset();

//     gd.player.jetpackTimer.reset();
// }

// void enterShotgunDeploy(Player& player, GameData &gd, Resources &res) {

//     player.texture = res.texGDeploy;
//     player.curAnimation = res.ANIM_PLAYER_SHOOT;
//     player.animations[gd.player.curAnimation].reset();
//     //load in shotgun
//     SDL_FRect collider = { 
//                             .x = 0,
//                             .y = 12,
//                             .w = 80,
//                             .h = 24
//                         };
//     glm::vec2 pos;
//     AnimatedObject* blast = new AnimatedObject(pos, collider, res.texShotgunBlast);
//     if(player.dir == 1) {
//         blast->pos = glm::vec2(player.pos.x + 32, player.pos.y - 4);
//         blast->dir = 1;
//     } else if (player.dir == -1) {
//         blast->pos = glm::vec2(player.pos.x - 80, player.pos.y - 4);
//         blast->dir = -1;
//     }
//     blast->animations = res.shotgunAnims;
//     blast->curAnimation = res.SHOTGUN_BLAST;
//     gd.player.blast = blast;
// }

// void enterSwordDeploy(Player& player, GameData &gd, Resources &res) {
//     player.texture = res.texSDeploy;
//     player.curAnimation = res.ANIM_PLAYER_SWORD_DEPLOY;
//     player.animations[gd.player.curAnimation].reset();
// }

// void dummyEnter(Player& player, GameData &gd, Resources &res) {
    
// }

// void emptyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime){

// }
