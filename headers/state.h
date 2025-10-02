#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>

#include "../headers/update.h"
#include "../headers/collision.h"
#include "../headers/gameData.h"
#include "../headers/initState.h"
#include "../headers/helper.h"
#include "../headers/globals.h"
#include "../headers/resources.h"

enum gameStates {
    TITLE,
    SETTINGS,
    HOST,
    JOIN,
    CHAR_SELECT,
    SPACESHIP,
    RESULTS,
    CREDITS
};

class GameState{
    public:
        int prevStateVal;
        int currStateVal;
        int nextStateVal;
        GameState * prevState;
        //GameState * nextState; //May not need
        void (*input)(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime);
        void (*update)();
        void (*render)();
        // void (*init)();

};

class TitleState : public GameState{

};

class SettingsState : public GameState{

};

class HostState : public GameState {

};

class JoinState : public GameState {

};

class CharSelectState : public GameState{

};

class LevelState : public GameState{
    public:
        void (*checkCollision)(const SDLState &state, GameData &gd, Resources &res,
	GameObject &a, GameObject &b, float deltaTime);
        void (*collisionResponse)(const SDLState &state, GameData &gd, Resources &res,
	const SDL_FRect &rectA, const SDL_FRect &rectB, const glm::vec2 &overlap,
	GameObject &a, GameObject &b, float deltaTime);
        bool (*aabbIntersection)(const SDL_FRect &a, const SDL_FRect &b, glm::vec2 &overlap); //Ask ryan what aabb means
        bool (*isOnScreen)(const SDLState &state, GameData &gd, GameObject &obj);
        float (*changeVelocity)(float vel, GameObject &obj);
        void (*init)(const SDLState &state, GameData &gd, const Resources &res);
};

/*class SpaceshipState : public LevelState {
     void (*createTiles)();
};

Add a copy for each additional stage
class LevelX : public LevelState {
     void (*createTiles)();
};
*/

class ResultsState : public GameState {

};

class CreditsState : public GameState {

};

