#include "../headers/helper.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/globals.h"
#include "../headers/object.h"

bool isOnscreen(const SDLState &state, GameData &gd, Object &obj) { // checks if obj is onscreen (with some leeway)
    bool onscreen = true;
    if (obj.pos.x - gd.mapViewport.x + 2 * TILE_SIZE < 0 || // left side
        obj.pos.x - gd.mapViewport.x - 2 * TILE_SIZE > state.logW || // right side
        obj.pos.y - gd.mapViewport.y + 2 * TILE_SIZE < 0 || // up
        obj.pos.y - gd.mapViewport.y - 2 * TILE_SIZE > state.logH) // down
    {
        onscreen = false;
    }
    return onscreen;
}

float changeVel(float vel, Player &p) { // this is for ease of accounting for obj_flip without having to have it every time, use when you change/need vel/pos
    return vel * p.flip; 
}

bool isSliding(Player &p) { // checks if a player is sliding
    if (p.vel.x * p.dir < 0 && p.grounded) {
       return true;     
    }
    return false;
}

glm::vec2 findCenterOfSprite(Object &obj) { // finds center of sprite by collider
    return glm::vec2((float)obj.collider.w / 2, (float)obj.collider.h / 2);
}

std::vector<float> distanceForm(GameData &gd, Object &a, Object &b) {
    glm::vec2 pOffset = findCenterOfSprite(a);
    float xDist = gd.mouseCoords.x - (a.pos.x - gd.mapViewport.x + pOffset.x); // A
    float yDist = gd.mouseCoords.y - (a.pos.y - gd.mapViewport.y + pOffset.y); // O
    float dist = std::sqrt(xDist * xDist + yDist * yDist); // distance formula, H
    float aH = xDist / dist; // cos
    float oH = yDist / dist; // sin
    std::vector<float> res = {xDist, yDist, dist, aH, oH};
    return res;
    //printf("xDist: %f, yDist: %f, dist: %f\n", xDist, yDist, dist);
}

glm::vec2 updatePos(Object &o, float deltaTime) {
    return o.vel * deltaTime;
}

void removeHook(Player &p) {
    p.hook.visible = false;
    p.hook.collided = false;
    p.hook.pos += glm::vec2(-10000.0f, -10000.0f); // maybe unnecessary
    p.hook.vel = glm::vec2(0);
}

void handleOutOfMap(GameData &gd, Resources &res, Player &p) {
    float maxY = 3000;
    if (std::abs(p.pos.y) > maxY && p.state_->stateVal != DEAD) {
        PlayerState* dState = new DeadState();
        p.handleState(dState, gd, res);
    }
}

bool isInBounds(GameData &gd, int x, int y) {
    if (x < 0 || y < 0 || x >= gd.grid_[0].size() || y >= gd.grid_.size()) {
            return false;
    }
    return true;
}

std::vector<Object*> getCloseTiles(const SDLState &state, GameData &gd, glm::vec2 pos) {
    std::vector<Object*> res;
    int BOUND = 3;
    int OFFSET = (BOUND - 1) / 2;
    glm::vec2 tilePos(std::round(pos.x / TILE_SIZE), std::round(pos.y / TILE_SIZE));
    for (int r = 0; r < BOUND; r++) {
        for (int c = 0; c < BOUND; c++) {
            int x = (int)tilePos.x + c - OFFSET;
            int y = (int)tilePos.y + r - OFFSET;
            //printf("posX = %f, posY = %f, tilePosX = %d, tilePosY = %d\n", pos.x, pos.y, x, y);
            if (isInBounds(gd, x, y) && gd.grid_[y][x] != nullptr) {
                res.push_back(gd.grid_[y][x]); // get 3x3 grid of tiles around object, with obj at center
            }
        }
    }
    return res;
}

std::vector<Object*> getOnscreenTiles(const SDLState &state, GameData &gd) {
    std::vector<Object*> res;   
    int leeway = 2; // leeway for slightly offscreen things
    int width = state.logW / TILE_SIZE + leeway;
    int offsetW = (width - 1) / 2;
    int height = state.logH / TILE_SIZE + leeway;
    int offsetH = (height - 1) / 2;
    glm::vec2 tilePos(std::round(gd.players_[0].pos.x / TILE_SIZE), std::round(gd.players_[0].pos.y / TILE_SIZE)); // get player pos
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            int x = (int)tilePos.x + w - offsetW;
            int y = (int)tilePos.y + h - offsetH;
            //printf("posX = %f, posY = %f, tilePosX = %d, tilePosY = %d\n", pos.x, pos.y, x, y);
            if (isInBounds(gd, x, y) && gd.grid_[y][x] != nullptr) {
                res.push_back(gd.grid_[y][x]); // get 3x3 grid of tiles around object, with obj at center
            }
        }
    }
    return res;
}

void slowObject(glm::vec2 &vel, float deltaTime) {
    if (abs(vel.x) >= 50) {
        if (vel.x > 0) {
                vel.x -= 150.f * deltaTime;
            } else {
                vel.x += 150.f * deltaTime;
            }
        }
    if (abs(vel.y) >= 50) {
        if (vel.y > 0) {
            vel.y -= 150.f * deltaTime;
        } else {
            vel.y += 150.f * deltaTime;
        }
    }		
}