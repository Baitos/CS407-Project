#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/update.h"
#include "../headers/collision.h"

float updatePlayer(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection) {
    if (obj.data.player.state != PlayerState::dead) {
        if (state.keys[SDL_SCANCODE_A]) {
            currentDirection += -1;
        }
        if (state.keys[SDL_SCANCODE_D]) {
            currentDirection += 1;
        }
        Timer &weaponTimer = obj.data.player.weaponTimer;
        weaponTimer.step(deltaTime);

        const auto handleShooting = [&state, &gs, &res, &obj, &weaponTimer]() {
            if (state.keys[SDL_SCANCODE_J]) {
                // bullets!
                    // in 2.5 hour video, go to 1:54:19 if you want to sync up shooting sprites with animations for running
                if (weaponTimer.isTimeOut()) {
                    /*if (obj.data.player.state == PlayerState::idle) {
                        obj.texture = res.texShoot;
                        obj.curAnimation = res.ANIM_PLAYER_SHOOT;
                    }*/
                    if(obj.curAnimation == res.ANIM_PLAYER_SLIDE) {
                        return;
                    }
                    obj.texture = res.texShoot;
                    obj.curAnimation=res.ANIM_PLAYER_SHOOT;
                    weaponTimer.reset();
                    GameObject bullet;
                    bullet.data.bullet = BulletData();
                    bullet.type = ObjectType::bullet;
                    bullet.dir = gs.player().dir;
                    bullet.texture = res.texBullet;
                    bullet.curAnimation = res.ANIM_BULLET_MOVING;
                    bullet.collider = SDL_FRect {
                        .x = 0,
                        .y = 0,
                        .w = static_cast<float>(res.texBullet->h),
                        .h = static_cast<float>(res.texBullet->h)
                    };
                    const float left = 0;
                    const float right = 24;
                    const float t = (obj.dir + 1) / 2.0f; // results in 0 to 1
                    const float xOffset = left + right * t; // LERP between left and right
                    const float yVariation = 40;
                    const float yVelocity = SDL_rand(yVariation) - yVariation / 2.0f;
                    bullet.vel = glm::vec2(
                    obj.vel.x + 300.0f * obj.dir, yVelocity);
                    //printf("bullet.vel.x = %f\n", bullet.vel.x);
                    bullet.maxSpeedX = 5000.0f;
                    bullet.animations = res.bulletAnims;
                    bullet.pos = glm::vec2( 
                        obj.pos.x + xOffset,
                        obj.pos.y + TILE_SIZE / 2 + 1
                    );
                    // try to reuse old inactive bullets
                    bool foundInactive = false;
                    for (int i = 0; i < gs.bullets.size() && !foundInactive; i++) {
                        if (gs.bullets[i].data.bullet.state == BulletState::inactive) {
                            foundInactive = true;
                            gs.bullets[i] = bullet;
                        }
                    }
                    // otherwise push new bullet
                    if (!foundInactive) {
                        gs.bullets.push_back(bullet);
                    }
                }
            }
        };
        switch (obj.data.player.state) {
            case PlayerState::idle:
            {
                if(currentDirection) { // if moving change to running
                    obj.data.player.state = PlayerState::moving;
                }
                /*else { // slowing down is now handled by running state
                    if (obj.vel.x) { // slow player down when idle
                        const float factor = obj.vel.x > 0 ? -1.5f : 1.5f;
                        float amount = factor * obj.acc.x * deltaTime;
                        if (std::abs(obj.vel.x) < std::abs(amount)) {
                            obj.vel.x = 0;
                        }
                        else {
                            obj.vel.x += amount;
                        }
                    }
                }*/
                obj.texture = res.texIdle;
                obj.curAnimation = res.ANIM_PLAYER_IDLE;
                handleShooting();
                break;
            }
            case PlayerState::moving:
            {
                if (!currentDirection && obj.grounded) { // if not moving, slow down
                    const float factor = obj.vel.x > 0 ? -1.0f : 1.0f;
                    float amount = factor * obj.acc.x * deltaTime;
                    if (std::abs(obj.vel.x) < std::abs(amount)) {
                        obj.vel.x = 0;
                        obj.data.player.state = PlayerState::idle; // once stopped, set player to idle
                    }
                    else {
                        obj.vel.x += amount;
                    }
                }
                if (obj.vel.x * obj.dir < 0 && obj.grounded) { // moving in different direction of vel and pressing a direction, sliding
                    obj.texture = res.texSlide;
                    obj.curAnimation = res.ANIM_PLAYER_SLIDE;
                } else {
                    obj.texture = res.texRun;
                    obj.curAnimation = res.ANIM_PLAYER_WALK;
                }
                handleShooting();
                break;
            }
            case PlayerState::jumping:
            {
                obj.texture = res.texJump;
                obj.curAnimation = res.ANIM_PLAYER_JUMP;
                handleShooting();
                break;
            } 
            case PlayerState::falling:
            {
                obj.texture = res.texDie;
                obj.curAnimation = res.ANIM_PLAYER_DIE;
                break;
            }
        }
        if (obj.pos.y > 1000) { // hard coded, lol!
            obj.data.player.state = PlayerState::dead; // die if you fall off
            obj.texture = res.texDie;
            obj.curAnimation = res.ANIM_PLAYER_DIE;
            obj.vel.x = 0;
        }
        //printf("Player x = %f, Player y = %f\n", obj.pos.x, obj.pos.y);
    } else { // player is dead, reset map
        Timer &deathTimer = obj.data.player.deathTimer;
        deathTimer.step(deltaTime);
        if (deathTimer.isTimeOut()) {
            run = false; // exit program
        }
    }
    return currentDirection;
}

float updateBullet(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection) {
    switch (obj.data.bullet.state) {
        case BulletState::moving: {
            if (obj.pos.x - gs.mapViewport.x < 0 || // left side
                obj.pos.x - gs.mapViewport.x > state.logW || // right side
                obj.pos.y - gs.mapViewport.y < 0 || // up
                obj.pos.y - gs.mapViewport.y > state.logH) // down
            { 
                obj.data.bullet.state = BulletState::inactive;
            }
            break;
        }
        case BulletState::colliding: {
            if (obj.animations[obj.curAnimation].isDone()) {
                obj.data.bullet.state = BulletState::inactive;
            }
        }
    }
    return currentDirection;
}

float updateEnemy(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection) {
    EnemyData &d = obj.data.enemy;
    switch (d.state) {
        /*case EnemyState::idle: {
            glm::vec2 playerDir = gs.player().pos - obj.pos;
            if (glm::length(playerDir) < 100) {
                currentDirection = playerDir.x < 0 ? -1 : 1;
            } else {
                obj.acc = glm::vec2(0);
                obj.vel.x = 0;
            }
            break;
        }*/ // this is for proximity based movement, ignore
        case EnemyState::damaged:
        {
            if (d.damagedTimer.step(deltaTime)) {
                // do nothing
            }
            break;
        }
        case EnemyState::dead: {
            obj.vel.x = 0;
            if (obj.curAnimation != -1 && obj.animations[obj.curAnimation].isDone()) {
                obj.curAnimation = -1;
                obj.spriteFrame = 1;
            }
            break;
        }
    }
    return currentDirection;
}
float updateObstacle(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime, float currentDirection) {
    //Timer for Laser
    Timer &laserTimer = obj.data.obstacle.laserTimer;
    laserTimer.step(deltaTime);
    if (laserTimer.isTimeOut()){
        //Resets the timer and switches tHe LaSeR
        laserTimer.reset();
        obj.data.obstacle.laserActive = !obj.data.obstacle.laserActive;
    }
    return currentDirection;
}

void update(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime) {
    // update animation
    if (obj.curAnimation != -1) {
        obj.animations[obj.curAnimation].step(deltaTime);
    }
    if (obj.dynamic && !obj.grounded) {
        obj.vel += glm::vec2(0, 700) * deltaTime; // gravity
        //printf("x=%d, y=%d\n", obj.pos.x, obj.pos.y);
    }
    float currentDirection = 0;
    switch (obj.type) {
        case ObjectType::player:
        {
            currentDirection = updatePlayer(state, gs, res, obj, deltaTime, currentDirection);
            break;
        }
        case ObjectType::bullet:
        {
            updateBullet(state, gs, res, obj, deltaTime, currentDirection);
            break;
        }
        case ObjectType::enemy:
        {
            updateEnemy(state, gs, res, obj, deltaTime, currentDirection);
            break;
        }
        case ObjectType::obstacle:
        {
            updateObstacle(state, gs, res, obj, deltaTime, currentDirection);
            break;
        }
    }
    if (currentDirection) {
        obj.dir = currentDirection;
    }
    obj.vel += currentDirection * obj.acc * deltaTime;
    if (std::abs(obj.vel.x) > obj.maxSpeedX) {
        obj.vel.x -= 1.5 * currentDirection * obj.acc.x * deltaTime;
        //obj.vel.x -= currentDirection * obj.acc * deltaTime;
    }

    // add vel to pos
    obj.pos += obj.vel * deltaTime;
    // collision
    bool foundGround = false;
    for (auto &layer : gs.layers) {
        for (GameObject &objB : layer) {
            if (isOnscreen(state, gs, obj) && isOnscreen(state, gs, objB)) {
                if (&obj != &objB) {
                    checkCollision(state, gs, res, obj, objB, deltaTime);
                    if ((objB.type == ObjectType::level && objB.data.level.state != LevelState::portal) || objB.type == ObjectType::obstacle) {
                        // grounded sensor
                        const float inset = 2.0;
                        SDL_FRect sensor {
                            .x = obj.pos.x + obj.collider.x + 1,
                            .y = obj.pos.y + obj.collider.y + obj.collider.h,
                            .w = obj.collider.w - inset,
                            .h = 1
                        };
                        SDL_FRect rectB {
                            .x = objB.pos.x + objB.collider.x,
                            .y = objB.pos.y + objB.collider.y,
                            .w = objB.collider.w,
                            .h = objB.collider.h
                        };
                        SDL_FRect rectC { 0 };
                        if (SDL_GetRectIntersectionFloat(&sensor, &rectB, &rectC)) {
                            foundGround = true;
                        }
                    }    
                }
            }
        }
        for (GameObject &objB : gs.lasers){
            checkCollision(state, gs, res, obj, objB, deltaTime);     
        }
    }
    if (obj.grounded != foundGround) { // changing state
        obj.grounded = foundGround;
        if (foundGround && obj.type == ObjectType::player && obj.data.player.state != PlayerState::dead) {
            obj.data.player.state = PlayerState::moving;
            obj.data.player.fastfalling = false;
            obj.data.player.canDoubleJump = true;
        }
    }
}



void handleKeyInput(const SDLState &state, GameState &gs, Resources &res, GameObject &obj,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime) {

    if (key.scancode == SDL_SCANCODE_F12 && keyDown && !key.repeat) { // debug
            gs.debugMode = !gs.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F11 && keyDown && !key.repeat) {
        gs.player().pos = gs.EntrancePortal;
        gs.player().pos.x -= 32;
    }
    if (obj.type == ObjectType::player) {
        const float JUMP_FORCE = -450.f;
        const auto handleJumping = [&state, &gs, &obj, key, keyDown, JUMP_FORCE]() {
            if (key.scancode == SDL_SCANCODE_SPACE && keyDown && !key.repeat) { // jumping
                if (obj.grounded) { // single jump
                    obj.data.player.state = PlayerState::jumping;
                    obj.vel.y = JUMP_FORCE;
                } else if (obj.data.player.canDoubleJump) { // double jump
                    obj.data.player.state = PlayerState::jumping;
                    obj.vel.y = JUMP_FORCE;  
                    obj.data.player.canDoubleJump = false;
                }
                printf("canDoubleJump = %d\n" , obj.data.player.canDoubleJump);
            }
        };
        const auto handleRunning = [&state, &gs, &obj, &res, key, keyDown]() {
            if (key.scancode == SDL_SCANCODE_LSHIFT) {
                if (keyDown) { // if held down, increase speed
                    obj.maxSpeedX = obj.data.player.maxRunX;
                    obj.curAnimation = res.ANIM_PLAYER_RUN; 
                } else {
                    obj.maxSpeedX = obj.data.player.maxWalkX;
                    obj.curAnimation = res.ANIM_PLAYER_WALK; 
                }
            }
        };
        const auto handleFalling = [&state, &gs, &obj, &res, key, keyDown, deltaTime]() {
            if (key.scancode == SDL_SCANCODE_S && keyDown && !obj.grounded) { // fastfall
                if (!key.repeat && !obj.data.player.fastfalling) {
                    obj.vel.y = -250.0f;
                    obj.data.player.fastfalling = true;
                    obj.data.player.canDoubleJump = false;
                }
                if (std::abs(obj.vel.y) < obj.maxSpeedY) {
                    obj.vel += glm::vec2(0, 700) * 6.0f * deltaTime;
                }
            }
        };
        switch (obj.data.player.state) {
            case PlayerState::idle:
            {
                handleJumping();
                handleRunning();
                break;
            }
            case PlayerState::moving:
            {
                handleJumping();
                handleRunning();
                handleFalling();
                break;
            }
            case PlayerState::jumping:
            {
                handleRunning();
                handleJumping();
                handleFalling();
                break;               
            }
        }
    }
}