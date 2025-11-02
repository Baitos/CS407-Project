#pragma once

class Timer;
class Player;

enum PlayerStateValue {
    NONE,
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
        virtual void draw(const SDLState &state, GameData &gd) {} // do nothing
        // Generic handlers (The player's state will call these by default if they don't have a new version defined, mostly for enter/exit)
        virtual PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) { return nullptr; }
        virtual PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { return nullptr; }
        virtual void enter(GameData &gd, Resources &res, Player &p) {}
        virtual void exit(GameData &gd, Resources &res, Player &p) {}
        PlayerState() {
            stateVal = NONE; // if it's ever this we have a problem
        }
};

class IdleState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        IdleState() {
            stateVal = IDLE;
        }
};

class WalkState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        WalkState() {
            stateVal = WALK;
        }
};

class RunState : public PlayerState {
    public:
        Timer sprintTimer;
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        RunState() : sprintTimer(1.5f) {
            stateVal = RUN;
        }
};

class SprintState : public PlayerState {
    public:
        AnimatedObject afterImage[3];
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        SprintState() {
            stateVal = SPRINT;
        }
};

class SlideState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        SlideState() {
            stateVal = SLIDE;
        }
};

class LaunchState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        LaunchState() {
            stateVal = LAUNCH;
        }
};

class JumpState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        JumpState() {
            stateVal = JUMP;
        }
};

class RollState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        RollState() {
            stateVal = ROLL;
        }
};

class StunnedState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) { return nullptr; } // do nothing
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        StunnedState() {
            stateVal = STUNNED;
        }
};

class DeadState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event) { return nullptr; } // do nothing
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        DeadState() {
            stateVal = DEAD;
        }
};

class ShotgunDeployState : public PlayerState {
    public:
        AnimatedObject* blast;
        void draw(const SDLState &state, GameData &gd);
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p); // do nothing
        ShotgunDeployState() {
            stateVal = SHOTGUN_DEPLOY;
        }
};

class SwordDeployState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        SwordDeployState() {
            stateVal = SWORD_DEPLOY;
        }
};

class JetpackDeployState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p) {} // do nothing
        JetpackDeployState() {
            stateVal = JETPACK_DEPLOY;
        }
};

class GrappleState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        GrappleState() {
            stateVal = GRAPPLE;
        }
};

class FastfallState : public PlayerState {
    public:
        void draw(const SDLState &state, GameData &gd) {} // do nothing
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_Event event);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        FastfallState() {
            stateVal = FASTFALL;
        }
};

std::string getStateFromEnum(PlayerStateValue ps);
//Handlers
PlayerState* handleJumping (GameData &gd, Resources &res, Player &p, SDL_Event event);
PlayerState* handleSprinting (GameData &gd, Resources &res, Player &p, SDL_Event event);
PlayerState* handleFalling (GameData &gd, Resources &res, Player &p, SDL_Event event);
void sharedUpdate(const SDLState &state, GameData &gd, Player &p, Resources &res, float deltaTime);
void sharedGravity(Player &p, float deltaTime);
void sharedMovement(const SDLState &state, Player &p);
