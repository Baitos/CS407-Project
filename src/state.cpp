#include "../headers/createTiles.h"
#include "../headers/drawLevel.h"
#include "../headers/updateMenu.h"
#include "../headers/updateLevel.h"
#include "../headers/state.h"
#include "../headers/collision.h"
#include "../headers/drawMenu.h"

extern GameState * currState;

//Function called to change the state
//NOTE: When a state is changed, call currState->init() immediately after

//Init functions should be located in createTiles.cpp
//Input and Update functions should be located in updateLevel/Menu.cpp
//Render functions should be located in drawLevel/Menu.cpp

GameState * changeState(GameState * tempState, GameData &gd){
    GameState * newState;
    //tempState->nextStateVal = CHAR_SELECT;
    //Initialize nextState
    switch (tempState->nextStateVal){
        case TITLE:
        {
            newState = new TitleState();
            newState->init = initTitle;
            newState->update = titleUpdate;
            newState->render = drawTitle;
            newState->input = titleInput;
            break;
        }
        case SETTINGS:
        {
            newState = new SettingsState();
            newState->init = initSettings;
            newState->update = settingsUpdate;
            newState->render = drawSettings;
            newState->input = settingsInputs;
            break;
        }
        case HOST:
        {
            newState = new HostState();
            newState->init = initHostLobby;
            newState->update = hostLobbyUpdate;
            newState->render = drawHostLobby;
            newState->input = hostLobbyInput;
            break;
        }
        case JOIN:
        {
            newState = new JoinState();
            newState->init = initJoinLobby;
            newState->update = joinLobbyUpdate;
            newState->render = drawJoinLobby;
            newState->input = joinLobbyInput;
            break;
        }
        case CHAR_SELECT:
        {
            newState = new CharSelectState();
            newState->init = initCharSelect;
            newState->update = charSelectUpdate;
            newState->render = drawCharSelect;
            newState->input = charSelectInputs;
            break;           
        }
        case SPACESHIP:
        {   
            gd.players_.clear();
            gd.player_placement_.clear();
            characterType type;
            if(((ResultsState*)currState)->character) {
                type = ((ResultsState*)currState)->character;
            }
            //Creating LevelState with init of Spaceship
            newState = new LevelState();
            newState->input = levelInputs;
            newState->update = levelUpdate;
            newState->render = drawLevel;
            newState->init = createTilesSpaceship;    
            
            if(type) {
                ((LevelState*)newState)->character = type;
            }
            break;
        }
        case GRASSLANDS:
        {
            gd.players_.clear();
            gd.player_placement_.clear();
            characterType type;
            if(((ResultsState*)currState)->character) {
                type = ((ResultsState*)currState)->character;
            }
            //Creating LevelState with init of Spaceship
            newState = new LevelState();
            newState->input = levelInputs;
            newState->update = levelUpdate;
            newState->render = drawLevel;
            newState->init = createTilesGrassland;    

            if(type) {
                ((LevelState*)newState)->character = type;
            }
            break;
        }
        case SNOW:
        {
            gd.players_.clear();
            gd.player_placement_.clear();
            characterType type;
            if(((ResultsState*)currState)->character) {
                type = ((ResultsState*)currState)->character;
            }
            //Creating LevelState with init of Spaceship
            newState = new LevelState();
            newState->input = levelInputs;
            newState->update = levelUpdate;
            newState->render = drawLevel;
            newState->init = createTilesSnow;    

            if(type) {
                ((LevelState*)newState)->character = type;
            }
            break;
        }
        case GAMEPLAY_SETTINGS:
        {
             //Creating LevelState with init of Spaceship
            newState = new GameplaySettingsState();
            newState->input = gameplaySettingsInput;
            newState->update = gameplaySettingsUpdate;
            newState->render = drawGameplaySettings;
            newState->init = initGameplaySettings;    
            break;
        }

        case RESULTS:
        {
            characterType type;
            if(((LevelState*)currState)->character) {
                type = ((LevelState*)currState)->character;
            }
    
            newState = new ResultsState();
            newState->render = drawResults;
            newState->init = initResults;
            newState->input = resultsInputs;
            newState->update = resultsUpdate;
            if(type) {
                ((ResultsState*)newState)->character = type;
            }
            break;
        }
        case END_RESULTS:
        {
            newState = new EndResultsState();
            newState->render = drawEndResults;
            newState->init = initEndResults;
            newState->input = endResultsInputs;
            newState->update = endResultsUpdate;
            break;
        }
    }
    tempState->unloadGameState(gd);
    newState->prevStateVal = tempState->currStateVal;
    newState->currStateVal = tempState->nextStateVal;
    newState->prevState = tempState;

    //To be removed when new screens are added
    //newState->nextStateVal = SPACESHIP;
    return newState;
}

void GameState::unloadGameState(GameData &gd) {
    gd.mapTiles_.clear();
    gd.bgTiles_.clear();
    gd.lasers_.clear();
    gd.portals_.clear();
    gd.lava_.clear();
    gd.water_.clear();
    gd.signs_.clear();
    gd.itemBoxes_.clear();
    gd.wire_.clear();

    gd.checkpoints_.clear();

    gd.md.gameplaySettingsNumLaps_.clear();
    gd.md.map_previews_.clear();
    gd.md.gameplaySettingsBrackets1_.clear();
    gd.md.gameplaySettingsBrackets2_.clear();
    gd.md.map_previews_text_.clear();
    gd.md.previews_.clear();
    gd.md.arrows_.clear();

    gd.rd.roundResults.clear();
    gd.rd.cumulativeResults.clear();

    gd.grid_.clear();

    gd.num_finished = 0;

    //TTF_CloseFont(gd.font);
    //TO-DO Add clearing players
    //gd.players_.clear();
}