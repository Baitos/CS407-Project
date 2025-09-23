#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/draw.h"

void drawObject(const SDLState &state, GameState &gs, GameObject &obj, float width, float height, float deltaTime) {
        float srcX = obj.curAnimation != -1 
                     ? obj.animations[obj.curAnimation].currentFrame() * width 
                     : (obj.spriteFrame - 1) * width;
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
        SDL_FlipMode flipMode = obj.dir == -1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
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
			    .y = obj.pos.y + obj.collider.y + obj.collider.h  - gs.mapViewport.y,
			    .w = obj.collider.w, 
                .h = 1
		    };
		    SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 150);
		    SDL_RenderFillRect(state.renderer, &sensor);

            SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_NONE);
        }
}

void drawParallaxBackground(SDL_Renderer *renderer, SDL_Texture *texture, float xVelocity, float &scrollPos, float scrollFactor, float deltaTime) {
    scrollPos -= xVelocity * scrollFactor * deltaTime; // moving background to the left at rate dependent on playerX
    if (scrollPos <= -texture->w) {
        scrollPos = 0;
    }
    SDL_FRect dst {
        .x = scrollPos,
        .y = 200,
        .w = texture->w * 2.0f,
        .h = static_cast<float>(texture->h)
    };
    SDL_RenderTextureTiled(renderer, texture, nullptr, 1, &dst);
}