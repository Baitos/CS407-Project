#pragma once

class Player;

enum PlayerStateValues {
    IDLE, 
    WALK, 
    RUN, 
    SPRINT,
    SLIDE, 
    LAUNCH, 
    JUMP, 
    ROLL, 
    FALL, 
    DEAD,
    SWORD_DEPLOY,
    SHOTGUN_DEPLOY,
    JETPACK_DEPLOY,
    GRAPPLE
};

class PlayerState {
    public:
        int currStateVal;
        //virtual ~PlayerState() {}
        void (*handleInput)(GameData &gd, Resources &res, SDL_KeyboardEvent key);
        void (*update)(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
        void (*enter)(Player& player, GameData &gd, Resources &res);
    
};

class IdleState : public PlayerState {

};

class WalkState : public PlayerState {

};

class RunState : public PlayerState {

};

class SprintState : public PlayerState {

};

class SlideState : public PlayerState {

};

class JumpLaunchState : public PlayerState {

};

class JumpState : public PlayerState {

};

class RollState : public PlayerState {

};

class FallState : public PlayerState {

};

class DeadState : public PlayerState {

};

class ShotgunDeployState : public PlayerState {

};

class SwordDeployState : public PlayerState {

};

class JetpackDeployState : public PlayerState {

};

class GrappleState : public PlayerState {

};

PlayerState * changePlayerState(GameData &gd, Resources &res, PlayerState * tempPlayer, PlayerStateValues newState);

//Idle Functions
void handleInputIdle(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateIdle(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterIdle(Player& player, GameData &gd, Resources &res);
//Walk Functions
void handleInputWalk(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateWalk(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterWalk(Player& player, GameData &gd, Resources &res);
//Run Fuctions
void handleInputRun(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateRun(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterRun(Player& player, GameData &gd, Resources &res);
// Slide Functions
void handleInputSlide(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateSlide(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterSlide(Player& player, GameData &gd, Resources &res);
//Jump Functions
void handleInputJump(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateJump(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterJump(Player& player, GameData &gd, Resources &res);
//Launch Functions
void handleInputLaunch(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateLaunch(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterLaunch(Player& player, GameData &gd, Resources &res);
//Roll Functions
void handleInputRoll(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateRoll(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterRoll(Player& player, GameData &gd, Resources &res);
//Sprint Functions
void handleInputSprint(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateSprint(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterSprint(Player& player, GameData &gd, Resources &res);
//Grapple Functions
void handleInputGrapple(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateGrapple(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterGrapple(Player& player, GameData &gd, Resources &res);
//Fall Functions
void updateFalling(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterFall(Player& player, GameData &gd, Resources &res);
//Dead Functions
void enterDead(Player& player, GameData &gd, Resources &res);
//Dummy Functions?
void dummyEnter(Player& player, GameData &gd, Resources &res);
void dummyInput(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void dummyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void emptyUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
//Jetpack Deploy Functions
void handleInputJetpackDeploy(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateJetpackDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterJetpackDeploy(Player& player, GameData &gd, Resources &res);
//Shotgun Deploy Functions
void handleInputShotgunDeploy(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateShotgunDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterShotgunDeploy(Player& player, GameData &gd, Resources &res);
//Sword Deploy Functions
void handleInputSwordDeploy(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateSwordDeploy(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterSwordDeploy(Player& player, GameData &gd, Resources &res);

void handleInputGrapple(GameData &gd, Resources &res, SDL_KeyboardEvent key);
void updateGrapple(const SDLState &state, GameData &gd, Resources &res, float deltaTime);
void enterGrapple(Player& player, GameData &gd, Resources &res);

//Handlers
void handleJumping (GameData &gd, Resources &res, SDL_KeyboardEvent key);
void handleRunning (GameData &gd, Resources &res, SDL_KeyboardEvent key);
void handleSprinting (GameData &gd, Resources &res, SDL_KeyboardEvent key);
void handleFalling (GameData &gd, Resources &res, SDL_KeyboardEvent key);
void sharedUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);