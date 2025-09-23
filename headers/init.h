#pragma once

#include <stdio.h>
#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "gameobject.h"

struct SDLState
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height, logW, logH;
    const bool *keys;
    SDLState() : keys(SDL_GetKeyboardState(nullptr)) {

    }
};

inline bool run = true;

const size_t LAYER_IDX_LEVEL = 0;
const size_t LAYER_IDX_CHARACTERS = 1;

const int MAP_ROWS = 40;
const int MAP_COLS = 140;
const int TILE_SIZE = 32;

struct GameState {
    std::array<std::vector<GameObject>, 2> layers;
    std::vector<GameObject> bgTiles;
    std::vector<GameObject> fgTiles;
    std::vector<GameObject> bullets;
    int playerIndex;
    SDL_FRect mapViewport;
    float bg2Scroll, bg3Scroll, bg4Scroll;
    bool debugMode;
    glm::vec2 ExitPortal;
    std::vector<GameObject> lasers;

    GameState(const SDLState &state) {
        playerIndex = -1; // will change when map is loaded
        mapViewport = SDL_FRect {
            .x = 0,
            .y = 0,
            .w = static_cast<float>(state.logW),
            .h = static_cast<float>(state.logH)
        };
        bg2Scroll = bg3Scroll = bg4Scroll = 0;
        debugMode = false;
    }
    GameObject &player() {
        return layers[LAYER_IDX_CHARACTERS][playerIndex];
    }
};

struct Resources {
    const int ANIM_PLAYER_IDLE = 0;
    const int ANIM_PLAYER_RUN = 1;
    const int ANIM_PLAYER_SLIDE = 2;
    const int ANIM_PLAYER_SHOOT = 3;
    const int ANIM_PLAYER_JUMP = 4;
    const int ANIM_PLAYER_DIE = 5;
    std::vector<Animation> playerAnims;
    const int ANIM_BULLET_MOVING = 0;
    const int ANIM_BULLET_HIT = 1;
    std::vector<Animation> bulletAnims;
    const int ANIM_ENEMY = 0;
    const int ANIM_ENEMY_DEAD = 1;
    std::vector<Animation> enemyAnims;
    const int PORTAL_IDLE = 0;
    std::vector<Animation> portalAnims;
    

    std::vector<SDL_Texture *> textures;
    SDL_Texture *texIdle, *texRun, *texJump, *texSlide, *texShoot, *texDie, 
                *texGrass, *texStone, *texBrick, *texFence, *texBush, 
                *texBullet, *texBulletHit, *texSpiny, *texSpinyDead,
                *texBg1, *texBg2, *texBg3, *texBg4, *texOnStage, *texOffStage, *texBg5, 
                *texLPortal, *texRPortal, *texLaser;

    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filepath) { // load texture from filepath
        // load game assets
        SDL_Texture *tex = IMG_LoadTexture(renderer, filepath.c_str());
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST); // pixel perfect
        textures.push_back(tex);
        return tex;
    }

    void load(SDLState &state, bool real) {
        playerAnims.resize(6); // 
        playerAnims[ANIM_PLAYER_IDLE] = Animation(1, 1.6f); // 1 frames, 1.6 seconds
        playerAnims[ANIM_PLAYER_RUN] = Animation(3, 0.3f);
        playerAnims[ANIM_PLAYER_SLIDE] = Animation(1, 1.0f);
        playerAnims[ANIM_PLAYER_SHOOT] = Animation(1, 0.3f);
        playerAnims[ANIM_PLAYER_JUMP] = Animation(1, 1.0f); 
        playerAnims[ANIM_PLAYER_DIE] = Animation(1, 1.0f);
        bulletAnims.resize(2); // 
        bulletAnims[ANIM_BULLET_MOVING] = Animation(4, 0.5f);
        bulletAnims[ANIM_BULLET_HIT] = Animation(3, 0.5f);
        enemyAnims.resize(2);
        enemyAnims[ANIM_ENEMY] = Animation(2, 0.6f);
        enemyAnims[ANIM_ENEMY_DEAD] = Animation(1, 1.0f);
        portalAnims.resize(2);
        portalAnims[PORTAL_IDLE] = Animation(3, 1.0f);

        if (real) {
            texIdle = loadTexture(state.renderer, "data/IdleL.png");
            texRun = loadTexture(state.renderer, "data/WalkLRL.png");
            texJump = loadTexture(state.renderer, "data/JumpL.png");
            texSlide = loadTexture(state.renderer, "data/SlideL.png");
            texShoot = loadTexture(state.renderer, "data/ShootL.png");
            texDie = loadTexture(state.renderer, "data/DieL.png");
            texBullet = loadTexture(state.renderer, "data/fireballL.png");
            texBulletHit = loadTexture(state.renderer, "data/fireballHitL.png");
        } else {
            texIdle = loadTexture(state.renderer, "data/IdleM.png");
            texRun = loadTexture(state.renderer, "data/WalkLRM.png");
            texJump = loadTexture(state.renderer, "data/JumpM.png");
            texSlide = loadTexture(state.renderer, "data/SlideM.png");
            texShoot = loadTexture(state.renderer, "data/ShootM.png");
            texDie = loadTexture(state.renderer, "data/DieM.png");
            texBullet = loadTexture(state.renderer, "data/fireballM.png");
            texBulletHit = loadTexture(state.renderer, "data/fireballHitM.png");
        }
        texGrass = loadTexture(state.renderer, "data/grass.png");
        texBrick = loadTexture(state.renderer, "data/brick.png");
        texStone = loadTexture(state.renderer, "data/stone.png");
        texBush = loadTexture(state.renderer, "data/bush.png");
        texFence = loadTexture(state.renderer, "data/fence.png");
        texBg1 = loadTexture(state.renderer, "data/Spaceship/Background.png");
        texBg2 = loadTexture(state.renderer, "data/bg_layer2.png");
        texBg3 = loadTexture(state.renderer, "data/bg_layer3.png");
        texBg4 = loadTexture(state.renderer, "data/bg_layer4.png");
        texSpiny = loadTexture(state.renderer, "data/Spiny.png");
        texSpinyDead = loadTexture(state.renderer, "data/SpinyDead.png");

        //Spaceship
        texOnStage = loadTexture(state.renderer, "data/spaceship/OnStage.png");
        texOffStage = loadTexture(state.renderer, "data/spaceship/OffStage.png");
        texBg5 = loadTexture(state.renderer, "data/spaceship/Bg5.png");
        texLPortal = loadTexture(state.renderer, "data/fence.png");
        texRPortal = loadTexture(state.renderer, "data/spaceship/RPortal.png");
        texLaser = loadTexture(state.renderer, "data/spaceship/Laser.png");

    }

    void unload() {
        for (SDL_Texture *tex : textures) {
            SDL_DestroyTexture(tex);
        }
    }
};

bool initialize(SDLState &state);
void cleanup(SDLState &state);
