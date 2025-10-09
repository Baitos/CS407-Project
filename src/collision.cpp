#include "../headers/collision.h"
#include "../headers/object.h"
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/state.h"
#include "../headers/helper.h"

void collisionCheckAndResponse(const SDLState &state, GameData &gd, Resources &res,
 	Player &player, float deltaTime)
{
	SDL_FRect rectA {
		.x = player.pos.x + player.collider.x,
		.y = player.pos.y + player.collider.y,
		.w = player.collider.w,
		.h = player.collider.h
	};
	glm::vec2 resolution{ 0 };
	for (Level &l : gd.mapTiles_){
		SDL_FRect rectB{
			.x = l.pos.x + l.collider.x,
			.y = l.pos.y + l.collider.y,
			.w = l.collider.w,
			.h = l.collider.h
		};
		if (intersectAABB(rectA, rectB, resolution)){
			// found intersection, respond accordingly
			if (resolution.x < resolution.y) {
				if (player.pos.x < l.pos.x) {
					player.pos.x -= resolution.x;
				} else {
					player.pos.x += resolution.x;
				}	
				player.vel.x = 0;
			} else {
				if (player.pos.y < l.pos.y) {
					player.pos.y -= resolution.y;
                	if (player.flip == 1) {
						player.grounded = true;
						player.canDoubleJump = true;
						gd.player.gravityScale = 1.0f; // reset gravity
                	}
				} else {
					player.pos.y += resolution.y;
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
	//glm::vec2 resolution = resolution;
	for (Laser &l : gd.lasers_){
		resolution = glm::vec2(0);
		SDL_FRect rectB{
			.x = l.pos.x + l.collider.x,
			.y = l.pos.y + l.collider.y,
			.w = l.collider.w,
			.h = l.collider.h
		};
		if (intersectAABB(rectA, rectB, resolution)){
			// found intersection, respond accordingly
			if(l.laserActive){
				//printf("FALLING");
				gd.player.state_ = changePlayerState(gd, res, player.state_, DEAD);
			
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
		resolution = glm::vec2(0);
		SDL_FRect rectB{
			.x = p.pos.x + p.collider.x,
			.y = p.pos.y + p.collider.y,
			.w = p.collider.w,
			.h = p.collider.h
		};
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

bool deltaIntersectAABB(SDLState &state, Object &a, Object &b, glm::vec2 &overlap, float deltaTime) {
	glm::vec2 delta = updatePos(a, deltaTime); // attempt to move forward
	SDL_FPoint vertices[4];
	vertices[0].x = a.pos.x + a.collider.x;
	vertices[0].y = a.pos.y + a.collider.y;
	
	vertices[1].x = a.pos.x + a.collider.x;
	vertices[1].y = a.pos.y + a.collider.y + a.collider.h;
	
	vertices[2].x = a.pos.x + a.collider.x + a.collider.w;
	vertices[2].y = a.pos.y + a.collider.y;
	
	vertices[3].x = a.pos.x + a.collider.x + a.collider.w;
	vertices[3].y = a.pos.y + a.collider.y + a.collider.h;


	// SDL_Vertex vertices[6]; // create shape of delta between two positions, vertices are left to right, top to bottom (if you drew the shape)
	// vertices[0].position.x = a.pos.x;
	// vertices[0].position.y = a.pos.y;
	// vertices[1].position.x = a.pos.x + a.collider.h;
	// vertices[1].position.y = a.pos.y + a.collider.h;
	// vertices[2].position.x = a.pos.x + a.collider.x;
	// vertices[2].position.y = a.pos.y;

	// vertices[3].position.x = vertices[1].position.x + delta.x;
	// vertices[3].position.y = vertices[1].position.y + delta.y;
	// vertices[4].position.x = vertices[2].position.x + delta.x;
	// vertices[4].position.y = vertices[2].position.y + delta.y;
	// vertices[5].position.x = vertices[0].position.x + delta.x + a.collider.x;
	// vertices[5].position.y = vertices[0].position.y + delta.y + a.collider.y;
	//SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 150);
	//SDL_RenderGeometry(state.renderer, nullptr, vertices, 6, nullptr, 4);
	//SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);

	SDL_FRect rectB {
		.x = b.pos.x + b.collider.x,
		.y = b.pos.y + b.collider.y,
		.w = b.collider.w,
		.h = b.collider.h
	};
	return false;
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