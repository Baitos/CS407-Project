#pragma once

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
    FALL, 
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
        virtual PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key) { return nullptr; }
        virtual PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) { return nullptr; }
        virtual void enter(GameData &gd, Resources &res, Player &p) {}
        virtual void exit(GameData &gd, Resources &res, Player &p) {}
        PlayerState() {
            stateVal = NONE; // if it's ever this we have a problem
        }
};

class IdleState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        IdleState() {
            stateVal = IDLE;
        }
};

class WalkState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        WalkState() {
            stateVal = WALK;
        }
};

class RunState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        RunState() {
            stateVal = RUN;
        }
};

class SprintState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        SprintState() {
            stateVal = SPRINT;
        }
};

class SlideState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        SlideState() {
            stateVal = SLIDE;
        }
};

class LaunchState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        LaunchState() {
            stateVal = LAUNCH;
        }
};

class JumpState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        JumpState() {
            stateVal = JUMP;
        }
};

class RollState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        RollState() {
            stateVal = ROLL;
        }
};

class FallState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        FallState() {
            stateVal = FALL;
        }
};

class DeadState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        DeadState() {
            stateVal = DEAD;
        }
};

class ShotgunDeployState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        ShotgunDeployState() {
            stateVal = SHOTGUN_DEPLOY;
        }
};

class SwordDeployState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        SwordDeployState() {
            stateVal = SWORD_DEPLOY;
        }
};

class JetpackDeployState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        JetpackDeployState() {
            stateVal = JETPACK_DEPLOY;
        }
};

class GrappleState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        GrappleState() {
            stateVal = GRAPPLE;
        }
};

class FastfallState : public PlayerState {
    public:
        PlayerState* handleInput(const SDLState &state, GameData &gd, Resources &res, Player &p, SDL_KeyboardEvent key);
        PlayerState* update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime);
        void enter(GameData &gd, Resources &res, Player &p);
        void exit(GameData &gd, Resources &res, Player &p);
        FastfallState() {
            stateVal = FASTFALL;
        }
};

std::string getStateFromEnum(PlayerStateValue ps);

//PlayerState * changePlayerState(GameData &gd, Resources &res, PlayerState * tempPlayer, PlayerStateValue newState);

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
PlayerState* handleJumping (GameData &gd, Resources &res, SDL_KeyboardEvent key);
PlayerState* handleRunning (GameData &gd, Resources &res, SDL_KeyboardEvent key);
PlayerState* handleSprinting (GameData &gd, Resources &res, SDL_KeyboardEvent key);
PlayerState* handleFalling (GameData &gd, Resources &res, SDL_KeyboardEvent key);
void sharedUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime);