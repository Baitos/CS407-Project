#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <format>

#include "../headers/collision.h"

void groundedCheck(const SDLState &state, GameData &gd, const Resources &res, GameObject &a, GameObject &b, float deltaTime) {
    bool foundGround = false;
    // grounded sensor
    const float inset = 2.0;
    SDL_FRect sensor {
        .x = a.pos.x + a.collider.x + 1,
        .y = a.flip == -1 ? a.pos.y : a.pos.y + a.collider.y + a.collider.h, // flip checker if flipped
        .w = a.collider.w - inset,
        .h = 1
    };
    SDL_FRect rectB {
        .x = b.pos.x + b.collider.x,
        .y = b.pos.y + b.collider.y,
        .w = b.collider.w,
        .h = b.collider.h
    };
    SDL_FRect rectC { 0 };
    if (SDL_GetRectIntersectionFloat(&sensor, &rectB, &rectC)) {
        foundGround = true;
    }
    if (a.grounded != foundGround) { // changing state
        a.grounded = foundGround;
        if (foundGround && a.data.player.state != PlayerState::dead) {
            a.data.player.state = PlayerState::moving;
            a.data.player.fastfalling = false;
            a.data.player.canDoubleJump = true;
            a.gravityScale = 1.0f;
        }
    }
}

void collisionResponse(const SDLState &state, GameData &gd, Resources &res,
	const SDL_FRect &rectA, const SDL_FRect &rectB, const glm::vec2 &overlap,
	GameObject &a, GameObject &b, float deltaTime)
{
	const auto genericResponse = [&]()
	{
		// horiz collision
		if (overlap.x < overlap.y)
		{
			if (a.pos.x < b.pos.x) // left
			{
				a.pos.x -= overlap.x;
			}
			else // right
			{
				a.pos.x += overlap.x;
			}
			a.vel.x = 0;
		}
        // vert collision
		else
		{
			if (a.pos.y < b.pos.y) // top
			{
				a.pos.y -= overlap.y;
                if (a.flip == 1) {
				    a.grounded = true;
                }
			}
			else // bottom
			{
				a.pos.y += overlap.y;
                if (a.flip == -1) {
				    a.grounded = true;
                }
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
                            a.pos = gd.ExitPortal;
                        }
                    }
                    break;
                }
                case ObjectType::obstacle: {
                        if(b.data.obstacle.laserActive){
                            //printf("FALLING");
                            a.texture = res.texDie;
                            a.curAnimation = res.ANIM_PLAYER_DIE;
                            
                            a.vel.x = changeVel(-a.vel.x, a);
                            float shouldFlip = a.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
                            if(shouldFlip * b.pos.y < shouldFlip * a.pos.y ){
                                a.vel.y = changeVel(200.f, a);
                            } else {
                                a.vel.y = changeVel(-400.f, a);
                            }

                            //printf("x=%d, y=%d\n", a.pos.x, a.pos.y);
                            a.data.player.state = PlayerState::falling;
                            a.gravityScale = 1.0f;
                        } 
                        break;
                }
                case ObjectType::enemy: {
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

bool intersectAABB(const SDL_FRect &a, const SDL_FRect &b, glm::vec2 &overlap)
{
	const float minXA = a.x;
	const float maxXA = a.x + a.w;
	const float minYA = a.y;
	const float maxYA = a.y + a.h;
	const float minXB = b.x;
	const float maxXB = b.x + b.w;
	const float minYB = b.y;
	const float maxYB = b.y + b.h;

	if ((minXA < maxXB && maxXA > minXB) &&
		(minYA <= maxYB && maxYA >= minYB))
	{
		overlap.x = std::min(maxXA - minXB, maxXB - minXA);
		overlap.y = std::min(maxYA - minYB, maxYB - minYA);
		return true;
	}
	return false;
}

void checkCollision(const SDLState &state, GameData &gd, Resources &res,
	GameObject &a, GameObject &b, float deltaTime)
{
	SDL_FRect rectA{
		.x = a.pos.x + a.collider.x,
		.y = a.pos.y + a.collider.y,
		.w = a.collider.w,
		.h = a.collider.h
	};
	SDL_FRect rectB{
		.x = b.pos.x + b.collider.x,
		.y = b.pos.y + b.collider.y,
		.w = b.collider.w,
		.h = b.collider.h
	};

	glm::vec2 resolution{ 0 };
	if (intersectAABB(rectA, rectB, resolution))
	{
		// found intersection, respond accordingly
		collisionResponse(state, gd, res, rectA, rectB, resolution, a, b, deltaTime);
	}
}