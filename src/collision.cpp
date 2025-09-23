#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/collision.h"

void collisionResponse(const SDLState &state, GameState &gs, const Resources &res, 
                       const SDL_FRect &rectA, const SDL_FRect &rectB, 
                       const SDL_FRect &rectC, GameObject &a, GameObject &b, float deltaTime) 
{
    const auto genericResponse = [&]() {
        if (rectC.w < rectC.h) { // horizontal col
            //printf("Horizontal Collision, %f = rectC.w, %f = rectC.h\n", rectC.w, rectC.h);
            if (a.vel.x > 0) { // going right
                a.pos.x -= rectC.w;
            }
            else if (a.vel.x < 0) { // going left
                a.pos.x += rectC.w;
            }
            if (a.type == ObjectType::enemy) {
                a.vel.x = -a.vel.x; // turn enemy around when it hits a wall
                a.dir = -a.dir;
            } else {
                a.vel.x = 0;
            }
            
        } 
        else { // vert col
            //printf("Vertical Collision, %f = rectC.w, %f = rectC.h\n", rectC.w, rectC.h);
            if (a.vel.y > 0) { // going down
                a.pos.y -= rectC.h;
            }
            else if (a.vel.y < 0)  { // going up
                a.pos.y += rectC.h;
            }
            a.vel.y = 0;
        }
    };
    // obj we are checking
    if (a.type == ObjectType::player) {
        if (a.data.player.state != PlayerState::dead) {
            // obj a is colliding with
            switch (b.type) {
                case ObjectType::level: {
                    if(b.data.level.state == LevelState::ground){
                        genericResponse();
                        
                    } else if (b.data.level.state == LevelState::portal ){
                        //Teleport to Exit Portal
                        if (b.data.level.isEntrance == true){
                            a.pos = gs.ExitPortal;
                        }
                    } else if  (b.data.level.state == LevelState::laser){
                        if(b.data.level.laserActive){
                            //printf("FALLING");
                            a.texture = res.texDie;
                            a.curAnimation = res.ANIM_PLAYER_DIE;
                            
                            a.vel.x = -(a.vel.x);
                            
                            if(b.pos.y < a.pos.y ){
                                a.vel.y = (200.f);
                            } else {
                                a.vel.y = -(400.f);
                            }

                            //printf("x=%d, y=%d\n", a.pos.x, a.pos.y);
                            a.data.player.state = PlayerState::falling;
                    
                            
                        }
                    }
                    break;
                }
                case ObjectType::enemy: {
                    printf("Here");
                    if (b.data.enemy.state != EnemyState::dead) {
                        PlayerData &d = a.data.player;
                        d.healthPoints -= 1;
                        if (d.healthPoints <= 0) {
                            const float JUMP_DEAD = -350.0f;
                            d.state = PlayerState::dead;
                            a.texture = res.texDie;
                            a.curAnimation = res.ANIM_PLAYER_DIE;
                            a.vel.x = 0;
                            a.vel.y = JUMP_DEAD;
                        }
                    }
                    break;
                    /*if (b.data.enemy.state != EnemyState::dead) {
                        a.vel = glm::vec2(100, 0) * -a.dir;
                    }
                    break;*/ // this would push the player away when touching an enemy. its buggy
                }
            }
        }
        
    } else if (a.type == ObjectType::bullet) {
        bool passthrough = false;
        switch (a.data.bullet.state) {
            case BulletState::moving:
            {
                switch (b.type) {
                    case ObjectType::level: {
                        break;
                    }
                    case ObjectType::enemy: {
                        EnemyData &d = b.data.enemy;
                        if (d.state != EnemyState::dead) {
                            if (b.dir == a.dir) {
                                b.dir = -a.dir; // turn enemy around
                                b.vel.x = -b.vel.x;
                            }
                            b.shouldFlash = true;
                            b.flashTimer.reset();
                            // could change enemy sprite here if needed
                            d.state = EnemyState::damaged;
                            // damage enemy and flag dead if needed
                            d.healthPoints -= 1;
                            if (d.healthPoints <= 0) {
                                const float JUMP_DEAD = -10.0f;
                                d.state = EnemyState::dead;
                                b.texture = res.texSpinyDead;
                                b.curAnimation = res.ANIM_ENEMY_DEAD;
                                b.pos.y += JUMP_DEAD; // make the enemy jump up a bit when they die then pass thru the floor
                            }
                            b.vel.x += 25.0f * b.dir;
                        } else {
                            passthrough = true;
                        }
                        break;
                    }
                }
                if (b.type != ObjectType::player && !passthrough) {
                    genericResponse();
                    a.vel *= 0;
                    a.data.bullet.state = BulletState::colliding;
                    a.texture = res.texBulletHit;
                    a.curAnimation = res.ANIM_BULLET_HIT;
                    a.collider.x = a.collider.y = 0;
                    a.collider.w = a.collider.h = static_cast<float>(res.texBulletHit->h); // exploding sprite has new size
                }
                break;
            }
        }
    } else if (a.type == ObjectType::enemy) {
        // obj a is colliding with
        switch (b.type) {
            case ObjectType::level: {
                if (a.data.enemy.state != EnemyState::dead) {
                    genericResponse();
                    break;
                }
            }
            case ObjectType::enemy: {
                if (a.data.enemy.state != EnemyState::dead && b.data.enemy.state != EnemyState::dead) {
                    genericResponse();
                    break;
                }
            }
        } // if we need to do something different for player uncomment
    } 
}

void checkCollision(const SDLState &state, GameState &gs, const Resources &res, GameObject &a, GameObject &b, float deltaTime) {
    SDL_FRect rectA { // create rectangle c by intersecting a and b; if c exists, its height is y coordinates overlapping and width is x coordinates overlapping
        .x = a.pos.x + a.collider.x, 
        .y = a.pos.y + a.collider.y,
        .w = a.collider.w, 
        .h = a.collider.h
    };
    SDL_FRect rectB {
        .x = b.pos.x + b.collider.x, 
        .y = b.pos.y + b.collider.y,
        .w = b.collider.w, 
        .h = b.collider.h
    };
    SDL_FRect rectC{ 0 };
    if (SDL_GetRectIntersectionFloat(&rectA, &rectB, &rectC)) {
        // found intersection, respond accordingly
        collisionResponse(state, gs, res, rectA, rectB, rectC, a, b, deltaTime);
    }
}