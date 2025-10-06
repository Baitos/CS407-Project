#pragma once

class Player;
enum PlayerStateType {
    IDLE,
    MOVING,
    SPRINTING,
};
class PlayerState {
    public:
        PlayerStateType type;
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

};
