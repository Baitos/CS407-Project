#include "../headers/collision.h"
#include "../headers/object.h"
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/state.h"

void collisionCheckAndResponse(const SDLState &state, GameData &gd, Resources &res,
 	Player &a, float deltaTime)
 {
	
	for (Level &l : gd.mapTiles_){
		SDL_FRect rectA{
			.x = a.pos.x + a.collider.x,
			.y = a.pos.y + a.collider.y,
			.w = a.collider.w,
			.h = a.collider.h
		};
		SDL_FRect rectB{
			.x = l.pos.x + l.collider.x,
			.y = l.pos.y + l.collider.y,
			.w = l.collider.w,
			.h = l.collider.h
		};

		glm::vec2 resolution{ 0 };
		//glm::vec2 overlap = resolution;
		if (intersectAABB(rectA, rectB, resolution)){
			glm::vec2 overlap = resolution;
			// found intersection, respond accordingly
			if (overlap.x < overlap.y){
				if (a.pos.x < l.pos.x) {
					a.pos.x -= overlap.x;
				} else {
					a.pos.x += overlap.x;
				}	
				a.vel.x = 0;
			} else {
				if (a.pos.y < l.pos.y) {
					a.pos.y -= overlap.y;
                if (a.flip == 1) {
				    a.grounded = true;
					a.canDoubleJump = true;
					gd.player.gravityScale = 1.0f; // reset gravity
                }
			} else {
				a.pos.y += overlap.y;
                if (a.flip == -1) {
				    a.grounded = true;
					a.canDoubleJump = true;
					gd.player.gravityScale = 1.0f; // reset gravity
                }
			}
			a.vel.y = 0;
			}
		}
	}
	//glm::vec2 overlap = resolution;
	for (Laser &l : gd.lasers_){
		SDL_FRect rectA{
			.x = a.pos.x + a.collider.x,
			.y = a.pos.y + a.collider.y,
			.w = a.collider.w,
			.h = a.collider.h
		};
		SDL_FRect rectB{
			.x = l.pos.x + l.collider.x,
			.y = l.pos.y + l.collider.y,
			.w = l.collider.w,
			.h = l.collider.h
		};

		glm::vec2 resolution{ 0 };
		if (intersectAABB(rectA, rectB, resolution)){
			// found intersection, respond accordingly
			if(l.laserActive){
				//printf("FALLING");
				a.state_->nextStateVal = DEAD;
				PlayerState * newState = changePlayerState(a.state_);
				delete a.state_;
				a.state_ = newState;
				a.state_->enter(a, gd, res);
			
				a.vel.x = changeVel(-a.vel.x, a);
				float shouldFlip = a.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
				if(shouldFlip * l.pos.y < shouldFlip * a.pos.y ){
					a.vel.y = changeVel(200.f, a);
				} else {
					a.vel.y = changeVel(-400.f, a);
				}

				a.gravityScale = 1.0f;
			} 
		}
	}
	for (Portal &p : gd.portals_){
		SDL_FRect rectA{
			.x = a.pos.x + a.collider.x,
			.y = a.pos.y + a.collider.y,
			.w = a.collider.w,
			.h = a.collider.h
		};
		SDL_FRect rectB{
			.x = p.pos.x + p.collider.x,
			.y = p.pos.y + p.collider.y,
			.w = p.collider.w,
			.h = p.collider.h
		};

		glm::vec2 resolution{ 0 };
		if (intersectAABB(rectA, rectB, resolution)){
			// found intersection, respond accordingly
			if (p.isEntrance == true){
					a.pos = gd.ExitPortal;
			}
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

/*void checkCollision(const SDLState &state, GameData &gd, Resources &res,
	Object &a, Object &b, float deltaTime)
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
}*/