#pragma once

#include <SDL3/SDL.h> // don't include SDL3_main in anything other than the main cpp file
#include <vector>
#include <string>
#include "../ext/glm/glm.hpp"
#include "animation.h"

struct SDLState; // forward def

struct Resources {
    const int ANIM_PLAYER_IDLE = 0;
    const int ANIM_PLAYER_WALK = 1;
    const int ANIM_PLAYER_RUN = 2;
    const int ANIM_PLAYER_SHOOT = 3;
    const int ANIM_PLAYER_JUMP = 4;
    const int ANIM_PLAYER_DIE = 5;
    const int ANIM_PLAYER_SLIDE = 6;
    const int ANIM_PLAYER_LAUNCH = 7;
    const int ANIM_PLAYER_SHOOT_JUMP = 8;
    const int ANIM_PLAYER_ROLL = 9;
    std::vector<Animation> playerAnims;
    const int ANIM_BULLET_MOVING = 0;
    const int ANIM_BULLET_HIT = 1;
    std::vector<Animation> bulletAnims;
    const int ANIM_ENEMY = 0;
    const int ANIM_ENEMY_DEAD = 1;
    std::vector<Animation> enemyAnims;
    const int PORTAL_IDLE = 0;
    std::vector<Animation> portalAnims;
    const int ICONS = 0;
    std::vector<Animation> charIconAnims;

    //FOR NOW.. change to 0 for shotgun, 1 for sword, 2 for jetpack
    int character;
    

    std::vector<SDL_Texture *> textures;
    SDL_Texture *texCrosshair, *texIdle, *texRun, *texJump, *texLaunch, *texSlide, *texShoot, *texDie, *texShootJump, *texRoll,
                *texGrass, *texStone, *texBrick, *texFence, *texBush, 
                *texBullet, *texBulletHit, *texSpiny, *texSpinyDead,
                *texBg1, *texBg2, *texBg3, *texBg4, *texOnStage, *texOffStage, *texBg5, 
                *texLPortal, *texRPortal, *texLaser, 
                *texFloor, *texTRCorner, *texRWall, *texBRCorner, *texLWall, *texBLCorner, *texTLCorner, *texCeiling, *texITLCorner, *texITRCorner, *texIBRCorner, *texIBLCorner, *texPlatform, 
                *texFan, *texPanelOne, *texPanelTwo, *texPanelThree, *texVentOne, *texVentTwo, *texBackWall, 
                *texCharSelectBackground, *texCursor, *texCharIcons, *texSword, *texShotgun, *texJetpack;
    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filepath);
    void load(SDLState &state);
    void unload();

    Resources(){};
};