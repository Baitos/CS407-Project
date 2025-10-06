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
    DEAD
};

class PlayerState {
    public:
        int currStateVal;
        int nextStateVal;
        virtual ~PlayerState() {}
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
    
};

class IdleState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};

class WalkState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};

class RunState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};

class SprintState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};

class JumpLaunchState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};

class JumpState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};

class RollState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};

class FallState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};

class DeadState : public PlayerState {
    public:
        virtual PlayerState* handleInput(Player& player, SDL_KeyboardEvent key);
        virtual void update(Player& player);
        virtual void enter(Player& player);
};


PlayerState * changePlayerState(PlayerState * tempPlayer);
