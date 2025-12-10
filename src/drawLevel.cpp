#include "../headers/drawLevel.h"
#include "../headers/helper.h"
#include "../headers/state.h"

extern GameState *currState;

//Draw Function for level Spaceship
void drawLevel(const SDLState &state, GameData &gd, Resources res, float deltaTime){
    // used for camera system
    gd.mapViewport.x = (gd.players_[gd.playerIndex].pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    gd.mapViewport.y = (gd.players_[gd.playerIndex].pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 
    //draw bg, switch to different color for snow level
    if(currState->currStateVal!=SNOW){
        SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
    } else {
        SDL_SetRenderDrawColor(state.renderer, 199, 255, 245, 255);
    }
    SDL_RenderClear(state.renderer);
    

    

    // draw bg tiles
    for (BackgroundObject &bg : gd.bgTiles_) {
        bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
    }

    //draw wire
    for (Wire &wire : gd.wire_) {
        //printf("REAAAHHHHH");
        wire.draw(state, gd, static_cast<float>(wire.texture->w), static_cast<float>(wire.texture->h)); 
    }

    std::vector<Object *> onscreenTiles_ = getOnscreenTiles(state, gd);
    for (Object* o : onscreenTiles_) {
        switch (o->type) {
            case LEVEL: 
            {
                Level& l = dynamic_cast<Level&>((*o));
                l.draw(state, gd, TILE_SIZE, TILE_SIZE);
                break;
            }
            case PORTAL:
            {
                Portal& po = dynamic_cast<Portal&>((*o));
                po.draw(state, gd, TILE_SIZE, TILE_SIZE * 2);
                break;
            }
            case LASER:
            {
                Laser& la = dynamic_cast<Laser&>((*o));
                if (la.laserActive) {
                    la.draw(state, gd, TILE_SIZE, TILE_SIZE);
                }
                break;
            }
            case SIGN:
            {
                Sign& s = dynamic_cast<Sign&>((*o));
                s.draw(state, gd, TILE_SIZE, TILE_SIZE);
                break;
            }
            case LAVA:
            {
                Lava& l = dynamic_cast<Lava&>((*o));
                l.draw(state, gd, TILE_SIZE, TILE_SIZE);
                break;
            }
            case WATER:
            {
                Water& w = dynamic_cast<Water&>((*o));
                w.draw(state, gd, TILE_SIZE, TILE_SIZE);
                break;
            }
            case ICE_BLOCK:
            {
                Level& l = dynamic_cast<Level&>((*o));
                l.draw(state, gd, TILE_SIZE, TILE_SIZE);
                break;
            }
            case CACTUS:
            {
                Level& l = dynamic_cast<Level&>((*o));
                l.draw(state, gd, TILE_SIZE, TILE_SIZE);
                break;
            }
            case REVOLVER:
            {
                Revolver& r = dynamic_cast<Revolver&>((*o));
                r.draw(state, gd, TILE_SIZE * 2, TILE_SIZE * 2);
                break;
            }
            
            case ITEMBOX:
            {
                ItemBox& box = dynamic_cast<ItemBox&>((*o));
                if (box.itemBoxActive) {
                    box.draw(state, gd, TILE_SIZE, TILE_SIZE);
                }
                break;
            }
        }
    }

    // draw players
    for(Player &p : gd.players_) {
        p.draw(state, gd, TILE_SIZE, TILE_SIZE);
        p.state_->draw(state, gd); // currently only for shotgun but could be used in future for state based particles/power up effects
        std::vector<Object *> closeTiles_ = getCloseTiles(state, gd, p.pos);
        for (Object* o : closeTiles_) {
            o->drawDebugNearby(state, gd, o->texture->w, o->texture->h);
        }
    }

    for(auto &i : gd.players_[gd.playerIndex].items_){
        if(i->type == FOG){
            i->draw(state, gd, i->width, i->height);
        }
    }
    //draw player's placement
    SDL_Color color = {0, 239, 255, 255};
    if(gd.players_[gd.playerIndex].position != -1) {
        std::string place = std::to_string(gd.players_[gd.playerIndex].position);
        switch(gd.players_[gd.playerIndex].position) {
                case 1:
                    place.append("st");
                    break;
                case 2:
                    place.append("nd");
                    break;
                case 3:
                    place.append("rd");
                    break;
                default:
                    place.append("th");
                    break;
        }
        
        SDL_Surface* textSurface = TTF_RenderText_Blended(gd.md.font, place.c_str(), place.length(), color);
        SDL_Texture* textTex = SDL_CreateTextureFromSurface(state.renderer, textSurface);
        SDL_SetTextureScaleMode(textTex, SDL_SCALEMODE_NEAREST);
        //change x any position to reflect viewport - should go in the bottom right
        // this centers the minimap's top right corner to the top right corner of the screen, allowing for minimaps of any size to work
        float xpos = (gd.players_[gd.playerIndex].pos.x + TILE_SIZE / 2) + (gd.mapViewport.w / 2) - gd.mapViewport.x; 
        float ypos = (gd.players_[gd.playerIndex].pos.y + TILE_SIZE / 2) + (gd.mapViewport.h / 2) - gd.mapViewport.y;
        SDL_FRect rect = { xpos - (float)textSurface->w, ypos - (float)textSurface->h, (float)textSurface->w, (float)textSurface->h };

        SDL_RenderTexture(state.renderer, textTex, nullptr, &rect);
        SDL_DestroyTexture(textTex);
        SDL_DestroySurface(textSurface);
    }


    gd.itemStorage_.draw(state, gd, 68, 68);

    

    gd.minimap.draw(state, gd, gd.minimap.texture->w, gd.minimap.texture->h);
    //dont need unless debugging checkpoint system and need to see checkpoints
    //renderCheckpoints(state.renderer, courseCheckpoints, gd);

    handleCrosshair(state, gd, res, deltaTime);
    
}