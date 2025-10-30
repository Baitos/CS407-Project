#include "../headers/drawLevel.h"
#include "../headers/helper.h"
#include "../headers/state.h"

//Draw Function for level Spaceship
void drawLevel(const SDLState &state, GameData &gd, Resources res, float deltaTime){
    // used for camera system
    gd.mapViewport.x = (gd.players_[0].pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    gd.mapViewport.y = (gd.players_[0].pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 
    //draw bg
    SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
    SDL_RenderClear(state.renderer);

    // draw bg tiles
    for (BackgroundObject &bg : gd.bgTiles_) {
        bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
    }

    // draw level tiles
    for(Level &level : gd.mapTiles_) {
        level.draw(state, gd, TILE_SIZE, TILE_SIZE);
    }

    // draw portal tiles
    for(Portal &portal : gd.portals_) {
        portal.draw(state, gd, TILE_SIZE, TILE_SIZE * 2);
    }

    // draw lasers
    for(Laser &laser : gd.lasers_) {
        if (laser.laserActive) {
            laser.draw(state, gd, TILE_SIZE, TILE_SIZE);
        }
    }

    // draw players
    for(Player &p : gd.players_) {
        p.draw(state, gd, TILE_SIZE, TILE_SIZE);
        p.state_->draw(state, gd); // currently only for shotgun but could be used in future for state based particles/power up effects
    }


    for(ItemBox &box : gd.itemBoxes_) {
        if (box.itemBoxActive) {
            box.draw(state, gd, TILE_SIZE, TILE_SIZE);
        }
    }
    gd.itemStorage_.draw(state, gd, 68, 68);

    gd.minimap.draw(state, gd, gd.minimap.texture->w, gd.minimap.texture->h);

    handleCrosshair(state, gd, res, deltaTime);
    
}