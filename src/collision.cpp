#include "../headers/collision.h"
#include "../headers/object.h"
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/state.h"

void collisionCheckAndResponse(const SDLState &state, GameData &gd, Resources &res,
 	Player &player, float deltaTime)
 {
	SDL_FRect rectA{
		.x = player.pos.x + player.collider.x,
		.y = player.pos.y + player.collider.y,
		.w = player.collider.w,
		.h = player.collider.h
	};
	for (Level &l : gd.mapTiles_){
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
				if (player.pos.x < l.pos.x) {
					player.pos.x -= overlap.x;
				} else {
					player.pos.x += overlap.x;
				}	
				player.vel.x = 0;
			} else {
				if (player.pos.y < l.pos.y) {
					player.pos.y -= overlap.y;
                if (player.flip == 1) {
				    player.grounded = true;
					player.canDoubleJump = true;
					gd.player.gravityScale = 1.0f; // reset gravity
                }
			} else {
				player.pos.y += overlap.y;
                if (player.flip == -1) {
				    player.grounded = true;
					player.canDoubleJump = true;
					gd.player.gravityScale = 1.0f; // reset gravity
                }
			}
			player.vel.y = 0;
			}
		}
	}
	//glm::vec2 overlap = resolution;
	for (Laser &l : gd.lasers_){
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
				player.state_->nextStateVal = DEAD;
				PlayerState * newState = changePlayerState(player.state_);
				delete player.state_;
				player.state_ = newState;
				player.state_->enter(player, gd, res);
			
				player.vel.x = changeVel(-player.vel.x, player);
				float shouldFlip = player.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
				if(shouldFlip * l.pos.y < shouldFlip * player.pos.y ){
					player.vel.y = changeVel(200.f, player);
				} else {
					player.vel.y = changeVel(-400.f, player);
				}

				player.gravityScale = 1.0f;
			} 
		}
	}
	for (Portal &p : gd.portals_){
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
					player.pos = gd.ExitPortal;
			}
		} 
	}
	for (ItemBox &box : gd.itemBoxes_) {

		SDL_FRect rectB{
			.x = box.pos.x + box.collider.x,
			.y = box.pos.y + box.collider.y,
			.w = box.collider.w,
			.h = box.collider.h
		};
		glm::vec2 resolution{ 0 };
		if (intersectAABB(rectA, rectB, resolution)) {
			// found intersection, respond accordingly
			if (box.itemBoxActive) {
				if (!player.pickingItem && !player.hasItem) {
        			box.generateItem(player, gd, res);
					gd.itemStorage_.texture = res.texItemRandomizer;
					gd.itemStorage_.curAnimation = res.ANIM_ITEM_CYCLE;
					gd.itemStorage_.animations[gd.itemStorage_.curAnimation].reset();
    			}
				box.itemBoxActive = false;
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