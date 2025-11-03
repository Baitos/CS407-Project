
#include <SDL3_image/SDL_image.h>
#include "../headers/resources.h"
#include "../headers/initState.h"
#include "../headers/player.h"

SDL_Texture* Resources::loadTexture(SDL_Renderer *renderer, const std::string &filepath) { // load texture from filepath
    // load game assets
    SDL_Texture *tex = IMG_LoadTexture(renderer, filepath.c_str());
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST); // pixel perfect
    textures.push_back(tex);
    return tex;
}

void Resources::load(SDLState &state) { // First variable controls how many frames there are, second is how long each frame lasts (in seconds)
    playerAnims.resize(13); // 
    playerAnims[ANIM_PLAYER_IDLE] = Animation(1, 1.6f);
    playerAnims[ANIM_PLAYER_WALK] = Animation(8, 1.0f);
    playerAnims[ANIM_PLAYER_RUN] = Animation(8, 0.5f);
    playerAnims[ANIM_PLAYER_SLIDE] = Animation(1, 1.0f);
    playerAnims[ANIM_PLAYER_SHOOT] = Animation(1, 2.0f);
    playerAnims[ANIM_PLAYER_JETPACK] = Animation(1, 0.8f);
    playerAnims[ANIM_PLAYER_JUMP] = Animation(1, 0.3f); 
    playerAnims[ANIM_PLAYER_DIE] = Animation(1, 1.0f);
    playerAnims[ANIM_PLAYER_LAUNCH] = Animation(3, 0.2f);
    playerAnims[ANIM_PLAYER_SHOOT_JUMP] = Animation(1, 0.8f);
    playerAnims[ANIM_PLAYER_ROLL] = Animation(5, 0.2f);
    playerAnims[ANIM_PLAYER_JETPACK_DEPLOY] = Animation(1, 2.0f);
    playerAnims[ANIM_PLAYER_SWORD_DEPLOY] = Animation(3, 0.3f);

    itemAnims.resize(6); // 
    itemAnims[ANIM_ITEM_EMPTY] = Animation(1, 1.0f);
    itemAnims[ANIM_ITEM_CYCLE] = Animation(8, 0.4f);
    itemAnims[ANIM_ITEM_PICKED] = Animation(1, 1.0f);
    itemAnims[ANIM_ITEM_EXPLOSION] = Animation(3, 0.5f);
    itemAnims[ANIM_ITEM_SOUNDWAVE] = Animation(9, 0.4f);
    itemAnims[ANIM_ITEM_PIE] = Animation(1, 1.0f);

    portalAnims.resize(2);
    portalAnims[PORTAL_IDLE] = Animation(3, 1.0f);
    charIconAnims.resize(1);
    charIconAnims[ICONS] = Animation(8, 0.0f);
    shotgunAnims.resize(1);
    shotgunAnims[SHOTGUN_BLAST] = Animation(3, 0.3f);

    minimapAnims.resize(2);
    minimapAnims[MAP] = Animation(1, 1.0f);
    minimapAnims[DOTS] = Animation(3, 0.0f);

    errorAnims.resize(1); // default to prevent crashes
    errorAnims[ERROR] = Animation(1, 1.0f);

    mapAnims.resize(6);
    mapAnims[MAP_GRASSLAND] = Animation(1, 1.0f);
    mapAnims[MAP_SPACESHIP] = Animation(1, 1.0f);
    mapAnims[MAP_3] = Animation(1, 1.0f);
    mapAnims[MAP_4] = Animation(1, 1.0f);
    mapAnims[MAP_5] = Animation(1, 1.0f);
    mapAnims[MAP_GRAND_PRIX] = Animation(1, 1.0f);

    mapTextAnims.resize(5);
    mapTextAnims[MAP_GRASSLAND_TEXT] = Animation(1, 1.0f);
    mapTextAnims[MAP_SPACESHIP_TEXT] = Animation(1, 1.0f);
    mapTextAnims[MAP_3_TEXT] = Animation(1, 1.0f);
    mapTextAnims[MAP_4_TEXT] = Animation(1, 1.0f);
    mapTextAnims[MAP_5_TEXT] = Animation(1, 1.0f);

    arrowAnims.resize(2);
    arrowAnims[RIGHT_ARROW] = Animation(1, 1.0f);
    arrowAnims[LEFT_ARROW] = Animation(1, 1.0f);


    //load shotgun character animations
    texIdle[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/idle_shotgun.png");
    texRun[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/running_shotgun.png");
    texJump[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/jumped_shotgun.png");
    texSlide[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/turning_shotgun.png");
    texShoot[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shooting_shotgun.png");
    texDie[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shot_shotgun.png");
    texShootJump[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shooting_jumped_shotgun.png");
    texLaunch[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/jumping_shotgun.png");
    texRoll[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/rolling_shotgun.png");
    
    //load sword character animations
    texIdle[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/idle_sword.png");
    texRun[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/running_sword.png");
    texJump[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/jumped_sword.png");
    texSlide[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/turning_sword.png");
    texShoot[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/swing_sword.png");
    texDie[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/shot_sword.png");
    texShootJump[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/swing_jumped_sword.png");
    texLaunch[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/jumping_sword.png");
    texRoll[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/rolling_sword.png");

    //load jetpack character animations
    texIdle[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/idle_jetpack.png");
    texRun[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/running_jetpack.png");
    texJump[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/jumped_jetpack.png");
    texSlide[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/turning_jetpack.png");
    texShoot[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/on_jetpack.png");
    texDie[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/shot_jetpack.png");
    texShootJump[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/on_jetpack.png");
    texLaunch[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/jumping_jetpack.png");
    texRoll[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/rolling_jetpack.png");

    //load special deploy animations
    texDeploy[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shooting_shotgun.png");
    texDeploy[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/Sword/swing_sword.png");
    texDeploy[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/on_jetpack.png");

    texShotgunBlast = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/fire_shotgun.png");

    texGrapple = loadTexture(state.renderer, "data/CharacterSprites/grapple.png");
    texGrappleAngle = loadTexture(state.renderer, "data/CharacterSprites/grappleAngle.png");

    texCrosshair = loadTexture(state.renderer, "data/crosshair.png");
    texBg1 = loadTexture(state.renderer, "data/Spaceship/Background.png");

    //Spaceship
    texOnStage = loadTexture(state.renderer, "data/spaceship/OnStage.png");
    texOffStage = loadTexture(state.renderer, "data/spaceship/OffStage.png");
    texBg5 = loadTexture(state.renderer, "data/spaceship/Background2.png");
    texLPortal = loadTexture(state.renderer, "data/spaceship/LPortal.png");
    texRPortal = loadTexture(state.renderer, "data/spaceship/RPortal.png");
    texLaser = loadTexture(state.renderer, "data/spaceship/Laser.png");
    texFloor = loadTexture(state.renderer, "data/spaceship/floor.png");
    texTRCorner = loadTexture(state.renderer, "data/spaceship/TRCorner.png");
    texRWall = loadTexture(state.renderer, "data/spaceship/RightWall.png");
    texBRCorner = loadTexture(state.renderer, "data/spaceship/BRCorner.png");
    texLWall = loadTexture(state.renderer, "data/spaceship/LeftWall.png");
    texBLCorner = loadTexture(state.renderer, "data/spaceship/BLCorner.png");
    texTLCorner = loadTexture(state.renderer, "data/spaceship/TLCorner.png");
    texCeiling = loadTexture(state.renderer, "data/spaceship/Ceiling.png");
    texIBLCorner = loadTexture(state.renderer, "data/spaceship/IBLCorner.png");
    texITLCorner = loadTexture(state.renderer, "data/spaceship/ITLCorner.png");
    texITRCorner = loadTexture(state.renderer, "data/spaceship/ITRCorner.png");
    texIBRCorner = loadTexture(state.renderer, "data/spaceship/IBRCorner.png");
    texPlatform = loadTexture(state.renderer, "data/spaceship/platform.png");

    //Spaceship Background
    texFan = loadTexture(state.renderer, "data/spaceship/BackgroundTiles/Fan.png");
    texPanelOne = loadTexture(state.renderer, "data/spaceship/BackgroundTiles/PanelOne.png");
    texPanelTwo = loadTexture(state.renderer, "data/spaceship/BackgroundTiles/PanelTwo.png");
    texPanelThree = loadTexture(state.renderer, "data/spaceship/BackgroundTiles/PanelThree.png");
    texVentOne = loadTexture(state.renderer, "data/spaceship/BackgroundTiles/VentOne.png");
    texVentTwo = loadTexture(state.renderer, "data/spaceship/BackgroundTiles/VentTwo.png");
    texBackWall = loadTexture(state.renderer, "data/spaceship/BackgroundTiles/BackWall.png");


    //Grasslands
    texCloud= loadTexture(state.renderer, "data/Grasslands/cloud.png");
    texWood= loadTexture(state.renderer, "data/Grasslands/wood.png");
    texLeaves= loadTexture(state.renderer, "data/Grasslands/leaves.png");
    texStone= loadTexture(state.renderer, "data/Grasslands/stone.png");
    texWater= loadTexture(state.renderer, "data/Grasslands/water.png");
    texLava= loadTexture(state.renderer, "data/Grasslands/lava.png");
    texGrass= loadTexture(state.renderer, "data/Grasslands/grass.png");
    texDirt= loadTexture(state.renderer, "data/Grasslands/dirt.png");
    texBgStone = loadTexture(state.renderer, "data/Grasslands/bg_stone.png");
    texBgWood = loadTexture(state.renderer, "data/Grasslands/bg_wood.png");
    texBgSky = loadTexture(state.renderer, "data/Grasslands/bg_sky.png");
    texSignUp = loadTexture(state.renderer, "data/Grasslands/sign_up.png");
    texSignDown = loadTexture(state.renderer, "data/Grasslands/sign_down.png");
    texSignRight = loadTexture(state.renderer, "data/Grasslands/sign_right.png");
    texSignLeft = loadTexture(state.renderer, "data/Grasslands/sign_left.png");

    //Char Select Background
    
    texCharSelectBackground = loadTexture(state.renderer, "data/CharSelect/background.png");
    texCursor = loadTexture(state.renderer, "data/CharSelect/cursor.png");
    texCharIcons = loadTexture(state.renderer, "data/CharSelect/charIcons.png");
    texSword = loadTexture(state.renderer, "data/CharSelect/running_sword_large.png");
    texShotgun = loadTexture(state.renderer, "data/CharSelect/running_shotgun_large.png");
    texJetpack = loadTexture(state.renderer, "data/CharSelect/running_jetpack_large.png");

    texMapPreviews[MAP_GRASSLAND] = loadTexture(state.renderer, "data/CharSelect/grasslands_preview.png");
    texMapPreviews[MAP_SPACESHIP] = loadTexture(state.renderer, "data/CharSelect/spaceship_preview.png");
    texMapPreviews[MAP_3] = loadTexture(state.renderer, "data/CharSelect/stage_3_preview.png");
    texMapPreviews[MAP_4] = loadTexture(state.renderer, "data/CharSelect/stage_4_preview.png");
    texMapPreviews[MAP_5] = loadTexture(state.renderer, "data/CharSelect/stage_5_preview.png");
    texMapPreviews[MAP_GRAND_PRIX] = loadTexture(state.renderer, "data/CharSelect/grand_prix_preview.png");

    texMapTextPreviews[MAP_GRASSLAND_TEXT] = loadTexture(state.renderer, "data/CharSelect/grasslands_preview_text.png");
    texMapTextPreviews[MAP_SPACESHIP_TEXT] = loadTexture(state.renderer, "data/CharSelect/spaceship_preview_text.png");
    texMapTextPreviews[MAP_3_TEXT] = loadTexture(state.renderer, "data/CharSelect/stage_3_preview_text.png");
    texMapTextPreviews[MAP_4_TEXT] = loadTexture(state.renderer, "data/CharSelect/stage_4_preview_text.png");
    texMapTextPreviews[MAP_5_TEXT] = loadTexture(state.renderer, "data/CharSelect/stage_5_preview_text.png");

    texLeftArrow = loadTexture(state.renderer, "data/CharSelect/left_arrow.png");
    texRightArrow = loadTexture(state.renderer, "data/CharSelect/right_arrow.png");

    //Items
    texItemBox = loadTexture(state.renderer, "data/ItemSprites/item_box.png");
    texItemStorage = loadTexture(state.renderer, "data/ItemSprites/item_storage.png");
    texItemRandomizer = loadTexture(state.renderer, "data/ItemSprites/item_randomization.png");
    texBombStorage = loadTexture(state.renderer, "data/ItemSprites/bomb_storage.png");
    texBoomboxStorage = loadTexture(state.renderer, "data/ItemSprites/boombox_storage.png");
    texBouncyBallStorage = loadTexture(state.renderer, "data/ItemSprites/bouncyball_storage.png");
    texFogStorage = loadTexture(state.renderer, "data/ItemSprites/fog_storage.png");
    texIceStorage = loadTexture(state.renderer, "data/ItemSprites/ice_storage.png");
    texMissileStorage = loadTexture(state.renderer, "data/ItemSprites/missile_storage.png");
    texSugarStorage = loadTexture(state.renderer, "data/ItemSprites/sugar_storage.png");
    texPieStorage = loadTexture(state.renderer, "data/ItemSprites/pie_storage.png");

    texBomb = loadTexture(state.renderer, "data/ItemSprites/bomb.png");
    texExplosion = loadTexture(state.renderer, "data/ItemSprites/explosion.png");

    texBoombox = loadTexture(state.renderer, "data/ItemSprites/boombox.png");
    texSoundwaves = loadTexture(state.renderer, "data/ItemSprites/boombox_soundwaves.png");

    texSugar = loadTexture(state.renderer, "data/ItemSprites/sugar.png");
    texSugarEffect = loadTexture(state.renderer, "data/ItemSprites/SugarEffect.png");

    texPie = loadTexture(state.renderer, "data/ItemSprites/pie.png");

    texIce = loadTexture(state.renderer, "data/ItemSprites/ice_background.png");
    //Settings
    texSettingsBackground = loadTexture(state.renderer, "data/SettingsSprites/background.png");
    texBigBorder = loadTexture(state.renderer, "data/SettingsSprites/lButtonOutline.png");
    texSmallBorder = loadTexture(state.renderer, "data/SettingsSprites/sButtonOutline.png");
    texSlider = loadTexture(state.renderer, "data/SettingsSprites/Slider.png");

    //Gameplay Settings
    texGameplaySettingsBackground = loadTexture(state.renderer, "data/GameplaySettings/gameplay_settings.png");
    texGameplaySettingsLeftBracket = loadTexture(state.renderer, "data/GameplaySettings/left_bracket.png");
    texGameplaySettingsRightBracket = loadTexture(state.renderer, "data/GameplaySettings/right_bracket.png");
    texGameplaySettings1 = loadTexture(state.renderer, "data/GameplaySettings/1.png");
    texGameplaySettings2 = loadTexture(state.renderer, "data/GameplaySettings/2.png");
    texGameplaySettings3 = loadTexture(state.renderer, "data/GameplaySettings/3.png");
    texGameplaySettings4 = loadTexture(state.renderer, "data/GameplaySettings/4.png");
    texGameplaySettings5 = loadTexture(state.renderer, "data/GameplaySettings/5.png");

    texTitle = loadTexture(state.renderer, "data/TitleScreen/title.png");
    texTextCursor = loadTexture(state.renderer, "data/TitleScreen/text_cursor.png");

    itemTextures = {texBombStorage, texBoomboxStorage, texBouncyBallStorage, texFogStorage,
        texIceStorage, texMissileStorage, texSugarStorage, texPieStorage};

    // minimap
    texMinimap[MAP_SPACESHIP] = loadTexture(state.renderer, "data/Spaceship/minimapSpaceship.png");
    texMinimap[MAP_GRASSLAND] = loadTexture(state.renderer, "data/Grasslands/minimapGrasslands.png");
    texPlayerDots[SHOTGUN] = loadTexture(state.renderer, "data/CharacterSprites/playerDotShotgun.png");
    texPlayerDots[SWORD] = loadTexture(state.renderer, "data/CharacterSprites/playerDotSword.png");
    texPlayerDots[JETPACK] = loadTexture(state.renderer, "data/CharacterSprites/playerDotJetpack.png");
}

void Resources::unload() {
    for (SDL_Texture *tex : textures) {
        SDL_DestroyTexture(tex);
    }
}