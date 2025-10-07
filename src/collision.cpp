#include "../headers/collision.h"
#include "../headers/object.h"
#include "../headers/player.h"
#include "../headers/playerState.h"

void collisionResponse(const SDLState &state, GameData &gd, Resources &res,
 	const SDL_FRect &rectA, const SDL_FRect &rectB, const glm::vec2 &overlap,
 	Player &a, Object &b, float deltaTime)
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
        if (a.state_->currStateVal != DEAD) {
            // obj a is colliding with
            switch (b.type) {
                case LEVEL: {
                    genericResponse();
                    break;
                }
				case PORTAL:
				{
                        if (((Portal)(b)).isEntrance == true){
                            a.pos = gd.ExitPortal;
                        }
                    }
				}
                case LASER: {
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
	Object &a, Object &b, float deltaTime)
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