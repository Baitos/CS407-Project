#include "../headers/drawMenu.h"
#include "../headers/helper.h"
#include "../headers/state.h"
#include "../headers/controls.h"
extern GameState * currState;
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

    for (AnimatedObject &p : gd.md.previews_){
        p.draw(state, gd, 96, 96); 
    }

    //Draw charIcon tiles
    for (charIconObject &ci : gd.md.charIcons_){
        ci.draw(state, gd, 34, 36); 
    }

    //draw map option
    for (AnimatedObject &map:gd.md.map_previews_){
        map.draw(state, gd, static_cast<float>(map.texture->w), static_cast<float>(map.texture->h));
    }
    if(!gd.isGrandPrix){
        for (AnimatedObject &map:gd.md.map_previews_text_){
            map.draw(state, gd, static_cast<float>(map.texture->w), static_cast<float>(map.texture->h));
        }
    }

    //draw arrows
    for (AnimatedObject &arrow:gd.md.arrows_){
        if(arrow.visible) {
            arrow.draw(state, gd, static_cast<float>(arrow.texture->w), static_cast<float>(arrow.texture->h));
        }
    }

    if(gd.md.settingsBorder.pos.y != 500.f){
        //printf("drawing, %f", gd.settingsBorder->pos.y);
        gd.md.settingsBorder.draw(state, gd,static_cast<float>(gd.md.settingsBorder.texture->w) * 2, static_cast<float>(gd.md.settingsBorder.texture->h)*2);
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
    for (Object &o : gd.md.settingsDials_) {
        o.draw(state, gd, static_cast<float>(o.texture->w) * 2, static_cast<float>(o.texture->h) * 2); 
    }
    SettingsState * curSettings = ((SettingsState *)currState);
    std::vector<std::string> controlStrings = curSettings->controlStrings;
    for (int i = 0; i < controlStrings.size(); i++) {
        SDL_RenderDebugText(state.renderer, curSettings->xStart,
             curSettings->yStart + i * curSettings->yOffset, controlStrings[i].c_str());
    }


    

    if(gd.md.settingsBorder.pos.y != 500.f){
        //printf("drawing, %f", gd.settingsBorder->pos.y);
        gd.md.settingsBorder.draw(state, gd,static_cast<float>(gd.md.settingsBorder.texture->w) * 2, static_cast<float>(gd.md.settingsBorder.texture->h)*2);
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
        for (Object &bg : gd.md.gameplaySettingsBrackets2_) {
            bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
        }
    } else {
        for (Object &bg : gd.md.gameplaySettingsBrackets1_) {
            bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
        }
    }
    for (Object &bg : gd.md.gameplaySettingsNumLaps_) {
        bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
    }

    if(gd.md.settingsBorder.pos.y != 500.f){
        //printf("drawing, %f", gd.settingsBorder->pos.y);
        gd.md.settingsBorder.draw(state, gd,static_cast<float>(gd.md.settingsBorder.texture->w) * 2, static_cast<float>(gd.md.settingsBorder.texture->h)*2);
    }

    //draw arrows
    for (AnimatedObject &arrow:gd.md.arrows_){
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
    if(gd.md.settingsBorder.pos.y != 500.f){
        //printf("drawing, %f", gd.settingsBorder->pos.y);
        gd.md.settingsBorder.draw(state, gd,static_cast<float>(gd.md.settingsBorder.texture->w) * 2, static_cast<float>(gd.md.settingsBorder.texture->h)*2);
    }

    //draw username
    //std::string username = "temp";
    SDL_Color color = {0, 239, 255, 255};
    if(gd.md.displayName != "") {
        SDL_Surface* textSurface = TTF_RenderText_Blended(gd.md.font, gd.md.displayName.c_str(), gd.md.displayName.length(), color);
        SDL_Texture* textTex = SDL_CreateTextureFromSurface(state.renderer, textSurface);
        SDL_SetTextureScaleMode(textTex, SDL_SCALEMODE_NEAREST);
        SDL_FRect rect = { 370, 175, (float)textSurface->w, (float)textSurface->h };

        SDL_RenderTexture(state.renderer, textTex, nullptr, &rect);
        SDL_DestroyTexture(textTex);
        SDL_DestroySurface(textSurface);
    }

    handleMousePointerTitle(state, gd, res, deltaTime);
}