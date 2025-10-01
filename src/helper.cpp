#include "../headers/helper.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/player.h"
#include "../headers/globals.h"
#include "../headers/gameobject.h"

bool isOnscreen(const SDLState &state, GameState &gs, GameObject &obj) { // checks if obj is onscreen (with some leeway)
    bool onscreen = true;
    if (obj.pos.x - gs.mapViewport.x + 2 * TILE_SIZE < 0 || // left side
        obj.pos.x - gs.mapViewport.x - 2 * TILE_SIZE > state.logW || // right side
        obj.pos.y - gs.mapViewport.y + 2 * TILE_SIZE < 0 || // up
        obj.pos.y - gs.mapViewport.y - 2 * TILE_SIZE > state.logH) // down
    {
        onscreen = false;
    }
    return onscreen;
}

float changeVel(float vel, GameObject &obj) { // this is for ease of accounting for obj_flip without having to have it every time, use when you change/need vel/pos
    return vel * obj.flip; 
}

bool isSliding(GameObject &obj) { // checks if an obj is sliding
    if (obj.vel.x * obj.dir < 0) {
       return true;     
    }
    return false;
}

glm::vec2 findCenterOfSprite(GameObject &obj) { // finds center of sprite by collider
    return glm::vec2((float)obj.collider.w / 2, (float)obj.collider.h / 2);
}
