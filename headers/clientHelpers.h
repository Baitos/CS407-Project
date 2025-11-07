#include "../headers/enet.h"

void charSelectMessageHandler(ENetEvent * event, GameData * gd, Resources &res, SDLState &state);
void levelMessageHandler(ENetEvent * event, GameData * gd, Resources &res, SDLState &state);
void joinMessageHandler(GameData * gd, std::string message);
Lobby * getLobbyFromString(std::string lobbyStr);