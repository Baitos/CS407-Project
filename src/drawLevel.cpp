#include "../headers/drawLevel.h"
#include "../headers/helper.h"
#include "../headers/state.h"

//Draw Function for level Spaceship
void drawLevel(const SDLState &state, GameData &gd, Resources res, float deltaTime){
    // used for camera system
    gd.mapViewport.x = (gd.player.pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    gd.mapViewport.y = (gd.player.pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 
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

    // draw player
    gd.hook.draw(state, gd, HOOK_SIZE, HOOK_SIZE);
    gd.player.draw(state, gd, TILE_SIZE, TILE_SIZE); // draw player class  
    handleCrosshair(state, gd, res, deltaTime);

    //draw blast if needed for shotgun
    if(gd.player.blast != nullptr) {
        gd.player.blast->draw(state, gd, 80, 48);
    }

    for(Laser &laser : gd.lasers_) {
        if (laser.laserActive) {
            laser.draw(state, gd, TILE_SIZE, TILE_SIZE);
        }
    }


}