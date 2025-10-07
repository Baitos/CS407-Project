#pragma once

class Player;

enum PlayerStateValues {
    IDLE, 
    WALK, 
    RUN, 
    SPRINT, 
    LAUNCH, 
    JUMP, 
    ROLL, 
    FALL, 
    DEAD,
    SWORD_DEPLOY,
    SHOTGUN_DEPLOY,
    JETPACK_DEPLOY
};

class PlayerState {
    public:
        int currStateVal;
        int nextStateVal;
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

class SwordDepolyState : public PlayerState {

};

class JetpackDeployState : public PlayerState {

};

PlayerState * changePlayerState(PlayerState * tempPlayer);

void handleInputIdle(GameData &gd, Resources &res, SDL_KeyboardEvent key, float deltaTime);
void handleJumping (GameData &gd, Resources &res, SDL_KeyboardEvent key);
void enterLaunch(Player& player, GameData &gd, Resources &res);