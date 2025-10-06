#include <SDL3/SDL.h>

#include "../headers/player.h"
#include "../headers/playerState.h"
#include <vector>

PlayerState* PlayerState::handleInput(Player& player, SDL_KeyboardEvent key) {
    return NULL;
};
void PlayerState::update(Player& player) {
};
void PlayerState::enter(Player& player) {
};
// for generic playerState, do nothing. This solely exists to be overwritten.

PlayerState* IdleState::handleInput(Player& player, SDL_KeyboardEvent key) { 
    return NULL;
};
void IdleState::update(Player& player) {
    if (0){ //TO-DO Change condition to switch player out of idle

    }
};
void IdleState::enter(Player& player) {
};


// float updatePlayer(const SDLState &state, GameData &gd, Resources &res, GameObject &obj, float deltaTime, float currentDirection) {
//     if (obj.data.player.state != PlayerState::dead) {
//         if (state.keys[SDL_SCANCODE_A]) {
//             currentDirection += -1;
//         }
//         if (state.keys[SDL_SCANCODE_D]) {
//             currentDirection += 1;
//         }
//
//         Timer &weaponTimer = obj.data.player.weaponTimer;
//         weaponTimer.step(deltaTime);
//         const auto handleShooting = [&state, &gd, &res, &obj, &weaponTimer]() {
//             if (state.keys[SDL_SCANCODE_J]) {
//                 // bullets!
//                     // in 2.5 hour video, go to 1:54:19 if you want to sync up shooting sprites with animations for running
//                 if (weaponTimer.isTimeOut()) {
//                     /*if (obj.data.player.state == PlayerState::idle) {
//                         obj.texture = res.texShoot;
//                         obj.curAnimation = res.ANIM_PLAYER_SHOOT;
//                     }*/
//                     if(obj.curAnimation == res.ANIM_PLAYER_SLIDE) {
//                         return;
//                     }
//                     if(obj.curAnimation == res.ANIM_PLAYER_JUMP) {
//                         obj.texture = res.texShootJump;
//                         obj.curAnimation=res.ANIM_PLAYER_SHOOT_JUMP;
//                     } else {
//                         obj.texture = res.texShoot;
//                         obj.curAnimation=res.ANIM_PLAYER_SHOOT;
//                     }
//                     weaponTimer.reset();
//                     GameObject bullet;
//                     bullet.data.bullet = BulletData();
//                     bullet.type = ObjectType::bullet;
//                     bullet.dir = gd.player().dir;
//                     bullet.texture = res.texBullet;
//                     bullet.curAnimation = res.ANIM_BULLET_MOVING;
//                     bullet.collider = SDL_FRect {
//                         .x = 0,
//                         .y = 0,
//                         .w = static_cast<float>(res.texBullet->h),
//                         .h = static_cast<float>(res.texBullet->h)
//                     };
//                     const float left = 0;
//                     const float right = 24;
//                     const float t = (obj.dir + 1) / 2.0f; // results in 0 to 1
//                     const float xOffset = left + right * t; // LERP between left and right
//                     const float yVariation = 40;
//                     const float yVelocity = SDL_rand(yVariation) - yVariation / 2.0f;
//                     bullet.vel = glm::vec2(
//                     obj.vel.x + 300.0f * obj.dir, yVelocity);
//                     //printf("bullet.vel.x = %f\n", bullet.vel.x);
//                     bullet.maxSpeedX = 5000.0f;
//                     bullet.animations = res.bulletAnims;
//                     bullet.pos = glm::vec2( 
//                         obj.pos.x + xOffset,
//                         obj.pos.y + TILE_SIZE / 2 + 1
//                     );
//                     // try to reuse old inactive bullets
//                     bool foundInactive = false;
//                     for (int i = 0; i < gd.bullets.size() && !foundInactive; i++) {
//                         if (gd.bullets[i].data.bullet.state == BulletState::inactive) {
//                             foundInactive = true;
//                             gd.bullets[i] = bullet;
//                         }
//                     }
//                     // otherwise push new bullet
//                     if (!foundInactive) {
//                         gd.bullets.push_back(bullet);
//                     }
//                 }
//             }
//         };
//         switch (obj.data.player.state) {
//             case PlayerState::idle:
//             {
//                 if(currentDirection) { // if moving change to running
//                     obj.data.player.state = PlayerState::moving;
//                 }
//                 /*else { // slowing down is now handled by running state
//                     if (obj.vel.x) { // slow player down when idle
//                         const float factor = obj.vel.x > 0 ? -1.5f : 1.5f;
//                         float amount = factor * obj.acc.x * deltaTime;
//                         if (std::abs(obj.vel.x) < std::abs(amount)) {
//                             obj.vel.x = 0;
//                         }
//                         else {
//                             obj.vel.x += amount;
//                         }
//                     }
//                 }*/
//                 obj.texture = res.texIdle;
//                 obj.curAnimation = res.ANIM_PLAYER_IDLE;
//                 handleShooting();
//                 break;
//             }
//             case PlayerState::moving:
//             {
//                 if (!currentDirection && obj.grounded) { // if not moving, slow down
//                     const float factor = obj.vel.x > 0 ? -1.0f : 1.0f;
//                     float amount = factor * obj.acc.x * deltaTime;
//                     if (std::abs(obj.vel.x) < std::abs(amount)) {
//                         obj.vel.x = 0;
//                         obj.data.player.state = PlayerState::idle; // once stopped, set player to idle
//                     }
//                     else {
//                         obj.vel.x += amount;
//                     }
//                 }
//                 if (isSliding(obj) && obj.grounded) { // moving in different direction of vel and pressing a direction, sliding
//                     obj.texture = res.texSlide;
//                     obj.curAnimation = res.ANIM_PLAYER_SLIDE;
//                 } else {
//                     obj.texture = res.texRun;
//                     obj.curAnimation = res.ANIM_PLAYER_WALK;
//                 }
//
//                 if (state.keys[SDL_SCANCODE_LSHIFT]) { // if not pressing then reset
//                     float LEEWAY = 20;
//                     if (obj.grounded && std::abs(obj.vel.x) >= (obj.data.player.maxRunX - LEEWAY)) { // if grounded and moving fast enter sprint (eventually)                
//                         if (!obj.data.player.sprintTimer.isTimeOut()) {
//                             obj.data.player.sprintTimer.step(deltaTime);
//                         } else {
//                             obj.maxSpeedX = obj.data.player.maxSprintX;
//                             obj.data.player.state = PlayerState::sprinting;
//                         }
//                     } 
//                 } else {
//                     obj.data.player.sprintTimer.reset();
//                 }
//                 handleShooting();
//                 break;
//             }
//             case PlayerState::sprinting:
//             {
//                 float LEEWAY = 20;
//                 if (obj.grounded && // if on ground and sliding or too slow reset sprint
//                     (isSliding(obj) || std::abs(obj.vel.x) < (obj.data.player.maxRunX - LEEWAY))) {       
//                     obj.data.player.sprintTimer.reset();
//                     obj.maxSpeedX = obj.data.player.maxRunX;
//                     obj.data.player.state = PlayerState::moving;
//                 }
//                 handleShooting();
//                 break;
//             }
//             case PlayerState::jumping:
//             {
//                 obj.texture = res.texJump;
//                 obj.curAnimation = res.ANIM_PLAYER_JUMP;
//                 handleShooting();
//                 break;
//             } 
//             case PlayerState::falling:
//             {
//                 obj.texture = res.texDie;
//                 obj.curAnimation = res.ANIM_PLAYER_DIE;
//                 break;
//             }
//             case PlayerState::jumpLaunch:
//             {
//                 obj.texture = res.texLaunch;
//                 obj.curAnimation = res.ANIM_PLAYER_LAUNCH;
//
//                 // once the launch animation finishes, switch to normal in-air "jump"
//                 if (obj.animations[obj.curAnimation].isDone()) {
//                     obj.data.player.state = PlayerState::jumping;
//                     obj.texture = res.texJump;
//                     obj.curAnimation = res.ANIM_PLAYER_JUMP;
//                     obj.animations[obj.curAnimation].reset();
//                 }
//                 handleShooting();
//                 break;
//             }
//             case PlayerState::roll:
//             {
//                 obj.texture = res.texRoll;
//                 obj.curAnimation = res.ANIM_PLAYER_ROLL;
//
//                 // when roll animation finishes, switch to moving
//                 if (obj.animations[obj.curAnimation].isDone()) {
//                     obj.data.player.state = PlayerState::moving;
//                     obj.texture = res.texRun;
//                     obj.curAnimation = res.ANIM_PLAYER_WALK;
//                     obj.animations[obj.curAnimation].reset();
//                 }
//                 handleShooting(); // optional: allow shooting while rolling
//                 break;
//             }
//         }
//         if (std::abs(obj.pos.y) > 1500) { // hard coded, lol!
//             obj.data.player.state = PlayerState::dead; // die if you fall off
//             obj.texture = res.texDie;
//             obj.curAnimation = res.ANIM_PLAYER_DIE;
//             obj.vel.x = 0;
//         }
//         //printf("Player x = %f, Player y = %f\n", obj.pos.x, obj.pos.y);
//     } else { // player is dead, reset map
//         Timer &deathTimer = obj.data.player.deathTimer;
//         deathTimer.step(deltaTime);
//         if (deathTimer.isTimeOut()) {
//             run = false; // exit program
//         }
//     }
//      return currentDirection;
// }
//
// void update(const SDLState &state, GameData &gd, Resources &res, GameObject &obj, float deltaTime) {
//     // update animation
//     if (obj.curAnimation != -1) {
//         obj.animations[obj.curAnimation].step(deltaTime);
//     }
//     if (obj.dynamic && !obj.grounded) {
//         obj.vel.y += changeVel(700 * obj.gravityScale * deltaTime, obj); // gravity
//         //printf("x=%d, y=%d\n", obj.pos.x, obj.pos.y);
//     }
//     float currentDirection = 0;
//     switch (obj.type) {
//         /*case ObjectType::player:
//         {
//             currentDirection = updatePlayer(state, gd, res, obj, deltaTime, currentDirection);
//             break;
//         }*/
//         case ObjectType::bullet:
//         {
//             updateBullet(state, gd, res, obj, deltaTime, currentDirection);
//             break;
//         }
//         case ObjectType::enemy:
//         {
//             updateEnemy(state, gd, res, obj, deltaTime, currentDirection);
//             break;
//         }
//         case ObjectType::obstacle:
//         {
//             updateObstacle(state, gd, res, obj, deltaTime, currentDirection);
//             break;
//         }
//     }
//    
//     if (currentDirection) {
//         obj.dir = currentDirection;
//     }
//     obj.vel += currentDirection * obj.acc * deltaTime;
//     if (std::abs(obj.vel.x) > obj.maxSpeedX) {
//         if (!isSliding(obj)) { // if not sliding slow down
//             obj.vel.x -= 1.5 * obj.acc.x * deltaTime * currentDirection;
//         }
//     }
//
//     // add vel to pos
//     obj.pos += obj.vel * deltaTime;
//     // collision
//     bool foundGround = obj.grounded;
//     obj.grounded = false;
//     for (GameObject &objB : gd.mapTiles) { // check if player is touching any map tiles, currently no enemy collision
//         if (obj.dynamic && isOnscreen(state, gd, obj) && isOnscreen(state, gd, objB)) {
//             checkCollision(state, gd, res, obj, objB, deltaTime);
//         } else if (obj.type == ObjectType::bullet) {
//             checkCollision(state, gd, res, obj, objB, deltaTime);
//         }
//     }
//     for (GameObject &objB : gd.lasers){
//         checkCollision(state, gd, res, obj, objB, deltaTime);     
//     }
//     /*if (obj.grounded && !foundGround) {
//         if (obj.grounded && obj.type == ObjectType::player) {
//             if ((obj.data.player.state == PlayerState::jumping && obj.data.player.fastfalling)|| obj.data.player.state == PlayerState::falling) {
//                 obj.data.player.state = PlayerState::roll;
//                 obj.texture = res.texRoll;
//                 obj.curAnimation = res.ANIM_PLAYER_ROLL;
//                 obj.animations[obj.curAnimation].reset();
//             } else {
//                 obj.data.player.state = PlayerState::moving;
//             }
//
//             obj.data.player.fastfalling = false;
//             obj.data.player.canDoubleJump = true;
//             obj.gravityScale = 1.0f;
//         }
//     }*/
// }


//Use tempPlayer->nextStateVal to return the new state of the player
PlayerState * changePlayerState(PlayerState * tempPlayer){
    PlayerState * newPlayer;
    switch(tempPlayer->nextStateVal){
        case IDLE:
        {
            break;
        }
        case WALK:
        {
            break;
        }
        case RUN:
        {
            break;
        }
        case SPRINT:
        {
            break;
        }
        case LAUNCH:
        {
            break;
        }
        case JUMP:
        {
            break;
        }
        case ROLL:
        {
            break;
        }
        case FALL:
        {
            break;
        }
        case DEAD:
        {
            break;
        }
    }

    newPlayer->currStateVal = tempPlayer->nextStateVal;
    return tempPlayer;
}