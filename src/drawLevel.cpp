#include "../headers/drawLevel.h"
#include "../headers/helper.h"
#include "../headers/state.h"

//Draw Function for level Spaceship
void drawLevel(const SDLState &state, GameData &gd, Resources res, float deltaTime){
    // used for camera system
    gd.mapViewport.x = (gd.players_[gd.playerIndex].pos.x + TILE_SIZE / 2) - (gd.mapViewport.w / 2); 
    gd.mapViewport.y = (gd.players_[gd.playerIndex].pos.y + TILE_SIZE / 2) - (gd.mapViewport.h / 2); 
    //draw bg
    SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
    SDL_RenderClear(state.renderer);

    // draw bg tiles
    for (BackgroundObject &bg : gd.bgTiles_) {
        bg.draw(state, gd, static_cast<float>(bg.texture->w), static_cast<float>(bg.texture->h)); 
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



    gd.itemStorage_.draw(state, gd, 68, 68);

    gd.minimap.draw(state, gd, gd.minimap.texture->w, gd.minimap.texture->h);
    //dont need unless debugging checkpoint system and need to see checkpoints
    //renderCheckpoints(state.renderer, courseCheckpoints, gd);

    handleCrosshair(state, gd, res, deltaTime);
    
}