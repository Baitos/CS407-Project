
#include <SDL3_image/SDL_image.h>
#include "../headers/resources.h"
#include "../headers/initState.h"

SDL_Texture* Resources::loadTexture(SDL_Renderer *renderer, const std::string &filepath) { // load texture from filepath
    // load game assets
    SDL_Texture *tex = IMG_LoadTexture(renderer, filepath.c_str());
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST); // pixel perfect
    textures.push_back(tex);
    return tex;
}

void Resources::load(SDLState &state) { // First variable controls how many frames there are, second is how long each frame lasts (in seconds)
    playerAnimsJ.resize(12); // 
    playerAnimsJ[ANIM_PLAYER_IDLE] = Animation(1, 1.6f);
    playerAnimsJ[ANIM_PLAYER_WALK] = Animation(8, 1.0f);
    playerAnimsJ[ANIM_PLAYER_RUN] = Animation(8, 0.5f);
    playerAnimsJ[ANIM_PLAYER_SLIDE] = Animation(1, 1.0f);
    playerAnimsJ[ANIM_PLAYER_SHOOT] = Animation(1, 0.8f);
    playerAnimsJ[ANIM_PLAYER_JUMP] = Animation(1, 0.3f); 
    playerAnimsJ[ANIM_PLAYER_DIE] = Animation(1, 1.0f);
    playerAnimsJ[ANIM_PLAYER_LAUNCH] = Animation(3, 0.2f);
    playerAnimsJ[ANIM_PLAYER_SHOOT_JUMP] = Animation(1, 0.8f);
    playerAnimsJ[ANIM_PLAYER_ROLL] = Animation(5, 0.2f);
    playerAnimsJ[ANIM_PLAYER_JETPACK_DEPLOY] = Animation(1, 2.0f);
    playerAnimsJ[ANIM_PLAYER_SWORD_DEPLOY] = Animation(3, 0.3f);

    playerAnims.resize(12); // 
    playerAnims[ANIM_PLAYER_IDLE] = Animation(1, 1.6f);
    playerAnims[ANIM_PLAYER_WALK] = Animation(8, 1.0f);
    playerAnims[ANIM_PLAYER_RUN] = Animation(8, 0.5f);
    playerAnims[ANIM_PLAYER_SLIDE] = Animation(1, 1.0f);
    playerAnims[ANIM_PLAYER_SHOOT] = Animation(1, 2.0f);
    playerAnims[ANIM_PLAYER_JUMP] = Animation(1, 0.3f); 
    playerAnims[ANIM_PLAYER_DIE] = Animation(1, 1.0f);
    playerAnims[ANIM_PLAYER_LAUNCH] = Animation(3, 0.2f);
    playerAnims[ANIM_PLAYER_SHOOT_JUMP] = Animation(1, 0.8f);
    playerAnims[ANIM_PLAYER_ROLL] = Animation(5, 0.2f);
    playerAnims[ANIM_PLAYER_JETPACK_DEPLOY] = Animation(1, 2.0f);
    playerAnims[ANIM_PLAYER_SWORD_DEPLOY] = Animation(3, 0.3f);

    itemAnims.resize(3); // 
    itemAnims[ANIM_ITEM_EMPTY] = Animation(1, 1.0f);
    itemAnims[ANIM_ITEM_CYCLE] = Animation(8, 0.4f);
    itemAnims[ANIM_ITEM_PICKED] = Animation(1, 1.0f);

    portalAnims.resize(2);
    portalAnims[PORTAL_IDLE] = Animation(3, 1.0f);
    charIconAnims.resize(1);
    charIconAnims[ICONS] = Animation(8, 0.0f);
    shotgunAnims.resize(1);
    shotgunAnims[SHOTGUN_BLAST] = Animation(3, 0.3f);

    errorAnims.resize(1); // default to prevent crashes
    errorAnims[ERROR] = Animation(1, 1.0f);


    //load shotgun character animations
    texIdleG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/idle_shotgun.png");
    texRunG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/running_shotgun.png");
    texJumpG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/jumped_shotgun.png");
    texSlideG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/turning_shotgun.png");
    texShootG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shooting_shotgun.png");
    texDieG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shot_shotgun.png");
    texShootJumpG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shooting_jumped_shotgun.png");
    texLaunchG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/jumping_shotgun.png");
    texRollG = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/rolling_shotgun.png");
    
    //load sword character animations
    texIdleS = loadTexture(state.renderer, "data/CharacterSprites/Sword/idle_sword.png");
    texRunS = loadTexture(state.renderer, "data/CharacterSprites/Sword/running_sword.png");
    texJumpS = loadTexture(state.renderer, "data/CharacterSprites/Sword/jumped_sword.png");
    texSlideS = loadTexture(state.renderer, "data/CharacterSprites/Sword/turning_sword.png");
    texShootS = loadTexture(state.renderer, "data/CharacterSprites/Sword/swing_sword.png");
    texDieS = loadTexture(state.renderer, "data/CharacterSprites/Sword/shot_sword.png");
    texShootJumpS = loadTexture(state.renderer, "data/CharacterSprites/Sword/swing_jumped_sword.png");
    texLaunchS = loadTexture(state.renderer, "data/CharacterSprites/Sword/jumping_sword.png");
    texRollS = loadTexture(state.renderer, "data/CharacterSprites/Sword/rolling_sword.png");

    //load jetpack character animations
    texIdleJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/idle_jetpack.png");
    texRunJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/running_jetpack.png");
    texJumpJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/jumped_jetpack.png");
    texSlideJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/turning_jetpack.png");
    texShootJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/on_jetpack.png");
    texDieJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/shot_jetpack.png");
    texShootJumpJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/on_jetpack.png");
    texLaunchJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/jumping_jetpack.png");
    texRollJ = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/rolling_jetpack.png");

    //load special deploy animations
    texJDeploy = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/on_jetpack.png");
    texGDeploy = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shooting_shotgun.png");
    texSDeploy = loadTexture(state.renderer, "data/CharacterSprites/Sword/swing_sword.png");

    texShotgunBlast = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/fire_shotgun.png");

    texGrapple = loadTexture(state.renderer, "data/CharacterSprites/grapple.png");
    texGrappleAngle = loadTexture(state.renderer, "data/CharacterSprites/grappleAngle.png");

    texCrosshair = loadTexture(state.renderer, "data/crosshair.png");
    texGrass = loadTexture(state.renderer, "data/grass.png");
    texBrick = loadTexture(state.renderer, "data/brick.png");
    texStone = loadTexture(state.renderer, "data/stone.png");
    texBush = loadTexture(state.renderer, "data/bush.png");
    texFence = loadTexture(state.renderer, "data/fence.png");
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

    //Char Select Background
    
    texCharSelectBackground = loadTexture(state.renderer, "data/CharSelect/background.png");
    texCursor = loadTexture(state.renderer, "data/CharSelect/cursor.png");
    texCharIcons = loadTexture(state.renderer, "data/CharSelect/charIcons.png");
    texSword = loadTexture(state.renderer, "data/CharSelect/running_sword_large.png");
    texShotgun = loadTexture(state.renderer, "data/CharSelect/running_shotgun_large.png");
    texJetpack = loadTexture(state.renderer, "data/CharSelect/running_jetpack_large.png");

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

    texSugarEffectL = loadTexture(state.renderer, "data/ItemSprites/SugarEffectL.png");
    texSugarEffectR = loadTexture(state.renderer, "data/ItemSprites/SugarEffectR.png");

    //Settings
    texSettingsBackground = loadTexture(state.renderer, "data/SettingsSprites/background.png");
    texBigBorder = loadTexture(state.renderer, "data/SettingsSprites/lButtonOutline.png");
    texSmallBorder = loadTexture(state.renderer, "data/SettingsSprites/sButtonOutline.png");
    texSlider = loadTexture(state.renderer, "data/SettingsSprites/Slider.png");

    itemTextures = {texBombStorage, texBoomboxStorage, texBouncyBallStorage, texFogStorage,
        texIceStorage, texMissileStorage, texSugarStorage, texPieStorage};
}

void Resources::unload() {
    for (SDL_Texture *tex : textures) {
        SDL_DestroyTexture(tex);
    }
}