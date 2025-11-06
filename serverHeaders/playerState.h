#pragma once
#include "gameData.h"

class Timer;
class Player;

enum PlayerStateValue {
    NOSTATE,
    IDLE, 
    WALK, 
    RUN, 
    SPRINT,
    SLIDE,
    FASTFALL, 
    LAUNCH, 
    JUMP, 
    ROLL, 
    STUNNED, 
    DEAD,
    SWORD_DEPLOY,
    SHOTGUN_DEPLOY,
    JETPACK_DEPLOY,
    GRAPPLE
};

class PlayerState {
    public:
        PlayerStateValue stateVal; // enum for state value
        virtual ~PlayerState() = default;
        // Generic handlers (The player's state will call these by default if they don't have a new version defined, mostly for enter/exit)
        virtual PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown) { return nullptr; }
        virtual PlayerState* update(const SDLState &state, GameData &gd, Player &p, float deltaTime, int keyID, int keyDown) { return nullptr; }
        virtual void enter(GameData &gd, Player &p) {}
        virtual void exit(GameData &gd, Player &p) {}
        PlayerState() {
            stateVal = NOSTATE; // if it's ever this we have a problem
        }
};

class IdleState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd, Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd, Player &p);
        void exit(GameData &gd, Player &p) {} // do nothing
        IdleState() {
            stateVal = IDLE;
        }
};

class WalkState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd, Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd, Player &p);
        void exit(GameData &gd,Player &p) {} // do nothing
        WalkState() {
            stateVal = WALK;
        }
};

class RunState : public PlayerState {
    public:
        Timer sprintTimer;
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd, Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd, Player &p);
        void exit(GameData &gd, Player &p) {} // do nothing
        RunState() : sprintTimer(1.5f) {
            stateVal = RUN;
        }
};

class SprintState : public PlayerState {
    public:
        AnimatedObject afterImage[3];
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd, Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd, Player &p);
        void exit(GameData &gd,Player &p) {} // do nothing
        SprintState() {
            stateVal = SPRINT;
        }
};

class SlideState : public PlayerState {
    public:
        
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd, Player &p);
        void exit(GameData &gd, Player &p) {} // do nothing
        SlideState() {
            stateVal = SLIDE;
        }
};

class LaunchState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd, Player &p) {} // do nothing
        LaunchState() {
            stateVal = LAUNCH;
        }
};

class JumpState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p) {} // do nothing
        JumpState() {
            stateVal = JUMP;
        }
};

class RollState : public PlayerState {
    public:
         // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p) {} // do nothing
        RollState() {
            stateVal = ROLL;
        }
};

class StunnedState : public PlayerState {
    public:
        bool hardStun;
         // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd,  Player &p, SDL_Event event) { return nullptr; } // do nothing
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p) {} // do nothing
        StunnedState() {
            stateVal = STUNNED;
            hardStun = false;
        }
        StunnedState(bool hardStun_) {
            stateVal = STUNNED;
            hardStun = hardStun_;
        }
};

class DeadState : public PlayerState {
    public:
         // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd,  Player &p, SDL_Event event) { return nullptr; } // do nothing
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p) {} // do nothing
        DeadState() {
            stateVal = DEAD;
        }
};

class ShotgunDeployState : public PlayerState {
    public:
        AnimatedObject* blast;
        void draw(const SDLState &state, GameData &gd);
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p); // do nothing
        ShotgunDeployState() {
            stateVal = SHOTGUN_DEPLOY;
        }
};

class SwordDeployState : public PlayerState {
    public:
         // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p) {} // do nothing
        SwordDeployState() {
            stateVal = SWORD_DEPLOY;
        }
};

class JetpackDeployState : public PlayerState {
    public:
         // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p);
        JetpackDeployState() {
            stateVal = JETPACK_DEPLOY;
        }
};

class GrappleState : public PlayerState {
    public:
         // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p);
        GrappleState() {
            stateVal = GRAPPLE;
        }
};

class FastfallState : public PlayerState {
    public:
         // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Player &p, int keyID, int keyDown);
        PlayerState* update(const SDLState &state, GameData &gd,  Player &p, float deltaTime, int keyID, int keyDown);
        void enter(GameData &gd,  Player &p);
        void exit(GameData &gd,  Player &p);
        FastfallState() {
            stateVal = FASTFALL;
        }
};

std::string getStateFromEnum(PlayerStateValue ps);
//Handlers
PlayerState* handleJumping (GameData &gd,  Player &p, int keyID, int keyDown);
PlayerState* handleSprinting (GameData &gd,  Player &p, int keyID, int keyDown);
PlayerState* handleFalling (GameData &gd,  Player &p, int keyID, int keyDown);
void sharedUpdate(const SDLState &state, Player &p,  float deltaTime,  GameData &gd);
void sharedGravity(Player &p, float deltaTime);
void sharedMovement(const SDLState &state, Player &p);
