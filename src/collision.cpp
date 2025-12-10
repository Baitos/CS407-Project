#include "../headers/collision.h"
#include "../headers/object.h"
#include "../headers/player.h"
#include "../headers/playerState.h"
#include "../headers/state.h"
#include "../headers/helper.h"
#include <algorithm>

extern GameState * currState;


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
		(minYA < maxYB && maxYA > minYB))
	{
		overlap.x = std::min(maxXA - minXB, maxXB - minXA);
		overlap.y = std::min(maxYA - minYB, maxYB - minYA);
		return true;
	}
	return false;
}

void Player::collisionResponse(const SDLState &state, GameData &gd, Resources &res,
 	Object &o, SDL_FRect &rectA, SDL_FRect &rectB, glm::vec2 &resolution, float deltaTime)
{
	switch (o.type) {
		case ICE_BLOCK:
		{
			//printf("player collided with ice\n");
			Level& l = dynamic_cast<Level&>(o);
			if (resolution.x < resolution.y) {
				
				if (this->pos.x < l.pos.x) {
					this->pos.x -= resolution.x;
				} else {
					this->pos.x += resolution.x;
				}	
				this->vel.x = 0;
			} else {
				if (this->pos.y < l.pos.y) {
					this->pos.y -= resolution.y;
				} else {
					this->pos.y += resolution.y;
				}
				this->vel.y = 0;
			}
			break;
		}
		case LEVEL:
		{
			//printf("player collided with level\n");
			Level& l = dynamic_cast<Level&>(o);
			if (resolution.x < resolution.y) {
				
				if (this->pos.x < l.pos.x) {
					this->pos.x -= resolution.x;
				} else {
					this->pos.x += resolution.x;
				}	
				this->vel.x = 0;
			} else {
				if (this->pos.y < l.pos.y) {
					this->pos.y -= resolution.y;
				} else {
					this->pos.y += resolution.y;
				}
				this->vel.y = 0;
			}
			break;
		}	
		case LASER: 
		{
			//printf("player collided with laser\n");
			Laser& la = dynamic_cast<Laser&>(o);
			if (la.laserActive){
				//printf("FALLING");
				sfxSound.playMusic("data/Audio/laser.wav", false);
				PlayerState* stState = new StunnedState();
                this->handleState(stState, gd, res);
			
				this->vel.x = changeVel(-this->vel.x, (*this));
				float shouldFlip = this->flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
				if(shouldFlip * o.pos.y < shouldFlip * this->pos.y){
					this->vel.y = changeVel(200.f, (*this));
				} else {
					this->vel.y = changeVel(-400.f, (*this));
				}

				this->gravityScale = 1.0f;
			} 
			break;
		}
		case PORTAL:
		{
			//printf("player collided with portal\n");
			Portal& po = dynamic_cast<Portal&>(o);
			if (po.isEntrance == true){
				this->pos = gd.ExitPortal;
			}
			break;
		}
		case WATER:
		{
			//Water& w = dynamic_cast<Water&>(o);
			slowObject(this->vel, deltaTime);
			break;
		}
		case LAVA:
		{
			//Lava& l = dynamic_cast<Lava&>(o);
			if(this->state_->stateVal != DEAD) {
				/*set player to dead and start the respawn counter*/
				PlayerState* dState = new DeadState();
				this->handleState(dState, gd, res);
			}
			break;
		}
		case REVOLVER: 
		{
			//printf("player collided with laser\n");
			Revolver& r = dynamic_cast<Revolver&>(o);
			if (r.inUse) {
				break;
			}
			// if a player touches a revolver not in use
			this->pos = r.pos + glm::vec2(TILE_SIZE / 2);
			this->grounded = false;
			PlayerState* stState = new StunnedState(true); // put in hard stun
			this->handleState(stState, gd, res);
			this->visible = false; // make player invisible
			r.inUse = true; // set in use to true
			r.spinning = true; // start spinning
			r.player = this; // set revolver pointer
			break;
		}
		case CACTUS: 
		{
			if (resolution.x < resolution.y) {
				
				if (this->pos.x < o.pos.x) {
					this->pos.x -= resolution.x;
				} else {
					this->pos.x += resolution.x;
				}	
				this->vel.x = 0;
			} else {
				if (this->pos.y < o.pos.y) {
					this->pos.y -= resolution.y;
				} else {
					this->pos.y += resolution.y;
				}
				this->vel.y = 0;
			}
			PlayerState* stState = new StunnedState();
			this->handleState(stState, gd, res);
			this->vel.x = changeVel(-this->vel.x, (*this));
			float shouldFlip = this->flip;
			if(shouldFlip * o.pos.y < shouldFlip * this->pos.y){
				this->vel.y = changeVel(200.f, (*this));
			} else {
				this->vel.y = changeVel(-400.f, (*this));
			}

			this->gravityScale = 1.0f;
			break;
		}
		case ITEMBOX:
		{
			ItemBox& box = dynamic_cast<ItemBox&>(o);
			if (box.itemBoxActive) {
				sfxSound.playMusic("data/Audio/item.wav", false);
				if (!this->pickingItem && !this->hasItem) {
        			box.generateItem((*this), gd, res);
					gd.itemStorage_.texture = res.texItemRandomizer;
					gd.itemStorage_.curAnimation = res.ANIM_ITEM_CYCLE;
					gd.itemStorage_.animations[gd.itemStorage_.curAnimation].reset();
    			}
				box.itemBoxActive = false;
			}
			break;
		}
	}
}

void Player::groundedCheck(Object &o, SDL_FRect &rectB, float deltaTime) {
	// grounded sensor
	const float inset = 2.0;
	SDL_FRect sensor {
		.x = this->pos.x + this->collider.x + 1,
		.y = this->pos.y + this->collider.y + this->collider.h,
		.w = this->collider.w - inset,
		.h = EPSILON
	};
	glm::vec2 resolution{ 0 };
	if (intersectAABB(sensor, rectB, resolution)) {
		if(o.type == ICE_BLOCK) {
			speedObject(this->vel, deltaTime);
		}
		this->grounded = true;
		this->canDoubleJump = true;
		this->gravityScale = 1.0f;
	}
}

void Player::checkCollision(const SDLState &state, GameData &gd, Resources &res,
 	float deltaTime)
{
	SDL_FRect rectA {
		.x = this->pos.x + this->collider.x,
		.y = this->pos.y + this->collider.y,
		.w = this->collider.w,
		.h = this->collider.h
	};
	glm::vec2 resolution{ 0 };
	std::vector<Object *> closeTiles_ = getCloseTiles(state, gd, this->pos);
	for (auto &o : closeTiles_) { 
		SDL_FRect rectB {
			.x = o->pos.x + o->collider.x,
			.y = o->pos.y + o->collider.y,
			.w = o->collider.w,
			.h = o->collider.h
		};
		if (intersectAABB(rectA, rectB, resolution)) {
			this->collisionResponse(state, gd, res, (*o), rectA, rectB, resolution, deltaTime);
		}
		if (o->type == LEVEL || o->type == ICE_BLOCK) {
			this->groundedCheck((*o), rectB, deltaTime);
		}
	}

		
	//check for weapon deployment
	for (Player &p2 : gd.players_) {
		if(p2.index != this->index) {
			//check if in shotgun player blast state
			if(ShotgunDeployState *s = dynamic_cast<ShotgunDeployState*>(p2.state_)) {
				sfxSound.playMusic("data/Audio/shotgun.wav", false);
				//check if currently blocking/deploying sword
				if(!dynamic_cast<SwordDeployState*> (this->state_)) {
					SDL_FRect rectB{
						.x = s->blast->pos.x + s->blast->collider.x,
						.y = s->blast->pos.y + s->blast->collider.y,
						.w = s->blast->collider.w,
						.h = s->blast->collider.h
					};
					glm::vec2 resolution{ 0 };
					if (intersectAABB(rectA, rectB, resolution)){
						//printf("Player %d shot by player %d\n", p.index, p2.index);
						PlayerState* stState = new StunnedState();
						this->isDead = true;
						this->handleState(stState, gd, res);
						
						this->vel.x = 0;
						this->vel.y = 0;
					}
				} else {
					//sword blocks
					//printf("Player %d BLOCKED shot by player %d\n", p.index, p2.index);
				}
			} else if (SwordDeployState *s = dynamic_cast<SwordDeployState*>(p2.state_)) {
				sfxSound.playMusic("data/Audio/sword.wav", false);
				//check if in sword player swing state
				SDL_FRect rectB{
					.x = p2.pos.x + this->collider.x - 5,
					.y = p2.pos.y + this->collider.y - 5,
					.w = p2.collider.w + 10,
					.h = p2.collider.h + 10
				};
				glm::vec2 resolution{ 0 };
				if (intersectAABB(rectA, rectB, resolution)){
					//printf("Player %d hit by player %d's sword\n", p.index, p2.index);
					PlayerState* stState = new StunnedState();
					this->isDead = true;
					this->handleState(stState, gd, res);
					
					this->vel.x = 0;
					this->vel.y = 0;
				}
			}
		}
	}
	//check for checkpoint system
	for (Checkpoint cp: gd.checkpoints_) {
		if (cp.index == (this->lastCheckpoint+1)%(gd.checkpoints_.size())){
			SDL_FRect rectB = cp.collider;
			//printf("checking %d\n", cp.index);
			glm::vec2 resolution{ 0 };
			if (intersectAABB(rectA, rectB, resolution)){
				printf("INTERSECT!!!");
				if(this->lastCheckpoint == gd.checkpoints_.size() - 1) {
					this->lapsCompleted++;
					printf("Completed the %dth lap!\n", this->lapsCompleted);
					//check if finished, add to placement
					if(this->lapsCompleted == gd.laps_per_race) {
						sfxSound.playMusic("data/Audio/race finish.wav", false);
						gd.num_finished++;
						gd.player_placement_.push_back(*this);
					} else {
						sfxSound.playMusic("data/Audio/lap finish.wav", false);
					}
				}
				this->lastCheckpoint = (this->lastCheckpoint+1)%(gd.checkpoints_.size());
				printf("passed checkpoint %d\n%f, %f\n%f, %f", this->lastCheckpoint, cp.collider.x, cp.collider.y, this->pos.x, this->pos.y);
			}
		}
	}
	//check for game 
	if(!gd.round_is_over) {
		if(gd.players_.size() == 1) {
			if(gd.num_finished == 1) {
				printf("\n\nGAME OVER\n\n");
				gd.round_is_over = true;
			}
		} else {
			if(gd.num_finished >= gd.players_.size()-1) {
				printf("\n\nGAME OVER (multiple player)\n\n");
				bool inArray = false;
				for(Player p: gd.player_placement_) {
					if(this->index == p.index){
						inArray = true;
						break;
					}
				}
				if(!inArray) {
					gd.player_placement_.push_back(*this);
				}
				gd.round_is_over = true;
			}
		}
	}

}

void Hook::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    if (!(this->visible)) {
        return;
    }
	SDL_FRect rectA {
		.x = this->pos.x + this->collider.x,
		.y = this->pos.y + this->collider.y,
		.w = this->collider.w,
		.h = this->collider.h
	};
    SDL_FRect rectB;
    glm::vec2 resolution{ 0 };
	std::vector<Object *> closeTiles_ = getCloseTiles(state, gd, this->pos);
	for (auto &o : closeTiles_) { 
		SDL_FRect rectB {
			.x = o->pos.x + o->collider.x,
			.y = o->pos.y + o->collider.y,
			.w = o->collider.w,
			.h = o->collider.h
		};
		if (o->type == LEVEL) {
			if (intersectAABB(rectA, rectB, resolution)) {
				this->vel = glm::vec2(0);
				if (!this->collided) {
					sfxSound.playMusic("data/Audio/grapple.wav", false);
					PlayerState* grappleState = new GrappleState();
					p.handleState(grappleState, gd, res);
					this->collided = true;
				}
			}
		}
		else if (o->type == ITEMBOX)
		{
			ItemBox& box = dynamic_cast<ItemBox&>(*o);
			if (intersectAABB(rectA, rectB, resolution) && box.itemBoxActive) {
				if (!p.pickingItem && !p.hasItem) {
					sfxSound.playMusic("data/Audio/item.wav", false);
        			box.generateItem(p, gd, res);
					gd.itemStorage_.texture = res.texItemRandomizer;
					gd.itemStorage_.curAnimation = res.ANIM_ITEM_CYCLE;
					gd.itemStorage_.animations[gd.itemStorage_.curAnimation].reset();
    			} else {
					sfxSound.playMusic("data/Audio/grapple.wav", false);
				}
				box.itemBoxActive = false;
				removeHook(p);
			}
		}
		else if (o->type == CACTUS) {
			if (intersectAABB(rectA, rectB, resolution)) {
				removeHook(p);
			}
			
		}
	}

    for (Player &p2 : gd.players_) {
        if (&p != &p2) { // do not check on self
            rectB = {
                .x = p2.pos.x + p2.collider.x,
                .y = p2.pos.y + p2.collider.y,
                .w = p2.collider.w,
                .h = p2.collider.h
            };
            if (intersectAABB(rectA, rectB, resolution) && !this->collided) {
				sfxSound.playMusic("data/Audio/grapple.wav", false);
                p.vel = 0.7f * this->vel;
                p2.vel = -0.3f * this->vel;
                removeHook(p);
                removeHook(p2);
                PlayerState* stunState = new StunnedState();
                p2.handleState(stunState, gd, res); // stun player you hit and disable their hook
            }
            Hook h2 = p2.hook;
            rectB = {
                .x = h2.pos.x + h2.collider.x,
                .y = h2.pos.y + h2.collider.y,
                .w = h2.collider.w,
                .h = h2.collider.h
            };
            if (intersectAABB(rectA, rectB, resolution) && !this->collided && h2.visible) { // Touching other hook
				sfxSound.playMusic("data/Audio/grapple.wav", false);
                removeHook(p);
                removeHook(p2);
            }
        } 
    }
}

void Bomb::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    SDL_FRect rectA{
		.x = this->pos.x + this->collider.x,
		.y = this->pos.y + this->collider.y,
		.w = this->collider.w,
		.h = this->collider.h
	};
    SDL_FRect rectB;
    glm::vec2 resolution{ 0 };
    for (Player &p2 : gd.players_) {
        // Don't do collision if already stunned
        if (p2.state_->stateVal == PlayerStateValue::STUNNED) {
            continue;
        }
		if (&p == &p2 && !this->leniencyTimer.isTimeOut()) {// dont collide w/ player who placed at start
			continue;
		}
        rectB = {
            .x = p2.pos.x + p2.collider.x,
            .y = p2.pos.y + p2.collider.y,
            .w = p2.collider.w,
            .h = p2.collider.h
        };
        if (intersectAABB(rectA, rectB, resolution) && this->visible) {
			sfxSound.playMusic("data/Audio/bomb.wav", false);
            p2.vel.y = -200.0f; 
            p2.vel.x = this->vel.x * 0.5;  
            p2.pos.y -= 1;              
            p2.grounded = false;

            PlayerState* stunState = new StunnedState(true); // hard stun
            p2.handleState(stunState, gd, res); // stun player you hit
            if (this->exploded) {
                break;
            }
            this->exploded = true;
            SDL_FRect expCollider = {
                .x = 0,
                .y = 0,
                .w = (float)TILE_SIZE * 5,
                .h = (float)TILE_SIZE * 5
            };
            this->collider = expCollider;
            this->pos.x -= TILE_SIZE * 2;
            this->pos.y -= TILE_SIZE * 2;
            this->texture = res.texExplosion;
            this->width = 156;
            this->height = 156;
            this->animations = res.itemAnims;
            this->curAnimation = res.ANIM_ITEM_EXPLOSION;
            this->animations[this->curAnimation].reset();
        }
    } 
}

void Boombox::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
     SDL_FRect rectA{
		.x = this->pos.x + this->collider.x,
		.y = this->pos.y + this->collider.y,
		.w = this->collider.w,
		.h = this->collider.h
	};
    SDL_FRect rectB;
    glm::vec2 resolution{ 0 };
    for (Player &p2 : gd.players_) {
        // Don't do collision on self or if already stunned
        if (&p2 == &p || p2.state_->stateVal == PlayerStateValue::STUNNED) {
            continue;
        }
        rectB = {
            .x = p2.pos.x + p2.collider.x,
            .y = p2.pos.y + p2.collider.y,
            .w = p2.collider.w,
            .h = p2.collider.h
        };
        if (intersectAABB(rectA, rectB, resolution) && this->visible) {
            p2.vel.y = -200.0f; 
            p2.vel.x = this->vel.x * 0.5;  
            p2.pos.y -= 1;              
            p2.grounded = false;

            PlayerState* stunState = new StunnedState(true); // hard stun
            p2.handleState(stunState, gd, res); // stun player you hit
        }
    }
}

void handleBallBounce(glm::vec2 &resolution, Ball &b, Object &o) {
	if (resolution.x < resolution.y) { // horiz
		if (b.pos.x < o.pos.x) {
			//printf("left ball\n");
			b.pos.x -= resolution.x + EPSILON;
		} else {
			//printf("right ball\n");
			b.pos.x += resolution.x + EPSILON;
		}	
		b.vel.x *= -1; 
		b.dir = b.dir * -1;
	} else { // vert
		if (b.pos.y < o.pos.y) {
			//printf("above ball\n");
			b.pos.y -= resolution.y + EPSILON;
		} else {
			//printf("below ball\n");
			b.pos.y += resolution.y + EPSILON;
		}
		b.vel.y *= -1;
	}
}

void Ball::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    SDL_FRect rectA{
		.x = this->pos.x + this->collider.x,
		.y = this->pos.y + this->collider.y,
		.w = this->collider.w,
		.h = this->collider.h
	};
    SDL_FRect rectB;
    glm::vec2 resolution{ 0 };

	std::vector<Object *> closeTiles_ = getCloseTiles(state, gd, this->pos);
	for (auto &o : closeTiles_) { 
		if (o->type == LEVEL || o->type == CACTUS || o->type == ICE_BLOCK) {
			SDL_FRect rectB {
				.x = o->pos.x + o->collider.x,
				.y = o->pos.y + o->collider.y,
				.w = o->collider.w,
				.h = o->collider.h
			};
			if (intersectAABB(rectA, rectB, resolution)) {
				sfxSound.playMusic("data/Audio/ball.wav", false);
				handleBallBounce(resolution, (*this), (*o));
			}
		}
	}

    for (Player &p2 : gd.players_) {
		if (&p == &p2 && !this->leniencyTimer.isTimeOut()) {// dont collide w/ player who threw at start
			continue;
		}
		rectB = {
			.x = p2.pos.x + p2.collider.x,
			.y = p2.pos.y + p2.collider.y,
			.w = p2.collider.w,
			.h = p2.collider.h
		};
		if (intersectAABB(rectA, rectB, resolution)) {
			if (p2.state_->stateVal == STUNNED) { // dont hit a stunned player again
				continue;
			}
			p2.vel.y = -200.0f; 
			p2.vel.x = this->vel.x * 0.5;  
			p2.pos.y -= 1;              
			p2.grounded = false;
			PlayerState* stunState = new StunnedState(true); // hard stun
			p2.handleState(stunState, gd, res); // stun player you hit
			handleBallBounce(resolution, (*this), p);
			this->bounces += 1;
			//printf("ball bounce %d\n", this->bounces);
			if (this->bounces >= this->MAX_BOUNCES) {
				this->active = false;
			}
		}
    }
}

void Pie::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    SDL_FRect rectA{
		.x = this->pos.x + this->collider.x,
		.y = this->pos.y + this->collider.y,
		.w = this->collider.w,
		.h = this->collider.h
	};
    SDL_FRect rectB;
    glm::vec2 resolution{ 0 };

	std::vector<Object *> closeTiles_ = getCloseTiles(state, gd, this->pos);
	for (auto &o : closeTiles_) { 
		if (o->type == LEVEL || o->type == CACTUS || o->type == ICE_BLOCK) {
			SDL_FRect rectB {
				.x = o->pos.x + o->collider.x,
				.y = o->pos.y + o->collider.y,
				.w = o->collider.w,
				.h = o->collider.h
			};
			if (intersectAABB(rectA, rectB, resolution)) {
				sfxSound.playMusic("data/Audio/pie.wav", false);
				this->active = false;
			}
		}
	}

    for (Player &p2 : gd.players_) {
        if (&p != &p2) { // do not check on self
            rectB = {
                .x = p2.pos.x + p2.collider.x,
                .y = p2.pos.y + p2.collider.y,
                .w = p2.collider.w,
                .h = p2.collider.h
            };
            if (intersectAABB(rectA, rectB, resolution)) {
				sfxSound.playMusic("data/Audio/pie.wav", false);
                p2.vel.y = -200.0f; 
                p2.vel.x = this->vel.x * 0.5;  
                p2.pos.y -= 1;              
                p2.grounded = false;

                PlayerState* stunState = new StunnedState(true); // hard stun
                p2.handleState(stunState, gd, res); // stun player you hit

                this->active = false;
            }
        } 
    }
}

void Ice::checkCollision(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    SDL_FRect rectA {
		.x = this->pos.x + this->collider.x,
		.y = this->pos.y + this->collider.y,
		.w = this->collider.w,
		.h = this->collider.h
	};
    SDL_FRect rectB;
    glm::vec2 resolution{ 0 };
	int AMOUNT = 4;
	for (Player &p2 : gd.players_) { // does affect player who used it for now
		rectB = {
			.x = p2.pos.x + p2.collider.x,
			.y = p2.pos.y + p2.collider.y,
			.w = p2.collider.w,
			.h = p2.collider.h
		};
		if (intersectAABB(rectA, rectB, resolution)) {
			slowObject(p2.vel, deltaTime * AMOUNT); // slow them down a lot
		}
		Hook h2 = p2.hook;
		rectB = {
			.x = h2.pos.x + h2.collider.x,
			.y = h2.pos.y + h2.collider.y,
			.w = h2.collider.w,
			.h = h2.collider.h
		};
		if (intersectAABB(rectA, rectB, resolution) && h2.visible) { 
			slowObject(p2.hook.vel, deltaTime * AMOUNT); // slow hook down too
		}
    }
}
