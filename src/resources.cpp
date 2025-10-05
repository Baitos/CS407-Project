
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
    playerAnims.resize(10); // 
    playerAnims[ANIM_PLAYER_IDLE] = Animation(1, 1.6f);
    playerAnims[ANIM_PLAYER_WALK] = Animation(8, 1.0f);
    playerAnims[ANIM_PLAYER_RUN] = Animation(8, 0.5f);
    playerAnims[ANIM_PLAYER_SLIDE] = Animation(1, 1.0f);
    if (character == 0 ){
        playerAnims[ANIM_PLAYER_SHOOT] = Animation(1, 0.8f);
    } else {
        playerAnims[ANIM_PLAYER_SHOOT] = Animation(1, 2.0f);
    }
    playerAnims[ANIM_PLAYER_JUMP] = Animation(1, 0.3f); 
    playerAnims[ANIM_PLAYER_DIE] = Animation(1, 1.0f);
    playerAnims[ANIM_PLAYER_LAUNCH] = Animation(3, 0.2f);
    playerAnims[ANIM_PLAYER_SHOOT_JUMP] = Animation(1, 0.8f);
    playerAnims[ANIM_PLAYER_ROLL] = Animation(5, 0.2f);
    bulletAnims.resize(2); // 
    bulletAnims[ANIM_BULLET_MOVING] = Animation(4, 0.5f);
    bulletAnims[ANIM_BULLET_HIT] = Animation(3, 0.5f);
    enemyAnims.resize(2);
    enemyAnims[ANIM_ENEMY] = Animation(2, 0.6f);
    enemyAnims[ANIM_ENEMY_DEAD] = Animation(1, 1.0f);
    portalAnims.resize(2);
    portalAnims[PORTAL_IDLE] = Animation(3, 1.0f);
    charIconAnims.resize(1);
    charIconAnims[ICONS] = Animation(8, 0.0f);


    if (character == 0) {
        //load shotgun character animations
        texIdle = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/idle_shotgun.png");
        texRun = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/running_shotgun.png");
        texJump = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/jumped_shotgun.png");
        texSlide = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/turning_shotgun.png");
        texShoot = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shooting_shotgun.png");
        texDie = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shot_shotgun.png");
        texShootJump = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/shooting_jumped_shotgun.png");
        texLaunch = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/jumping_shotgun.png");
        texRoll = loadTexture(state.renderer, "data/CharacterSprites/Shotgun/rolling_shotgun.png");
        texBullet = loadTexture(state.renderer, "data/fireballM.png");
        texBulletHit = loadTexture(state.renderer, "data/fireballHitM.png");
    } else if (character == 1){
        //load sword character animations
        texIdle = loadTexture(state.renderer, "data/CharacterSprites/Sword/idle_sword.png");
        texRun = loadTexture(state.renderer, "data/CharacterSprites/Sword/running_sword.png");
        texJump = loadTexture(state.renderer, "data/CharacterSprites/Sword/jumped_sword.png");
        texSlide = loadTexture(state.renderer, "data/CharacterSprites/Sword/turning_sword.png");
        texShoot = loadTexture(state.renderer, "data/CharacterSprites/Sword/swing_sword.png");
        texDie = loadTexture(state.renderer, "data/CharacterSprites/Sword/shot_sword.png");
        texShootJump = loadTexture(state.renderer, "data/CharacterSprites/Sword/swing_jumped_sword.png");
        texLaunch = loadTexture(state.renderer, "data/CharacterSprites/Sword/jumping_sword.png");
        texRoll = loadTexture(state.renderer, "data/CharacterSprites/Sword/rolling_sword.png");
        texBullet = loadTexture(state.renderer, "data/fireballM.png");
        texBulletHit = loadTexture(state.renderer, "data/fireballHitM.png");
    } else if (character == 2){
        //load jetpack character animations
        texIdle = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/idle_jetpack.png");
        texRun = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/running_jetpack.png");
        texJump = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/jumped_jetpack.png");
        texSlide = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/turning_jetpack.png");
        texShoot = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/on_jetpack.png");
        texDie = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/shot_jetpack.png");
        texShootJump = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/on_jetpack.png");
        texLaunch = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/jumping_jetpack.png");
        texRoll = loadTexture(state.renderer, "data/CharacterSprites/Jetpack/rolling_jetpack.png");
        texBullet = loadTexture(state.renderer, "data/fireballM.png");
        texBulletHit = loadTexture(state.renderer, "data/fireballHitM.png");
    }
    texCrosshair = loadTexture(state.renderer, "data/crosshair.png");
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
}

void Resources::unload() {
    for (SDL_Texture *tex : textures) {
        SDL_DestroyTexture(tex);
    }
}