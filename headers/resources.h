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
    const int ANIM_PLAYER_JETPACK = 4;
    const int ANIM_PLAYER_JUMP = 5;
    const int ANIM_PLAYER_DIE = 6;
    const int ANIM_PLAYER_SLIDE = 7;
    const int ANIM_PLAYER_LAUNCH = 8;
    const int ANIM_PLAYER_SHOOT_JUMP = 9;
    const int ANIM_PLAYER_ROLL = 10;
    const int ANIM_PLAYER_JETPACK_DEPLOY = 11;
    const int ANIM_PLAYER_SWORD_DEPLOY = 12;
    std::vector<Animation> playerAnims;
    const int SHOTGUN_BLAST = 0;
    std::vector<Animation> shotgunAnims;

    

    const int ANIM_ITEM_EMPTY = 0;
    const int ANIM_ITEM_CYCLE = 1;
    const int ANIM_ITEM_PICKED = 2;
    std::vector<Animation> itemAnims;
    std::vector<SDL_Texture *> itemTextures;
    const int PORTAL_IDLE = 0;
    std::vector<Animation> portalAnims;
    const int ICONS = 0;
    std::vector<Animation> charIconAnims;
    const int ERROR = 0;
    std::vector<Animation> errorAnims;

    const int MAP = 0;
    const int DOTS = 1;
    std::vector<Animation> minimapAnims;
    
    //anim for map select
    const int MAP_GRASSLAND = 0;
    const int MAP_SPACESHIP = 1;
    const int MAP_3 = 2;
    const int MAP_4 = 3;
    const int MAP_5 = 4;
    std::vector<Animation> mapAnims;
    const int MAP_GRASSLAND_TEXT = 0;
    const int MAP_SPACESHIP_TEXT = 1;
    const int MAP_3_TEXT = 2;
    const int MAP_4_TEXT = 3;
    const int MAP_5_TEXT = 4;
    const int MAP_GRAND_PRIX = 5;
    std::vector<Animation> mapTextAnims;
    const int LEFT_ARROW = 0;
    const int RIGHT_ARROW = 1;
    std::vector<Animation> arrowAnims;

    //FOR NOW.. change to 0 for shotgun, 1 for sword, 2 for jetpack
    int character;

    SDL_Texture *texIdle[3];
    SDL_Texture *texRun[3];
    SDL_Texture *texJump[3];
    SDL_Texture *texLaunch[3];
    SDL_Texture *texSlide[3];
    SDL_Texture *texShoot[3];
    SDL_Texture *texDie[3];
    SDL_Texture *texShootJump[3];
    SDL_Texture *texRoll[3];
    SDL_Texture *texDeploy[3];
    SDL_Texture *texMapPreviews[6];
    SDL_Texture *texMapTextPreviews[5];
    std::vector<SDL_Texture *> textures;
    SDL_Texture *texCrosshair, 
                // *texIdleS, *texRunS, *texJumpS, *texLaunchS, *texSlideS, *texShootS, *texDieS, *texShootJumpS, *texRollS,
                // *texIdleG, *texRunG, *texJumpG, *texLaunchG, *texSlideG, *texShootG, *texDieG, *texShootJumpG, *texRollG,
                // *texIdleJ, *texRunJ, *texJumpJ, *texLaunchJ, *texSlideJ, *texShootJ, *texDieJ, *texShootJumpJ, *texRollJ,
                // *texJDeploy, *texGDeploy, *texSDeploy,
                *texShotgunBlast,
                *texGrapple, *texGrappleAngle,
                *texBullet, *texBulletHit,
                *texBg1, *texBg2, *texBg3, *texBg4, *texOnStage, *texOffStage, *texBg5, 
                *texLPortal, *texRPortal, *texLaser, 
                *texFloor, *texTRCorner, *texRWall, *texBRCorner, *texLWall, *texBLCorner, *texTLCorner, *texCeiling, *texITLCorner, *texITRCorner, *texIBRCorner, *texIBLCorner, *texPlatform, 
                *texFan, *texPanelOne, *texPanelTwo, *texPanelThree, *texVentOne, *texVentTwo, *texBackWall,
                *texCloud, *texWood, *texLeaves, *texStone, *texWater, *texLava, *texGrass, *texDirt,
                *texBgWood, *texBgSky, *texBgStone,
                *texSignUp, *texSignDown, *texSignRight, *texSignLeft,
                *texCharSelectBackground, *texCursor, *texCharIcons, *texSword, *texShotgun, *texJetpack, 
                *texLeftArrow, *texRightArrow,
                *texItemBox, *texItemStorage, *texItemRandomizer, 
                *texBombStorage, *texBoomboxStorage, *texBouncyBallStorage, *texFogStorage,
                *texIceStorage, *texMissileStorage, *texSugarStorage, *texPieStorage,
                *texBomb, *texExplosion, *texBoombox, *texSoundwaves, *texSugar,
                *texSugarEffectL, *texSugarEffectR,
                *texSettingsBackground, *texBigBorder, *texSmallBorder, *texSlider,       
                *texTitle, *texTextCursor,
                *texGameplaySettingsBackground, *texGameplaySettingsLeftBracket, *texGameplaySettingsRightBracket,
                *texGameplaySettings1,*texGameplaySettings2,*texGameplaySettings3,*texGameplaySettings4,*texGameplaySettings5,
                *texMinimap;
                SDL_Texture *texPlayerDots[3];  
    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filepath);
    void load(SDLState &state);
    void unload();

    Resources(){};
};