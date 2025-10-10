#include "../headers/drawMenu.h"
#include "../headers/helper.h"
#include "../headers/state.h"

void drawCharSelect(const SDLState &state, GameData &gd, Resources res, float deltaTime){
    // used for camera system
    gd.mapViewport.x = 0; 
    gd.mapViewport.y = 0; 
    //draw bg
    SDL_SetRenderDrawColor(state.renderer, 13, 22, 59, 255);
    SDL_RenderClear(state.renderer);

    // draw bg tiles
    for (BackgroundObject &bg : gd.bgTiles_) {
        bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
    }

    for (AnimatedObject &p : gd.previews_){
        p.draw(state, gd, 96, 96); 
    }

    //Draw charIcon tiles
    for (charIconObject &ci : gd.charIcons_){
        ci.draw(state, gd, 34, 36); 
    }
    
    if(gd.settingsBorder.pos.y != 500.f){
        //printf("drawing, %f", gd.settingsBorder->pos.y);
        gd.settingsBorder.draw(state, gd,static_cast<float>(gd.settingsBorder.texture->w) * 2, static_cast<float>(gd.settingsBorder.texture->h)*2);
    }
    handleMousePointerCharSelect(state, gd, res, deltaTime);
}


void drawSettings(const SDLState &state, GameData &gd, Resources res, float deltaTime){
    // used for camera system
    gd.mapViewport.x = 0; 
    gd.mapViewport.y = 0; 
    //draw bg
    SDL_SetRenderDrawColor(state.renderer, 13, 22, 59, 255);
    SDL_RenderClear(state.renderer);

    // draw bg tiles
    for (BackgroundObject &bg : gd.bgTiles_) {
        bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
    }
    for (Object &o : gd.settingsDials_) {
        o.draw(state, gd, static_cast<float>(o.texture->w) * 2, static_cast<float>(o.texture->h) * 2); 
    }

    

    if(gd.settingsBorder.pos.y != 500.f){
        //printf("drawing, %f", gd.settingsBorder->pos.y);
        gd.settingsBorder.draw(state, gd,static_cast<float>(gd.settingsBorder.texture->w) * 2, static_cast<float>(gd.settingsBorder.texture->h)*2);
    }
    handleMousePointerSettings(state, gd, res, deltaTime);
}