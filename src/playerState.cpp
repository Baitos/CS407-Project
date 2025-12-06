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
        case NOSTATE: return "NOSTATE";
        case IDLE: return "IDLE";
        case WALK: return "WALK";
        case RUN: return "RUN";
        case SPRINT: return "SPRINT";
        case SLIDE: return "SLIDE";
        case FASTFALL: return "FASTFALL";
        case LAUNCH: return "LAUNCH";
        case JUMP: return "JUMP";
        case ROLL: return "ROLL";
        case STUNNED: return "STUNNED";
        case DEAD: return "DEAD";
        case SWORD_DEPLOY: return "SWORD_DEPLOY";
        case SHOTGUN_DEPLOY: return "SHOTGUN_DEPLOY";
        case JETPACK_DEPLOY: return "JETPACK_DEPLOY";
        case GRAPPLE: return "GRAPPLE";
        default: return "ERROR";
    }        
}

//HandlersACTION_JUMP
PlayerState* handleJumping(GameData &gd, Resources &res, Player &p, SDL_Event event) {
    SDL_KeyboardEvent key = event.key;
    if (gd.controls->actionPerformed(ACTION_JUMP, event) && key.down && !key.repeat) {
        if (p.grounded) { // single jump
            return new LaunchState();
        } else if (p.canDoubleJump) { // double jump
            p.canDoubleJump = false;
            p.vel.y = changeVel(JUMP_FORCE, p); // this could be put into a double jump state 
            return new JumpState();
        }
    } else if (!key.down && gd.controls->actionPerformed(typeAction::ACTION_JUMP, event)) { // letting go of jump
        float termVel = -200.0f; // option 2: Set velocity to predefined amount when you let go. makes less sharp jumps
        float shouldFlip = p.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
        if (shouldFlip * p.vel.y < shouldFlip * termVel) { 
            p.vel.y = changeVel(termVel, p);
        }
    }
    return nullptr;
}

PlayerState* handleSprinting(GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (!event.key.down && gd.controls->actionPerformed(ACTION_SPRINT, event)) {
        return new WalkState();
    }
    return nullptr;
}

PlayerState* handleFalling(GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (event.key.down && gd.controls->actionPerformed(ACTION_FASTFALL, event) && !p.grounded) { // fastfall
        return new FastfallState();
    }
    return nullptr;
}

/* so technically this is handleInput but BECAUSE handleInput doesn't technically get called every frame (only when a key is pressed)
    currentDirection does not get set to zero every frame UNLESS this is put in update. there is probably a way to fix this but 
    im just trying to get everything back in functioning condition before i make it even more optimized */
void sharedMovement(const SDLState &state, Player &p) { // basic movement function
    p.currentDirection = 0;
    if (state.keys[SDL_SCANCODE_A]) {
        p.currentDirection += -1.f;
    }
    if (state.keys[SDL_SCANCODE_D]) { // don't set to else cuz the two keys cancel e/o out
        p.currentDirection += 1.f;
    }
}

void sharedGravity(Player &p, float deltaTime) { // call on airborne states
    p.vel.y += changeVel(700 * p.gravityScale * deltaTime, p); // gravity
}

void sharedUpdate(const SDLState &state, Player &p, float deltaTime) { // basic animation and cooldown function, should always be called pretty much
    sharedMovement(state, p);
    if (p.curAnimation != -1) {
        p.animations[p.curAnimation].step(deltaTime);
    }
    p.cooldownTimer.step(deltaTime);

    if (p.currentDirection) { // update direction based on currentDirection
        p.dir = p.currentDirection;
    }
    if (!p.grounded) { 
        sharedGravity(p, deltaTime);
    }
}

// IDLE
PlayerState* IdleState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {   
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* IdleState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    sharedUpdate(state, p, deltaTime);
    if (p.currentDirection) { // if moving change to walking
        return new WalkState(); 
    }
    return nullptr;
}

void IdleState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texIdle[p.character];
    p.curAnimation = res.ANIM_PLAYER_IDLE; 
    p.animations[p.curAnimation].reset();
    p.vel.x = 0;
}

// WALK
PlayerState* WalkState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleFalling(gd, res, p, event)) return retState;
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* WalkState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {   
    sharedUpdate(state, p, deltaTime);
    if (state.keys[gd.controls->getActionKey(typeAction::ACTION_SPRINT)] && p.currentDirection) {
        return new RunState();
    }
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
    if (isSliding(p)) { // moving in different direction of vel and pressing a direction, sliding
        return new SlideState();
    }
    return nullptr;
}

void WalkState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRun[p.character];
    p.curAnimation = res.ANIM_PLAYER_WALK; 
    p.animations[p.curAnimation].reset();
    p.maxSpeedX = p.maxWalkX;
}

// RUN
PlayerState* RunState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    if (auto retState = handleFalling(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* RunState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {   
    sharedUpdate(state, p, deltaTime);
    if (!state.keys[gd.controls->getActionKey(typeAction::ACTION_SPRINT)] || !p.currentDirection) { // if not pressing then reset
        return new WalkState();
    }
    float LEEWAY = 25;
    if (p.grounded && std::abs(p.vel.x) >= (p.maxRunX - LEEWAY)) { // if grounded and moving fast enter sprint (eventually)                
        if (!this->sprintTimer.isTimeOut()) {
            this->sprintTimer.step(deltaTime);
        } else {
            return new SprintState();
        }
    } else {
        this->sprintTimer.reset();
    }
    if (isSliding(p)) { // moving in different direction of vel and pressing a direction, sliding
        return new SlideState();
    }
    return nullptr;
}

void RunState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRun[p.character];
    p.curAnimation = res.ANIM_PLAYER_RUN; 
    p.maxSpeedX = p.maxRunX;
    this->sprintTimer.reset();
}

// SPRINT
PlayerState* SprintState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    if (auto retState = handleSprinting(gd, res, p, event)) return retState;
    if (auto retState = handleFalling(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* SprintState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    sharedUpdate(state, p, deltaTime);
    float LEEWAY = 25;
    if (p.grounded && // if on ground and sliding or too slow reset sprint
        (isSliding(p) || std::abs(p.vel.x) < (p.maxRunX - LEEWAY) || !p.currentDirection)) {       
        return new WalkState();
    }
    return nullptr;
}

void SprintState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRun[p.character];
    p.curAnimation = res.ANIM_PLAYER_RUN; 
    p.animations[p.curAnimation].reset();
    p.maxSpeedX = p.maxSprintX;
}

// JUMP 
PlayerState* JumpState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    if (auto retState = handleFalling(gd, res, p, event)) return retState;
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

// LAUNCH
PlayerState* LaunchState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    if (auto retState = handleFalling(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* LaunchState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
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

// ROLL
PlayerState* RollState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    if (auto retState = handleFalling(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* RollState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    sharedUpdate(state, p, deltaTime);
    if (p.animations[p.curAnimation].isDone()) {
        return new RunState();
    }
    return nullptr;
}

void RollState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texRoll[p.character];
    p.curAnimation = res.ANIM_PLAYER_ROLL; 
    p.animations[p.curAnimation].reset();
}

// SLIDE
PlayerState* SlideState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* SlideState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    sharedUpdate(state, p, deltaTime);
    if (p.currentDirection == 0) { // prevents you from sliding forever when you let go
        p.currentDirection = p.dir;
    } 
    if (!isSliding(p)) {
        return new WalkState();
    }
    return nullptr;
}

void SlideState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texSlide[p.character];
    p.curAnimation = res.ANIM_PLAYER_SLIDE; 
    p.animations[p.curAnimation].reset();
}

// FASTFALL
PlayerState* FastfallState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* FastfallState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    sharedUpdate(state, p, deltaTime);
    if (p.grounded) {
        return new RollState();
    }
    return nullptr;
}

void FastfallState::enter(GameData &gd, Resources &res, Player &p) { // 
    p.vel.y = changeVel(-250.0f, p); // hop
    p.gravityScale = 3.0f;
    p.texture = res.texJump[p.character];
    p.curAnimation = res.ANIM_PLAYER_JUMP; 
    p.animations[p.curAnimation].reset();
}

void FastfallState::exit(GameData &gd, Resources &res, Player &p) {
    p.gravityScale = 1.0f;
} 

// STUNNED

PlayerState* StunnedState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    if (!this->hardStun) {
        sharedUpdate(state, p, deltaTime);
    } else { // if hardStunned, disable control
        p.cooldownTimer.step(deltaTime);
        p.currentDirection = 0;
        if (!p.grounded) { 
            sharedGravity(p, deltaTime);
        }
    }
    if (p.grounded) { // if moving change to running
        return new RollState();  
    }
    return nullptr;
}

void StunnedState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texDie[p.character];
    p.curAnimation = res.ANIM_PLAYER_DIE; 
    p.animations[p.curAnimation].reset();
    removeHook(p);
}

// DEAD
void DeadState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texDie[p.character];
    p.curAnimation = res.ANIM_PLAYER_DIE; 
    p.animations[p.curAnimation].reset();
    p.respawnTimer.reset();
    p.isDead = true;        
}

PlayerState* DeadState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    //step forward timer if player currently dead
    p.respawnTimer.step(deltaTime);
    p.vel = glm::vec2(0);
    //check if player can respawn
    if (p.respawnTimer.isTimeOut()) {
        p.pos.x = gd.checkpoints_[p.lastCheckpoint].collider.x;
        p.pos.y = gd.checkpoints_[p.lastCheckpoint].collider.y + gd.checkpoints_[p.lastCheckpoint].collider.h - TILE_SIZE*2;
        //set state to idle
        return new IdleState();
    }
    return nullptr;
}

// SWORD DEPLOY
PlayerState* SwordDeployState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* SwordDeployState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    sharedUpdate(state, p, deltaTime);
    if (p.animations[p.curAnimation].isDone()) { 
        p.cooldownTimer.reset();
        if(p.vel.x != 0) {
            return new WalkState();
        } else {
            return new IdleState();
        }
    }
    return nullptr;
}

void SwordDeployState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texDeploy[p.character];
    p.curAnimation = res.ANIM_PLAYER_SWORD_DEPLOY;
    p.animations[p.curAnimation].reset();
}

// SHOTGUN DEPLOY
void ShotgunDeployState::draw(const SDLState &state, GameData &gd) {
    //draw blast if needed for shotgun
    if(this->blast != nullptr) {
        this->blast->draw(state, gd, 80, 48);
    }  
}

PlayerState* ShotgunDeployState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* ShotgunDeployState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    sharedUpdate(state, p, deltaTime);
    
    if (this->blast != nullptr) { // update shotgun blast
        this->blast->update(state, gd, res, deltaTime);
    }
    
    if(this->blast->animations[this->blast->curAnimation].isDone()) { 
        if(p.vel.x != 0) {
            return new WalkState();
        } else {
            return new IdleState();
        }
    } else {
        if(p.dir == 1) {
            this->blast->pos = glm::vec2(p.pos.x + 32, p.pos.y - 4);
            this->blast->dir = 1;
        } else if (p.dir == -1) {
            this->blast->pos = glm::vec2(p.pos.x - 80, p.pos.y - 4);
            this->blast->dir = -1;
        }
    }

    //draw blast if needed for shotgun, could be put into its own draw function absolutely
    if(this->blast != nullptr) {
        this->blast->draw(state, gd, 80, 48);
    }

    return nullptr;
}

void ShotgunDeployState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texDeploy[p.character];
    p.curAnimation = res.ANIM_PLAYER_SHOOT;
    p.animations[p.curAnimation].reset();
    //load in shotgun
    SDL_FRect collider = { 
                            .x = 0,
                            .y = 12,
                            .w = 80,
                            .h = 24
                        };
    glm::vec2 pos;
    AnimatedObject* blast = new AnimatedObject(pos, collider, res.texShotgunBlast);
    if(p.dir == 1) {
        blast->pos = glm::vec2(p.pos.x + 32, p.pos.y - 4);
        blast->dir = 1;
    } else if (p.dir == -1) {
        blast->pos = glm::vec2(p.pos.x - 80, p.pos.y - 4);
        blast->dir = -1;
    }
    blast->animations = res.shotgunAnims;
    blast->curAnimation = res.SHOTGUN_BLAST;
    this->blast = blast;
}

void ShotgunDeployState::exit(GameData &gd, Resources &res, Player &p) {
    delete this->blast;
    this->blast = nullptr;
    p.cooldownTimer.reset();
}

// JETPACK DEPLOY
PlayerState* JetpackDeployState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    return nullptr;
}

PlayerState* JetpackDeployState::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { // TODO:
    sharedUpdate(state, p, deltaTime);
    p.vel.y -= 600.f * deltaTime;
    int vertDir = 0;
    //calculate direction
     if (state.keys[SDL_SCANCODE_W]) {
        vertDir += -1.f;
    }
    if (state.keys[SDL_SCANCODE_S]) {
        vertDir += 1.f;
    }
    if(p.currentDirection == 1 && vertDir == 0) { // right
        p.vel.x += 1000.f * deltaTime;
    } else if (p.currentDirection == -1 && vertDir == 0) { // left
        p.vel.x -= 1000.f * deltaTime;
    } else if (p.currentDirection == 0 && vertDir == 1) { // down
        p.vel.y += 1000.f * deltaTime;
    } else if (p.currentDirection == 0 && vertDir == -1) { // up
        p.vel.y -= 1000.f * deltaTime;
    } else if (p.currentDirection == 1 && vertDir == 1) { // right-down
        p.vel.x += (1000/sqrt(2)) * deltaTime;
        p.vel.y += (1000/sqrt(2)) * deltaTime;
    } else if (p.currentDirection == 1 && vertDir == -1) { // right-up
        p.vel.x += (1000/sqrt(2)) * deltaTime;
        p.vel.y -= (1000/sqrt(2)) * deltaTime;
    } else if (p.currentDirection == -1 && vertDir == 1) { // left-down
        p.vel.x -= (1000/sqrt(2)) * deltaTime;
        p.vel.y += (1000/sqrt(2)) * deltaTime;
    } else if (p.currentDirection == -1 && vertDir == -1) { // left-up
        p.vel.x -= (1000/sqrt(2)) * deltaTime;
        p.vel.y -= (1000/sqrt(2)) * deltaTime;
    }

    //Check timer for state change
    if (!p.jetpackTimer.isTimeOut()) {
        p.jetpackTimer.step(deltaTime);
    } else {
        p.cooldownTimer.reset();
        if (!p.grounded) {
            return new JumpState();
        }
        else if (p.vel.x != 0) {
            return new WalkState();
        } else {
            return new IdleState();
        }
    }
    return nullptr;
}

void JetpackDeployState::enter(GameData &gd, Resources &res, Player &p) {
    p.texture = res.texDeploy[p.character];
    p.curAnimation = res.ANIM_PLAYER_JETPACK_DEPLOY;
    p.animations[p.curAnimation].reset();
    p.jetpackTimer.reset();
}

void JetpackDeployState::exit(GameData &gd, Resources &res, Player &p) {
    //p.cooldownTimer.step(5.0f); // temp, testing
}

// GRAPPLE

PlayerState* GrappleState::handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) {
    if (auto retState = handleJumping(gd, res, p, event)) return retState;
    return nullptr;
}

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

void GrappleState::exit(GameData &gd, Resources &res, Player &p) { // this exit function is a bit redundant but it allows for the grapple to disappear if you jump out of it
    p.hook.visible = false;
    p.hook.collided = false;
    p.hook.pos += glm::vec2(-10000.0f, -10000.0f); // maybe unnecessary
    p.hook.vel = glm::vec2(0);
}