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

using namespace std;

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
                    if(obj.curAnimation == res.ANIM_PLAYER_JUMP) {
                        obj.texture = res.texShootJump;
                        obj.curAnimation=res.ANIM_PLAYER_SHOOT_JUMP;
                    } else {
                        obj.texture = res.texShoot;
                        obj.curAnimation=res.ANIM_PLAYER_SHOOT;
                    }
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
                if (isSliding(obj) && obj.grounded) { // moving in different direction of vel and pressing a direction, sliding
                    obj.texture = res.texSlide;
                    obj.curAnimation = res.ANIM_PLAYER_SLIDE;
                } else {
                    obj.texture = res.texRun;
                    obj.curAnimation = res.ANIM_PLAYER_WALK;
                }

                if (state.keys[SDL_SCANCODE_LSHIFT]) { // if not pressing then reset
                    float LEEWAY = 20;
                    if (obj.grounded && std::abs(obj.vel.x) >= (obj.data.player.maxRunX - LEEWAY)) { // if grounded and moving fast enter sprint (eventually)                
                        if (!obj.data.player.sprintTimer.isTimeOut()) {
                            obj.data.player.sprintTimer.step(deltaTime);
                        } else {
                            obj.maxSpeedX = obj.data.player.maxSprintX;
                            obj.data.player.state = PlayerState::sprinting;
                        }
                    } 
                } else {
                    obj.data.player.sprintTimer.reset();
                }
                handleShooting();
                break;
            }
            case PlayerState::sprinting:
            {
                float LEEWAY = 20;
                if (obj.grounded && // if on ground and sliding or too slow reset sprint
                    (isSliding(obj) || std::abs(obj.vel.x) < (obj.data.player.maxRunX - LEEWAY))) {       
                    obj.data.player.sprintTimer.reset();
                    obj.maxSpeedX = obj.data.player.maxRunX;
                    obj.data.player.state = PlayerState::moving;
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
            case PlayerState::jumpLaunch:
            {
                obj.texture = res.texLaunch;
                obj.curAnimation = res.ANIM_PLAYER_LAUNCH;

                // once the launch animation finishes, switch to normal in-air "jump"
                if (obj.animations[obj.curAnimation].isDone()) {
                    obj.data.player.state = PlayerState::jumping;
                    obj.texture = res.texJump;
                    obj.curAnimation = res.ANIM_PLAYER_JUMP;
                    obj.animations[obj.curAnimation].reset();
                }
                handleShooting();
                break;
            }
            case PlayerState::roll:
            {
                obj.texture = res.texRoll;
                obj.curAnimation = res.ANIM_PLAYER_ROLL;

                // when roll animation finishes, switch to moving
                if (obj.animations[obj.curAnimation].isDone()) {
                    obj.data.player.state = PlayerState::moving;
                    obj.texture = res.texRun;
                    obj.curAnimation = res.ANIM_PLAYER_WALK;
                    obj.animations[obj.curAnimation].reset();
                }
                handleShooting(); // optional: allow shooting while rolling
                break;
            }
        }
        if (std::abs(obj.pos.y) > 1500) { // hard coded, lol!
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
        obj.vel.y += changeVel(700 * obj.gravityScale * deltaTime, obj); // gravity
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
        if (!isSliding(obj)) { // if not sliding slow down
            obj.vel.x -= 1.5 * obj.acc.x * deltaTime * currentDirection;
        }
    }

    // add vel to pos
    obj.pos += obj.vel * deltaTime;
    // collision
    bool foundGround = obj.grounded;
    obj.grounded = false;
    for (GameObject &objB : gs.mapTiles) { // check if player is touching any map tiles, currently no enemy collision
        if (obj.dynamic && isOnscreen(state, gs, obj) && isOnscreen(state, gs, objB)) {
            checkCollision(state, gs, res, obj, objB, deltaTime);
        } else if (obj.type == ObjectType::bullet) {
            checkCollision(state, gs, res, obj, objB, deltaTime);
        }
    }
    for (GameObject &objB : gs.lasers){
        checkCollision(state, gs, res, obj, objB, deltaTime);     
    }
    if (obj.grounded && !foundGround) {
        if (obj.grounded && obj.type == ObjectType::player) {
            if ((obj.data.player.state == PlayerState::jumping && obj.data.player.fastfalling)|| obj.data.player.state == PlayerState::falling) {
                obj.data.player.state = PlayerState::roll;
                obj.texture = res.texRoll;
                obj.curAnimation = res.ANIM_PLAYER_ROLL;
                obj.animations[obj.curAnimation].reset();
            } else {
                obj.data.player.state = PlayerState::moving;
            }

            obj.data.player.fastfalling = false;
            obj.data.player.canDoubleJump = true;
            obj.gravityScale = 1.0f;
        }
    }
}

void handleCrosshair(const SDLState &state, GameState &gs, Resources &res, GameObject &obj, float deltaTime) {
    SDL_GetMouseState(&gs.mouseCoords.x, &gs.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    //printf("Xrat: %f, Yrat: %f\n", xRatio, yRatio);
    gs.mouseCoords.x = gs.mouseCoords.x * xRatio;
    gs.mouseCoords.y = gs.mouseCoords.y * yRatio;
    SDL_FRect dst { 
        .x = gs.mouseCoords.x - OFFSET,
        .y = gs.mouseCoords.y - OFFSET,
        .w = CROSSHAIR_SIZE,
        .h = CROSSHAIR_SIZE
    };
    SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255); // draw line to crosshair
    glm::vec2 pOffset = findCenterOfSprite(gs.player());
    //printf("x: %d, y: %d\n", pOffset.x, pOffset.y);
    SDL_RenderLine(state.renderer, gs.player().pos.x - gs.mapViewport.x + pOffset.x, gs.player().pos.y - gs.mapViewport.y + pOffset.y, gs.mouseCoords.x, gs.mouseCoords.y);
    SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);
    
    //printf("mouseX: %f, mouseY: %f\n", gs.mouseCoords.x, gs.mouseCoords.y);
    SDL_RenderTexture(state.renderer, res.texCrosshair, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn
    
}

void handleKeyInput(const SDLState &state, GameState &gs, Resources &res, GameObject &obj,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime) {

    if (key.scancode == SDL_SCANCODE_F12 && keyDown && !key.repeat) { // debug
            gs.debugMode = !gs.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F11 && keyDown && !key.repeat) { // tp to entrance portal
        gs.player().pos = gs.EntrancePortal;
        gs.player().pos.x -= TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_F2 && keyDown && !key.repeat) { // anti gravity
        gs.player().flip = -1 * gs.player().flip;
    }
    if (key.scancode == SDL_SCANCODE_F1) {
        run = false;
    }
    if (obj.type == ObjectType::player) {
        const float JUMP_FORCE = -450.f;
        const auto handleJumping = [&state, &gs, &obj, res, key, keyDown, JUMP_FORCE]() {
            if (key.scancode == SDL_SCANCODE_SPACE && keyDown && !key.repeat) { // jumping
                if (obj.grounded) { // single jump
                    //add something for jump animation before going up
                    //obj.data.player.state = PlayerState::jumping;
                     obj.data.player.state = PlayerState::jumpLaunch; // <-- use launch state
                     obj.texture = res.texLaunch;
                     obj.curAnimation = res.ANIM_PLAYER_LAUNCH; 
                     obj.animations[obj.curAnimation].reset();
                     obj.vel.y = changeVel(JUMP_FORCE, obj); 
                } else if (obj.data.player.canDoubleJump) { // double jump
                    obj.data.player.state = PlayerState::jumping;
                    obj.vel.y = changeVel(JUMP_FORCE, obj);  
                    obj.data.player.canDoubleJump = false;
                    obj.gravityScale = 1.0f; // reset gravity
                }
                //printf("canDoubleJump = %d\n" , obj.data.player.canDoubleJump);
            }
            else if (!keyDown && key.scancode == SDL_SCANCODE_SPACE) { // letting go of jump
                /*if (obj.vel.y < 0) { // OPTION 1: Set velocity to 0 when you let go. makes sharp but precise jumps
                    obj.vel.y = 0;
                }*/
                float termVel = -200.0f; // option 2: Set velocity to predefined amount when you let go. makes less sharp jumps
                float shouldFlip = obj.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
                if (shouldFlip * obj.vel.y < shouldFlip * termVel) { 
                    obj.vel.y = changeVel(termVel, obj);
                }
                //obj.gravityScale = 2.0f; // option 3; double their gravity until they land
            }
        };
        const auto handleRunning = [&state, &gs, &obj, &res, key, keyDown, deltaTime]() {
            if (key.scancode == SDL_SCANCODE_LSHIFT) {
                if (keyDown) { // if held down, increase speed
                    obj.maxSpeedX = obj.data.player.maxRunX;
                    obj.curAnimation = res.ANIM_PLAYER_RUN;
                } else {
                    obj.maxSpeedX = obj.data.player.maxWalkX;
                    obj.curAnimation = res.ANIM_PLAYER_WALK; 
                    obj.data.player.sprintTimer.reset();
                }
            }
        };
        const auto handleSprinting = [&state, &gs, &obj, &res, key, keyDown]() {
            if (key.scancode == SDL_SCANCODE_LSHIFT && !keyDown) {
                obj.maxSpeedX = obj.data.player.maxWalkX;
                obj.curAnimation = res.ANIM_PLAYER_WALK;
                obj.data.player.sprintTimer.reset();
                obj.data.player.state = PlayerState::moving;
            }
        };
        const auto handleFalling = [&state, &gs, &obj, &res, key, keyDown, deltaTime]() {
            if (key.scancode == SDL_SCANCODE_S && keyDown && !obj.grounded) { // fastfall
                if (!key.repeat && !obj.data.player.fastfalling) {
                    obj.vel.y = changeVel(-250.0f, obj);
                    obj.data.player.fastfalling = true;
                    obj.data.player.state = PlayerState::jumping;
                    obj.curAnimation = res.ANIM_PLAYER_JUMP;
                    //obj.data.player.canDoubleJump = false;
                }
                obj.gravityScale = 3.0f;
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
            case PlayerState::jumpLaunch:
            {
                handleRunning();
                handleJumping();
                handleFalling();
                break;               
            }
            case PlayerState::roll:
            {
                handleRunning();
                handleJumping();
                handleFalling();
                break;               
            }
            case PlayerState::sprinting:
            {
                handleSprinting();
                handleJumping();
                handleFalling();
                break;
            }
        }
        //printf("velX = %f, velY = %f\n", obj.vel.x, obj.vel.y);
    }
}