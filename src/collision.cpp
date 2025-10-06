#include "../headers/collision.h"
#include "../headers/object.h"
#include "../headers/player.h"

void collisionResponse(const SDLState &state, GameData &gd, Resources &res,
 	const SDL_FRect &rectA, const SDL_FRect &rectB, const glm::vec2 &overlap,
 	Object &a, Object &b, float deltaTime)
 {
// 	const auto genericResponse = [&]()
// 	{
// 		// horiz collision
// 		if (overlap.x < overlap.y)
// 		{
// 			if (a.pos.x < b.pos.x) // left
// 			{
// 				a.pos.x -= overlap.x;
// 			}
// 			else // right
// 			{
// 				a.pos.x += overlap.x;
// 			}
// 			a.vel.x = 0;
// 		}
//         // vert collision
// 		else
// 		{
// 			if (a.pos.y < b.pos.y) // top
// 			{
// 				a.pos.y -= overlap.y;
//                 if (a.flip == 1) {
// 				    a.grounded = true;
//                 }
// 			}
// 			else // bottom
// 			{
// 				a.pos.y += overlap.y;
//                 if (a.flip == -1) {
// 				    a.grounded = true;
//                 }
// 			}
// 			a.vel.y = 0;
// 		}
// 	};
//     // obj we are checking
//     if (a.type == ObjectType::player) {
//         if (a.data.player.state != PlayerState::dead) {
//             // obj a is colliding with
//             switch (b.type) {
//                 case ObjectType::level: {
//                     if(b.data.level.state == LevelState::ground){
//                         genericResponse();
                        
//                     } else if (b.data.level.state == LevelState::portal ){
//                         //Teleport to Exit Portal
//                         if (b.data.level.isEntrance == true){
//                             a.pos = gs.ExitPortal;
//                         }
//                     }
//                     break;
//                 }
//                 case ObjectType::obstacle: {
//                         if(b.data.obstacle.laserActive){
//                             //printf("FALLING");
//                             a.texture = res.texDie;
//                             a.curAnimation = res.ANIM_PLAYER_DIE;
                            
//                             a.vel.x = changeVel(-a.vel.x, a);
//                             float shouldFlip = a.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
//                             if(shouldFlip * b.pos.y < shouldFlip * a.pos.y ){
//                                 a.vel.y = changeVel(200.f, a);
//                             } else {
//                                 a.vel.y = changeVel(-400.f, a);
//                             }

//                             //printf("x=%d, y=%d\n", a.pos.x, a.pos.y);
//                             a.data.player.state = PlayerState::falling;
//                             a.gravityScale = 1.0f;
//                         } 
//                         break;
//                 }
//             }
//         }
//     } else if (a.type == ObjectType::bullet) {
//         bool passthrough = false;
//         switch (a.data.bullet.state) {
//             case BulletState::moving:
//             {
//                 switch (b.type) {
//                     case ObjectType::level: {
//                         break;
//                     }
//                 }
//                 if (b.type != ObjectType::player && !passthrough) {
//                     genericResponse();
//                     a.vel *= 0;
//                     a.data.bullet.state = BulletState::colliding;
//                     a.texture = res.texBulletHit;
//                     a.curAnimation = res.ANIM_BULLET_HIT;
//                     a.collider.x = a.collider.y = 0;
//                     a.collider.w = a.collider.h = static_cast<float>(res.texBulletHit->h); // exploding sprite has new size
//                 }
//                 break;
//             }
//         }
//     } 
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
	/*SDL_FRect rectA{
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
	}*/
}