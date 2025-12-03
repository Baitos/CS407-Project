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

    if(gd.md.border.pos.y != 500.f){
        //printf("drawing, %f", gd.border->pos.y);
        gd.md.border.draw(state, gd,static_cast<float>(gd.md.border.texture->w) * 2, static_cast<float>(gd.md.border.texture->h)*2);
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
    if(gd.md.border.pos.y != 500.f){
        //printf("drawing, %f", gd.border->pos.y);
        gd.md.border.draw(state, gd,static_cast<float>(gd.md.border.texture->w) * 2, static_cast<float>(gd.md.border.texture->h)*2);
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

    if(gd.md.border.pos.y != 500.f){
        //printf("drawing, %f", gd.border->pos.y);
        gd.md.border.draw(state, gd,static_cast<float>(gd.md.border.texture->w) * 2, static_cast<float>(gd.md.border.texture->h)*2);
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
    if(gd.md.border.pos.y != 500.f){
        //printf("drawing, %f", gd.border->pos.y);
        gd.md.border.draw(state, gd,static_cast<float>(gd.md.border.texture->w) * 2, static_cast<float>(gd.md.border.texture->h)*2);
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
void drawHostLobby(const SDLState &state, GameData &gd, Resources res, float deltaTime){
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
    if(gd.md.border.pos.y != 500.f){
        //printf("drawing, %f", gd.border->pos.y);
        gd.md.border.draw(state, gd,static_cast<float>(gd.md.border.texture->w) * 2, static_cast<float>(gd.md.border.texture->h)*2);
    }

    //draw username
    //std::string username = "temp";
    SDL_Color color = {0, 239, 255, 255};
    if(gd.md.password != "") {
        SDL_Surface* textSurface = TTF_RenderText_Blended(gd.md.font, gd.md.password.c_str(), gd.md.password.length(), color);
        SDL_Texture* textTex = SDL_CreateTextureFromSurface(state.renderer, textSurface);
        SDL_SetTextureScaleMode(textTex, SDL_SCALEMODE_NEAREST);
        SDL_FRect rect = { 350, 262, (float)textSurface->w, (float)textSurface->h };

        SDL_RenderTexture(state.renderer, textTex, nullptr, &rect);
        SDL_DestroyTexture(textTex);
        SDL_DestroySurface(textSurface);
    }
    handleMousePointerHostLobby(state, gd, res, deltaTime);
}

void drawResults(const SDLState &state, GameData &gd, Resources res, float deltaTime){
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

    //render header
    SDL_Color white = { 255, 255, 255, 255 };
    int w = 400 ,h = 30;
    SDL_Texture* header = createTextTexture(state.renderer, gd.md.font, (gd.rd->resultsPhase == ResultData::RESULTS_SHOW_ROUND ? "Round Results" : "Cumulative Standings"), white, w, h);
    if (header) {
        SDL_FRect dst = { 400 - w/2.0f, 20.0f, static_cast<float>(w), static_cast<float>(h) };
        SDL_RenderTexture(state.renderer, header, nullptr, &dst);
        SDL_DestroyTexture(header);
    }

    //pick which list to render
    std::vector<ResultData::ResultEntry> &list = (gd.rd->resultsPhase == ResultData::RESULTS_SHOW_ROUND) ? gd.rd->roundResults : gd.rd->cumulativeResults;

    //draw rows
    float startX = 150.0f;
    float startY = 60.0f;
    float rowH = 30.0f;
    float iconSize = 30.0f;

    //for each entry/player
    for(int i=0; i<list.size(); i++) {
        ResultData::ResultEntry &entry = list[i];
        Player* p = entry.player;
        float y = startY + i * (rowH + 8.0f);

        //number
        std::string placeStr = std::to_string(entry.placement);
        SDL_Texture* texPlace = createTextTexture(state.renderer, gd.md.font, placeStr, white, w, h);
        if(texPlace) {
            SDL_FRect dst = {startX, y + (rowH-h) / 2.0f, static_cast<float>(w), static_cast<float>(h)};
            SDL_RenderTexture(state.renderer, texPlace, nullptr, &dst);
            SDL_DestroyTexture(texPlace);
        }

        //username
        std::string uname = p->username;
        SDL_Texture* texName = createTextTexture(state.renderer, gd.md.font, uname, white, w, h);
        if(texName) {
            SDL_FRect dst = {startX + 60.0f, y+(rowH-h) / 2.0f, static_cast<float>(w), static_cast<float>(h)};
            SDL_RenderTexture(state.renderer, texName, nullptr, &dst);
            SDL_DestroyTexture(texName);
        }

        //icon
        SDL_Texture* icon = res.texIdle[p->character];
        if(icon) {
            SDL_FRect dst = {startX + 200.0f, y+(rowH-iconSize)/2.0f, iconSize, iconSize};
            SDL_FRect src = {0,0,static_cast<float>(icon->w), static_cast<float>(icon->h)};
            SDL_RenderTexture(state.renderer, icon, &src, &dst);
        }

        //pts
        std::string ptsString;
        if(gd.rd->resultsPhase == ResultData::RESULTS_SHOW_ROUND) {
            ptsString = "+" + std::to_string(entry.pointsEarned) + " pts";
        } else {
            ptsString = std::to_string(entry.pointsEarned) + " pts";
        }
        SDL_Texture* texPoints = createTextTexture(state.renderer, gd.md.font, ptsString, white, w, h);
        if(texPoints) {
            SDL_FRect dst = {500.0f, y+(rowH-h)/2.0f, static_cast<float>(w), static_cast<float>(h)};
            SDL_RenderTexture(state.renderer, texPoints, nullptr, &dst);
            SDL_DestroyTexture(texPoints);
        }
    }

    //handleMousePointerTitle(state, gd, res, deltaTime);
    handleMousePointerResults(state,gd,res,deltaTime);
}

void drawJoinLobby(const SDLState &state, GameData &gd, Resources res, float deltaTime){
    gd.mapViewport.x = 0; 
    gd.mapViewport.y = 0; 
    //draw bg
    SDL_SetRenderDrawColor(state.renderer, 13, 22, 59, 255);
    SDL_RenderClear(state.renderer);

    // draw bg tiles
    for (BackgroundObject &bg : gd.bgTiles_) {
        bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
    }
    // used for camera system
    if(gd.md.border.pos.y != 500.f){
        //printf("drawing, %f", gd.border->pos.y);
        gd.md.border.draw(state, gd,static_cast<float>(gd.md.border.texture->w) * 2, static_cast<float>(gd.md.border.texture->h)*2);
    }
    if (gd.md.verticalDial != nullptr) {
        gd.md.verticalDial->draw(state, gd, static_cast<float>(gd.md.verticalDial->texture->w), static_cast<float>(gd.md.verticalDial->texture->h));
    }
    if (gd.md.lobbySelectBorder != nullptr && gd.md.lobbySelectBorder->visible) {
        gd.md.lobbySelectBorder->draw(state, gd, static_cast<float>(gd.md.lobbySelectBorder->texture->w), static_cast<float>(gd.md.lobbySelectBorder->texture->h));
    }
    //draw username
    //std::string username = "temp";
    SDL_Color color = {0, 239, 255, 255};
        if(gd.md.password != "") {
        SDL_Surface* textSurface = TTF_RenderText_Blended(gd.md.font, gd.md.password.c_str(), gd.md.password.length(), color);
        SDL_Texture* textTex = SDL_CreateTextureFromSurface(state.renderer, textSurface);
        SDL_SetTextureScaleMode(textTex, SDL_SCALEMODE_NEAREST);
        SDL_FRect rect = { 320, 22, (float)textSurface->w, (float)textSurface->h };

        SDL_RenderTexture(state.renderer, textTex, nullptr, &rect);
        SDL_DestroyTexture(textTex);
        SDL_DestroySurface(textSurface);
    }
    Lobby lobby;
    std::vector<Lobby *> lobbies = gd.md.isPrivate ? gd.md.privateLobbies_ : gd.md.publicLobbies_;
    std::string lobbyCapacity;
    const char * lobbyName;
    for (int i = gd.md.startLobbyIndex; (i < gd.md.startLobbyIndex + 7 && (i < lobbies.size())); i++) {
        // TODO name lobby with number if hostName is empty
        lobbyCapacity = std::to_string(lobbies[i]->playerCount) + " / 8)";
        lobbyName = (lobbies[i]->hostName + std::string("'s Lobby (") + lobbyCapacity).c_str();
        SDL_Surface* textSurface = TTF_RenderText_Blended(gd.md.font, lobbyName, strlen(lobbyName), color);
        SDL_Texture* textTex = SDL_CreateTextureFromSurface(state.renderer, textSurface);
        SDL_SetTextureScaleMode(textTex, SDL_SCALEMODE_NEAREST);
        SDL_FRect rect = { 70, 72 + 38*(float)(i - gd.md.startLobbyIndex), (float)textSurface->w, (float)textSurface->h};
        SDL_RenderTexture(state.renderer, textTex, nullptr, &rect);
        SDL_DestroyTexture(textTex);
        SDL_DestroySurface(textSurface);
    }
    handleMousePointerJoinLobby(state, gd, res, deltaTime);
}
 
void drawEndResults(const SDLState &state, GameData &gd, Resources res, float deltaTime){
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

    //render header
    SDL_Color white = { 255, 255, 255, 255 };
    int w = 400 ,h = 30;
    SDL_Texture* header = createTextTexture(state.renderer, gd.md.font, "Final Results", white, w, h);
    if (header) {
        SDL_FRect dst = { 400 - w/2.0f, 20.0f, static_cast<float>(w), static_cast<float>(h) };
        SDL_RenderTexture(state.renderer, header, nullptr, &dst);
        SDL_DestroyTexture(header);
    }

    //pick which list to render
    std::vector<ResultData::ResultEntry> &list =  gd.rd->cumulativeResults;

    //draw rows
    float startX = 150.0f;
    float startY = 60.0f;
    float rowH = 30.0f;
    float iconSize = 30.0f;

    //for each entry/player
    for(int i=0; i<list.size(); i++) {
        ResultData::ResultEntry &entry = list[i];
        Player* p = entry.player;
        float y = startY + i * (rowH + 8.0f);

        //number
        std::string placeStr = std::to_string(entry.placement);
        SDL_Texture* texPlace = createTextTexture(state.renderer, gd.md.font, placeStr, white, w, h);
        if(texPlace) {
            SDL_FRect dst = {startX, y + (rowH-h) / 2.0f, static_cast<float>(w), static_cast<float>(h)};
            SDL_RenderTexture(state.renderer, texPlace, nullptr, &dst);
            SDL_DestroyTexture(texPlace);
        }

        //username
        std::string uname = p->username;
        SDL_Texture* texName = createTextTexture(state.renderer, gd.md.font, uname, white, w, h);
        if(texName) {
            SDL_FRect dst = {startX + 60.0f, y+(rowH-h) / 2.0f, static_cast<float>(w), static_cast<float>(h)};
            SDL_RenderTexture(state.renderer, texName, nullptr, &dst);
            SDL_DestroyTexture(texName);
        }

        //icon
        SDL_Texture* icon = res.texIdle[p->character];
        if(icon) {
            SDL_FRect dst = {startX + 200.0f, y+(rowH-iconSize)/2.0f, iconSize, iconSize};
            SDL_FRect src = {0,0,static_cast<float>(icon->w), static_cast<float>(icon->h)};
            SDL_RenderTexture(state.renderer, icon, &src, &dst);
        }

        //pts
        std::string ptsString;
        ptsString = std::to_string(entry.pointsEarned) + " pts";
        SDL_Texture* texPoints = createTextTexture(state.renderer, gd.md.font, ptsString, white, w, h);
        if(texPoints) {
            SDL_FRect dst = {500.0f, y+(rowH-h)/2.0f, static_cast<float>(w), static_cast<float>(h)};
            SDL_RenderTexture(state.renderer, texPoints, nullptr, &dst);
            SDL_DestroyTexture(texPoints);
        }
    }

    handleMousePointerEndResults(state,gd,res,deltaTime);
}