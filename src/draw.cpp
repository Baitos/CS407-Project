#include "../headers/draw.h"
#include "../headers/initState.h"
#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/player.h"
#include "../headers/globals.h"
#include "../headers/helper.h"

void drawObject(const SDLState &state, GameState &gs, GameObject &obj, float width, float height, float deltaTime) {
        if (!isOnscreen(state, gs, obj)) {
            return;  
        }
        float srcX = obj.curAnimation != -1 ? obj.animations[obj.curAnimation].currentFrame() * width : (obj.spriteFrame - 1) * width;
        SDL_FRect src {
            .x = srcX,
            .y = 0,
            .w = width,
            .h = height
        };

        SDL_FRect dst {
            .x = obj.pos.x - gs.mapViewport.x,
            .y = obj.pos.y - gs.mapViewport.y,
            .w = width,
            .h = height
        };
        SDL_FlipMode flipMode; // = obj.dir == -1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        if (obj.dir == -1) {            
            if (obj.flip == -1) {
                flipMode = (SDL_FlipMode)3; // FLIP_VERTICAL_AND_HORIZONTAL
            } else {
                flipMode = SDL_FLIP_HORIZONTAL;
            }
        } else {
            if (obj.flip == -1) {
                flipMode = SDL_FLIP_VERTICAL;
            } else {
                flipMode = SDL_FLIP_NONE;
            }
        }
        if (!obj.shouldFlash) {
            SDL_RenderTextureRotated(state.renderer, obj.texture, &src, &dst, 0, nullptr, flipMode); // src is for sprite stripping, dest is for where sprite should be drawn
        } else {
            // flash with white tint
            SDL_SetTextureColorModFloat(obj.texture, 2.5f, 2.5f, 2.5f);  
            SDL_RenderTextureRotated(state.renderer, obj.texture, &src, &dst, 0, nullptr, flipMode);
            SDL_SetTextureColorModFloat(obj.texture, 1.0f, 1.0f, 1.0f);
            if (obj.flashTimer.step(deltaTime)) {
                obj.shouldFlash = false;
            }
        }


        if (gs.debugMode) {
            SDL_FRect rectA {
                .x = obj.pos.x + obj.collider.x - gs.mapViewport.x, 
                .y = obj.pos.y + obj.collider.y - gs.mapViewport.y,
                .w = obj.collider.w, 
                .h = obj.collider.h
            };
            SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_BLEND);

            SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 150);
            SDL_RenderFillRect(state.renderer, &rectA);
            SDL_FRect sensor{  
			    .x = obj.pos.x + obj.collider.x - gs.mapViewport.x,
			    .y = obj.pos.y + obj.collider.y + obj.collider.h - gs.mapViewport.y,
			    .w = obj.collider.w, 
                .h = 1
		    };
		    SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 150);
		    SDL_RenderFillRect(state.renderer, &sensor);

            SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_NONE);
        }
}

void drawLevel(const SDLState &state, GameState &gs, Level &level, float width, float height, float deltaTime) {
    SDL_FRect src {
        .x = 0,
        .y = 0,
        .w = width,
        .h = height
    };

    SDL_FRect dst {
        .x = level.pos.x - gs.mapViewport.x,
        .y = level.pos.y - gs.mapViewport.y,
        .w = width,
        .h = height
    };
    SDL_RenderTextureRotated(state.renderer, level.texture, &src, &dst, 0, nullptr, SDL_FLIP_NONE);
}