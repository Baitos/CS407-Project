#include "../headers/minimap.h"
#include "../headers/helper.h"
#include "../headers/gameData.h"

void Minimap::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) { // update minimap dots
    // this centers the minimap's top right corner to the top right corner of the screen, allowing for minimaps of any size to work
    gd.minimap.pos.x = (gd.players_[gd.playerIndex].pos.x + TILE_SIZE / 2) + (gd.mapViewport.w / 2) - gd.minimap.texture->w; 
    gd.minimap.pos.y = (gd.players_[gd.playerIndex].pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2);
    
    float OFFSET = (this->playerDots[0].texture->w - 1) / 2; // calculate center of the square
    int count = 0;
    for (Object &d : this->playerDots) { // find pos of dots
        d.pos = this->pos - glm::vec2(OFFSET, OFFSET); // this->pos is like (0, 0) for the player, subtract offset so center of dot is at point
        d.pos.x += gd.players_[count].pos.x / TILE_SIZE;
        d.pos.y += gd.players_[count].pos.y / TILE_SIZE;
        
        ++count;
    }
}

void Minimap::draw(const SDLState &state, GameData &gd, float width, float height) {
    Object::draw(state, gd, width, height); // do generic object draw
    for (Object &d : this->playerDots) { // draw dots
        d.draw(state, gd, MINIMAP_DOT_SIZE, MINIMAP_DOT_SIZE);
    }
}