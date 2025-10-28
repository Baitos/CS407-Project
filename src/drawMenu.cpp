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

    //draw map option
    for (AnimatedObject &map:gd.map_previews_){
        map.draw(state, gd, static_cast<float>(map.texture->w), static_cast<float>(map.texture->h));
    }
    if(!gd.isGrandPrix){
        for (AnimatedObject &map:gd.map_previews_text_){
            map.draw(state, gd, static_cast<float>(map.texture->w), static_cast<float>(map.texture->h));
        }
    }

    //draw arrows
    for (AnimatedObject &arrow:gd.arrows_){
        if(arrow.visible) {
            arrow.draw(state, gd, static_cast<float>(arrow.texture->w), static_cast<float>(arrow.texture->h));
        }
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

void drawGameplaySettings(const SDLState &state, GameData &gd, Resources res, float deltaTime){
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
    //braw brackets around grand prix or other
    if(!gd.isGrandPrix) {
        for (Object &bg : gd.gameplaySettingsBrackets2_) {
            bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
        }
    } else {
        for (Object &bg : gd.gameplaySettingsBrackets1_) {
            bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
        }
    }
    for (Object &bg : gd.gameplaySettingsNumLaps_) {
        bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
    }

    if(gd.settingsBorder.pos.y != 500.f){
        //printf("drawing, %f", gd.settingsBorder->pos.y);
        gd.settingsBorder.draw(state, gd,static_cast<float>(gd.settingsBorder.texture->w) * 2, static_cast<float>(gd.settingsBorder.texture->h)*2);
    }

    //draw arrows
    for (AnimatedObject &arrow:gd.arrows_){
        if(arrow.visible) {
            //printf("draw arrow at %f %f", arrow.pos.x, arrow.pos.y);
            arrow.draw(state, gd, static_cast<float>(arrow.texture->w), static_cast<float>(arrow.texture->h));
        }
    }

    handleMousePointerGameplaySettings(state, gd, res, deltaTime);
}

void drawTitle(const SDLState &state, GameData &gd, Resources res, float deltaTime){
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
    if(gd.settingsBorder.pos.y != 500.f){
        //printf("drawing, %f", gd.settingsBorder->pos.y);
        gd.settingsBorder.draw(state, gd,static_cast<float>(gd.settingsBorder.texture->w) * 2, static_cast<float>(gd.settingsBorder.texture->h)*2);
    }

    //draw username
    std::string username = "temp";
    SDL_Color color = {0, 239, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(gd.font, username.c_str(), username.length(), color);
    SDL_Texture* textTex = SDL_CreateTextureFromSurface(state.renderer, textSurface);
    SDL_FRect rect = { 370, 173, (float)textSurface->w, (float)textSurface->h };
    SDL_RenderTexture(state.renderer, textTex, nullptr, &rect);
    SDL_DestroyTexture(textTex);
    SDL_DestroySurface(textSurface);

    handleMousePointerTitle(state, gd, res, deltaTime);
}