#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "../ext/glm/glm.hpp"
#include "initState.h"
#include "player.h"
#include "menu.h"

struct GameData {
    std::vector<Player> players_;
    std::vector<Laser> lasers_;
    std::vector<ItemBox> itemBoxes_;
    std::vector<Item> items_;
    std::vector<Portal> portals_;

    glm::vec2 ExitPortal, 
              EntrancePortal;

    ItemStorage itemStorage_;
    Object border;
    std::vector<Level> mapTiles_;

    int numPlayers; 

    GameData(const SDLState &state) {
        numPlayers = 0;std::vector<Lobby> parseLobbies(std::string LobbyQuery) {
    // TODO Parse lobbies
}
void sendJoinRequest(Lobby lobby) {
    // Todo Format join
}
    }
};