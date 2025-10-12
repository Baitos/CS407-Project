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
        case STUNNED:             return "STUNNED";
        case DEAD:             return "DEAD";
        case SWORD_DEPLOY:     return "SWORD_DEPLOY";
        case SHOTGUN_DEPLOY:   return "SHOTGUN_DEPLOY";
        case JETPACK_DEPLOY:   return "JETPACK_DEPLOY";
        case GRAPPLE:          return "GRAPPLE";
        default:               return "ERROR";
    }        
}

//Handlers
PlayerState* handleJumping(GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    if (key.scancode == SDL_SCANCODE_SPACE && key.down && !key.repeat) {
        //printf("Jump handled");
        //printf("%d", p.grounded);
        if (p.grounded) { // single jump
            return new LaunchState();
        }
    //     } else if (p.canDoubleJump) { // double jump
    //         return new JumpState();
            
    //         // p.vel.y = changeVel(JUMP_FORCE, p);  
    //         // p.canDoubleJump = false;
    //         // p.gravityScale = 1.0f; // reset gravity
    //     }
    // } else if (!key.down && key.scancode == SDL_SCANCODE_SPACE) { // letting go of jump
    //     float termVel = -200.0f; // option 2: Set velocity to predefined amount when you let go. makes less sharp jumps
    //     float shouldFlip = p.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
    //     if (shouldFlip * p.vel.y < shouldFlip * termVel) { 
    //         p.vel.y = changeVel(termVel, p);
    //     }
    }
    
    return nullptr;
}

PlayerState* handleRunning(GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    /*if (key.scancode == SDL_SCANCODE_LSHIFT) {
        //printf("running handled");
            if (key.down && p.currentDirection) { // if held down, increase speed               
                return new RunState(); 
            } else {
                return new WalkState();               
            }
        }*/
    return nullptr;
}

PlayerState* handleSprinting(GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    /*if (key.scancode == SDL_SCANCODE_LSHIFT && !key.down) {
        //printf("Sprinting handled");
        return new WalkState();
    }*/
    return nullptr;
}

PlayerState* handleFalling(GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    /*if (key.scancode == SDL_SCANCODE_S && key.down && !p.grounded) { // fastfall
        //printf("Falling handled");
        if (!key.repeat && !p.fastFalling) {
            p.vel.y = changeVel(-250.0f, p);
            p.fastFalling = true;

            //Enter Jumping State
            return new JumpState();
            //Call enter on jumping state
        }
        p.gravityScale = 3.0f;
    }*/
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
    if (p.currentDirection) {
        p.dir = p.currentDirection;
    }
}

void sharedGravity(Player &p, float deltaTime) { // call on airborne states
    p.vel.y += changeVel(700 * p.gravityScale * deltaTime, p); // gravity
}

void sharedUpdate(const SDLState &state, Player &p, float deltaTime) { // basic animation and cooldown function, should always be called pretty much
    if (p.curAnimation != -1) {
        p.animations[p.curAnimation].step(deltaTime);
    }
    p.cooldownTimer.step(deltaTime);

    if (p.currentDirection) { // update direction based on currentDirection
        p.dir = p.currentDirection;
    }

    if(p.usingSugar) {
        ((Sugar *) &p.item)->sugarTimer.step(deltaTime);
        p.vel.x += .5f * p.currentDirection;
        if(((Sugar *) &p.item)->sugarTimer.isTimeOut()){
            //printf("Stopped sugar\n");
            p.usingSugar = false;
        }
    }
    if (!p.grounded) { 
        sharedGravity(p, deltaTime);
    }
    

    // add vel to pos 
    p.pos += updatePos(p, deltaTime);
}

// IDLE
PlayerState* IdleState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    
    sharedMovement(state, p);
    if (p.currentDirection) { // if moving change to walking
        return new WalkState(); 
    }
    PlayerState* ret = handleJumping(gd, res, p, key);
    if (ret != nullptr) {
        return ret;
    }

    return nullptr;
}

PlayerState* IdleState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    //p.vel.x = 0;    
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
    PlayerState* ret = handleJumping(gd, res, p, key);
    if (ret != nullptr) {
        return ret;
    }
    ret = handleRunning(gd, res, p, key);
    if (ret != nullptr) {
        return ret;
    }
    return nullptr;
}

PlayerState* WalkState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {   
    sharedUpdate(state, p, deltaTime);
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
PlayerState* RunState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    handleRunning(gd, res, p, key);
    handleFalling(gd, res, p, key);
    return nullptr;
}

PlayerState* RunState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {   
    sharedUpdate(state, p, deltaTime);
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
    return nullptr;
}

void RunState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRun[p.character];
    p.curAnimation = res.ANIM_PLAYER_RUN; 
    p.maxSpeedX = p.maxRunX;
    p.sprintTimer.reset();
}

void RunState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// JUMP 
PlayerState* JumpState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    handleRunning(gd, res, p, key);
    handleFalling(gd, res, p, key);
    return nullptr;
}

PlayerState* JumpState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {   
    sharedUpdate(state, p, deltaTime);
    if (p.grounded && p.dir == 0) {
        return new IdleState();
    } else if (p.grounded) {
        return new WalkState();
    }
    return nullptr;
}

void JumpState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texJump[p.character];
    p.curAnimation = res.ANIM_PLAYER_JUMP; 
    p.animations[p.curAnimation].reset();
}

void JumpState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// LAUNCH
PlayerState* LaunchState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    handleRunning(gd, res, p, key);
    handleFalling(gd, res, p, key);
    return nullptr;
}

PlayerState* LaunchState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    if (p.animations[p.curAnimation].isDone()) {
        return new JumpState();
    }
    return nullptr;
}

void LaunchState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texLaunch[p.character];
    p.curAnimation = res.ANIM_PLAYER_LAUNCH; 
    p.animations[p.curAnimation].reset();
    p.vel.y = changeVel(JUMP_FORCE, p); // launch! 
}

void LaunchState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// ROLL
PlayerState* RollState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    handleRunning(gd, res, p, key);
    handleFalling(gd, res, p, key);
    return nullptr;
}

PlayerState* RollState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    return nullptr;
}

void RollState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRoll[p.character];
    p.curAnimation = res.ANIM_PLAYER_ROLL; 
    p.animations[p.curAnimation].reset();
}

void RollState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// SPRINT
PlayerState* SprintState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    handleSprinting(gd, res, p, key);
    handleFalling(gd, res, p, key);
    return nullptr;
}

PlayerState* SprintState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    return nullptr;
}

void SprintState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRun[p.character];
    p.curAnimation = res.ANIM_PLAYER_RUN; 
    p.animations[p.curAnimation].reset();
    p.maxSpeedX = p.maxSprintX;
}

void SprintState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// SLIDE
PlayerState* SlideState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    handleRunning(gd, res, p, key);
    return nullptr;
}

PlayerState* SlideState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    if (p.currentDirection == 0) { // this entire function is awful and making me sad
        p.currentDirection = p.dir;
    } 
    if (!isSliding(p)) {
        return new RunState();
    }
    return nullptr;
}

void SlideState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texSlide[p.character];
    p.curAnimation = res.ANIM_PLAYER_SLIDE; 
    p.animations[p.curAnimation].reset();
    p.sprintTimer.reset();
}

void SlideState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// FASTFALL
PlayerState* FastfallState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    return new JumpState();
}

PlayerState* FastfallState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    return nullptr;
}

void FastfallState::enter(GameData &gd, Resources &res, Player &p) { // todo

}

void FastfallState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// STUNNED
PlayerState* StunnedState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    return nullptr;
}

PlayerState* StunnedState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    return nullptr;
}

void StunnedState::enter(GameData &gd, Resources &res, Player &p) { // todo

}

void StunnedState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// DEAD
void DeadState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texDie[p.character];
    p.curAnimation = res.ANIM_PLAYER_DIE; 
    p.animations[p.curAnimation].reset();
}

// SWORD DEPLOY
PlayerState* SwordDeployState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    return nullptr;
}

PlayerState* SwordDeployState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    return nullptr;
}

void SwordDeployState::enter(GameData &gd, Resources &res, Player &p) { // todo

}

void SwordDeployState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// SHOTGUN DEPLOY
PlayerState* ShotgunDeployState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    return nullptr;
}

PlayerState* ShotgunDeployState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    return nullptr;
}

void ShotgunDeployState::enter(GameData &gd, Resources &res, Player &p) { // todo

}

void ShotgunDeployState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing

// JETPACK DEPLOY
PlayerState* JetpackDeployState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) {
    sharedMovement(state, p);
    handleJumping(gd, res, p, key);
    return nullptr;
}

PlayerState* JetpackDeployState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    return nullptr;
}

void JetpackDeployState::enter(GameData &gd, Resources &res, Player &p) { // todo

}

void JetpackDeployState::exit(GameData &gd, Resources &res, Player &p) {} // do nothing


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
   
    
// }

// void updateFalling(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
//     sharedUpdate(state, gd,res,deltaTime);

//     // p.state_->nextStateVal = FALL;
//     // p.state_ = changePlayerState(gd, res, p.state_);
//     // 
// }

// void handleInputSlide(GameData &gd, Resources &res, SDL_KeyboardEvent key) {;
//     handleRunning(gd,res,key);
//     handleJumping(gd,res,key);
// }
// void updateSlide(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
//     sharedUpdate(state, gd,res,deltaTime);
//     if (p.currentDirection == 0) { // this entire function is awful and making me sad
//         p.currentDirection = p.dir;
//     } 
//     if (!isSliding(p)) {
//         p.state_ = changePlayerState(gd, res, p.state_, RUN);
//     }
// }
// void updateLaunch(const SDLState &state, GameData &gd, Resources &res, float deltaTime){
//     sharedUpdate(state, gd,res,deltaTime);
        
//     // p.state_->nextStateVal = LAUNCH;
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
