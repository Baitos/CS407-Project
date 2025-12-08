#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include "../ext/glm/glm.hpp"
#include "object.h"
#include "player.h"

struct SDLState;
struct GameData;
struct Resources;
struct Lobby;

bool isOnscreen(const SDLState &state, GameData &gd, Object &obj);
float changeVel(float vel, Player &p);
bool isSliding(Player &p);
glm::vec2 findCenterOfSprite(Object &obj);
std::vector<float> distanceForm(GameData &gd, Object &a, Object &b); // returns xDist, yDist, dist = sqrt[(b.x-a.x)^2 + (b.y-a.y)^2], ah, oh as a vector
glm::vec2 updatePos(Object &o, float deltaTime);

void handleOutOfMap(GameData &gd, Resources &res, Player &p); // checks if player is out of map and puts them in dead state if so
void removeHook(Player &p); // removes player hook (click released, state changed, etc)

std::vector<Object*> getCloseTiles(const SDLState &state, GameData &gd, glm::vec2 pos); // gets tiles that are in close proximity to given obj position
std::vector<Object*> getOnscreenTiles(const SDLState &state, GameData &gd); // gets tiles that are onscreen

void slowObject(glm::vec2 &vel, float deltaTime); // slow down object 
void sendJoinRequest(Lobby lobby);
void speedObject(glm::vec2 &vel, float deltaTime); // speed up object 

SDL_Texture* createTextTexture(SDL_Renderer* renderer, TTF_Font*font, const std::string &text, SDL_Color color, int &outW, int &outH);
