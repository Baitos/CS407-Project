#include "../serverHeaders/helper.h"
#include "../serverHeaders/initState.h"
#include "../serverHeaders/gameData.h"
#include "../serverHeaders/resources.h"
#include "../serverHeaders/player.h"
#include "../serverHeaders/globals.h"
#include "../serverHeaders/object.h"

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