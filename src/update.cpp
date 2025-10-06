#include "../headers/update.h"
#include "../headers/collision.h"
#include "../headers/gameData.h"
#include "../headers/initState.h"
#include "../headers/helper.h"
#include "../headers/globals.h"
#include "../headers/resources.h"
#include "../headers/state.h"
#include "../headers/playerState.h"

using namespace std;

extern GameState *currState;

// Update Function for level Spaceship
void levelUpdate(const SDLState &state, GameData &gd, Resources &res, float deltaTime)
{
    // update portals
    for (Portal &portal : gd.portals_)
    {
        portal.update(state, gd, res, deltaTime);
    }

    // update lasers
    for (Laser &laser : gd.lasers_)
    {
        laser.update(state, gd, res, deltaTime);
    }
    // update item boxes if on cooldown
    for (ItemBox &box : gd.itemBoxes_)
    {
        if (!box.itemBoxActive)
        {
            box.update(state, gd, res, deltaTime);
        }
    }
}

float updatePlayer(const SDLState &state, GameData &gd, Resources &res, Player &player, float deltaTime, float currentDirection)
{
    if (player.isDead)
    {
        // player is dead, reset map

        /* Death Timer currently disabled

        Timer &deathTimer = player.data.player.deathTimer;
        deathTimer.step(deltaTime);
        if (deathTimer.isTimeOut()) {
            running = false; // exit program
        */

        running = false;
        return 0;
    }

    if (state.keys[SDL_SCANCODE_A])
    {
        currentDirection += -1;
    }
    if (state.keys[SDL_SCANCODE_D])
    {
        currentDirection += 1;
    }
    /* TODO Weapontimer
    Timer &weaponTimer = player.weaponTimer;
    weaponTimer.step(deltaTime);
    */

    switch (player.state_->type)
    {
    case IDLE:
    {
        if (currentDirection)
        { // if moving change to running
            player.state_->type = MOVING;
        }

        player.texture = res.texIdle;
        player.curAnimation = res.ANIM_PLAYER_IDLE;
        // handleShooting();
        break;
    }
    case MOVING:
    {
        if (!currentDirection && player.grounded)
        { // if not moving, slow down
            const float factor = player.vel.x > 0 ? -1.0f : 1.0f;
            float amount = factor * player.acc.x * deltaTime;
            if (std::abs(player.vel.x) < std::abs(amount))
            {
                player.vel.x = 0;
                player.state_->type = IDLE; // once stopped, set player to idle
            }
            else
            {
                player.vel.x += amount;
            }
        }
        if (isSliding(player) && player.grounded)
        { // moving in different direction of vel and pressing a direction, sliding
            player.texture = res.texSlide;
            player.curAnimation = res.ANIM_PLAYER_SLIDE;
        }
        else
        {
            player.texture = res.texRun;
            player.curAnimation = res.ANIM_PLAYER_WALK;
        }
        /* TODO Sprinting
        if (state.keys[SDL_SCANCODE_LSHIFT])
        { // if not pressing then reset
            float LEEWAY = 20;
            if (player.grounded && std::abs(player.vel.x) >= (player.maxSpeedX - LEEWAY))
            { // if grounded and moving fast enter sprint (eventually)
                if (!player.sprintTimer.isTimeOut())
                {
                    player.sprintTimer.step(deltaTime);
                }
                else
                {
                    player.maxSpeedX = player.data.player.maxSprintX;
                    player.data.player.state = PlayerState::sprinting;
                }
            }
        }
        else
        {
            player.data.player.sprintTimer.reset();
        }
        */
        // handleShooting();
        break;
    }
        /* TODO Movement States
        case SPRINTING:
        {
            float LEEWAY = 20;
            if (player.grounded && // if on ground and sliding or too slow reset sprint
                (isSliding(player) || std::abs(player.vel.x) < (player.maxSpeedX - LEEWAY)))
            {
                player.data.player.sprintTimer.reset();
                player.maxSpeedX = player.data.player.maxRunX;
                player.data.player.state = PlayerState::moving;
            }
            handleShooting();
            break;
        }
        case PlayerState::jumping:
        {
            player.texture = res.texJump;
            player.curAnimation = res.ANIM_PLAYER_JUMP;
            handleShooting();
            break;
        }
        case PlayerState::falling:
        {
            player.texture = res.texDie;
            player.curAnimation = res.ANIM_PLAYER_DIE;
            break;
        }
        case PlayerState::jumpLaunch:
        {
            player.texture = res.texLaunch;
            player.curAnimation = res.ANIM_PLAYER_LAUNCH;

            // once the launch animation finishes, switch to normal in-air "jump"
            if (player.animations[player.curAnimation].isDone())
            {
                player.data.player.state = PlayerState::jumping;
                player.texture = res.texJump;
                player.curAnimation = res.ANIM_PLAYER_JUMP;
                player.animations[player.curAnimation].reset();
            }
            handleShooting();
            break;
        }
        case PlayerState::roll:
        {
            player.texture = res.texRoll;
            player.curAnimation = res.ANIM_PLAYER_ROLL;

            // when roll animation finishes, switch to moving
            if (player.animations[player.curAnimation].isDone())
            {
                player.data.player.state = PlayerState::moving;
                player.texture = res.texRun;
                player.curAnimation = res.ANIM_PLAYER_WALK;
                player.animations[player.curAnimation].reset();
            }
            handleShooting(); // optional: allow shooting while rolling
            break;
        }
        }
        */
        if (std::abs(player.pos.y) > 1500)
        {
            player.isDead = true; // die if you fall off
            player.texture = res.texDie;
            player.curAnimation = res.ANIM_PLAYER_DIE;
            player.vel.x = 0;
        }
        // printf("Player x = %f, Player y = %f\n", player.pos.x, player.pos.y);
    }
    return currentDirection;
}

/* TODO
void handleShooting(SDLState &state, GameData &gd, Resources &res, Player &player, Timer &weaponTimer)
   {
        if (state.keys[SDL_SCANCODE_J])
        {
            // bullets!
            // in 2.5 hour video, go to 1:54:19 if you want to sync up shooting sprites with animations for running
            if (weaponTimer.isTimeOut())
            { instanceof
                if (player.data.player.state == PlayerState::idle)
                {
                    player.texture = res.texShoot;
                    player.curAnimation = res.ANIM_PLAYER_SHOOT;
                }
                if (player.curAnimation == res.ANIM_PLAYER_SLIDE)
                {
                    return;
                }
                if (player.curAnimation == res.ANIM_PLAYER_JUMP)
                {
                    player.texture = res.texShootJump;
                    player.curAnimation = res.ANIM_PLAYER_SHOOT_JUMP;
                }
                else
                {
                    player.texture = res.texShoot;
                    player.curAnimation = res.ANIM_PLAYER_SHOOT;
                }
                weaponTimer.reset();
                /*
                Gameplayerect bullet;
                bullet.data.bullet = BulletData();
                bullet.type = playerectType::bullet;
                bullet.dir = gd.player().dir;
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
                const float t = (player.dir + 1) / 2.0f; // results in 0 to 1
                const float xOffset = left + right * t; // LERP between left and right
                const float yVariation = 40;
                const float yVelocity = SDL_rand(yVariation) - yVariation / 2.0f;
                bullet.vel = glm::vec2(
                player.vel.x + 300.0f * player.dir, yVelocity);
                //printf("bullet.vel.x = %f\n", bullet.vel.x);
                bullet.maxSpeedX = 5000.0f;
                bullet.animations = res.bulletAnims;
                bullet.pos = glm::vec2(
                    player.pos.x + xOffset,
                    player.pos.y + TILE_SIZE / 2 + 1
                );
                // try to reuse old inactive bullets
                bool foundInactive = false;
                for (int i = 0; i < gd.bullets.size() && !foundInactive; i++) {
                    if (gd.bullets[i].data.bullet.state == BulletState::inactive) {
                        foundInactive = true;
                        gd.bullets[i] = bullet;
                    }
                }
                // otherwise push new bullet
                if (!foundInactive) {
                    gd.bullets.push_back(bullet);
                }
            }

            }
        };
    */
// float updateBullet(const SDLState &state, GameData &gd, Resources &res, Gameobject &obj, float deltaTime, float currentDirection)
// {
//     switch (obj.data.bullet.state)
//     {
//     case BulletState::moving:
//     {
//         if (obj.pos.x - gd.mapViewport.x < 0 ||          // left side
//             obj.pos.x - gd.mapViewport.x > state.logW || // right side
//             obj.pos.y - gd.mapViewport.y < 0 ||          // up
//             obj.pos.y - gd.mapViewport.y > state.logH)   // down
//         {
//             obj.data.bullet.state = BulletState::inactive;
//         }
//         break;
//     }
//     case BulletState::colliding:
//     {
//         if (obj.animations[obj.curAnimation].isDone())
//         {
//             obj.data.bullet.state = BulletState::inactive;
//         }
//     }
//     }
//     return currentDirection;
// }

// TODO Update? Not sure how this is supposed to work now tbh - Rei
// void update(const SDLState &state, GameData &gd, Resources &res, AnimatedObject &obj, float deltaTime)
// {
//     // update animation
//     if (obj.curAnimation != -1)
//     {
//         obj.animations[obj.curAnimation].step(deltaTime);
//     }
//     if (obj.dynamic && !obj.grounded)
//     {
//         obj.vel.y += changeVel(700 * obj.gravityScale * deltaTime, obj); // gravity
//         // printf("x=%d, y=%d\n", obj.pos.x, obj.pos.y);
//     }
//     float currentDirection = 0;
//     switch (obj.type)
//     {
//     /*case objectType::Player:
//     {
//         currentDirection = updatePlayer(state, gd, res, obj, deltaTime, currentDirection);
//         break;
//     }*/
//     case objectType::bullet:
//     {
//         updateBullet(state, gd, res, obj, deltaTime, currentDirection);
//         break;
//     }
//     case objectType::obstacle:
//     {
//         updateObstacle(state, gd, res, obj, deltaTime, currentDirection);
//         break;
//     }
//     }

//     if (currentDirection)
//     {
//         obj.dir = currentDirection;
//     }
//     obj.vel += currentDirection * obj.acc * deltaTime;
//     if (std::abs(obj.vel.x) > obj.maxSpeedX)
//     {
//         if (!isSliding(obj))
//         { // if not sliding slow down
//             obj.vel.x -= 1.5 * obj.acc.x * deltaTime * currentDirection;
//         }
//     }

//     // add vel to pos
//     obj.pos += obj.vel * deltaTime;
//     // collision
//     bool foundGround = obj.grounded;
//     obj.grounded = false;
//     for (Gameobject &objB : gd.mapTiles)
//     { // check if obj is touching any map tiles, currently no enemy collision
//         if (obj.dynamic && isOnscreen(state, gd, obj) && isOnscreen(state, gd, objB))
//         {
//             checkCollision(state, gd, res, obj, objB, deltaTime);
//         }
//         else if (obj.type == objectType::bullet)
//         {
//             checkCollision(state, gd, res, obj, objB, deltaTime);
//         }
//     }
//     for (Gameobject &objB : gd.lasers)
//     {
//         checkCollision(state, gd, res, obj, objB, deltaTime);
//     }

//     // TODO Delete? IDK what this is for - Rei
//     /*if (obj.grounded && !foundGround) {
//         if (obj.grounded && obj.type == objectType::obj) {
//             if ((obj.data.obj.state == objState::jumping && obj.data.obj.fastfalling)|| obj.data.obj.state == objState::falling) {
//                 obj.data.obj.state = objState::roll;
//                 obj.texture = res.texRoll;
//                 obj.curAnimation = res.ANIM_obj_ROLL;
//                 obj.animations[obj.curAnimation].reset();
//             } else {
//                 obj.data.obj.state = objState::moving;
//             }

//             obj.data.obj.fastfalling = false;
//             obj.data.obj.canDoubleJump = true;
//             obj.gravityScale = 1.0f;
//         }
//     }*/
// }

void handleCrosshair(const SDLState &state, GameData &gd, Resources &res, float deltaTime)
{
    SDL_GetMouseState(&gd.mouseCoords.x, &gd.mouseCoords.y);
    float CROSSHAIR_SIZE = 15;
    float OFFSET = 7;
    float yRatio = (float)state.logH / state.height;
    float xRatio = (float)state.logW / state.width;
    gd.mouseCoords.x = gd.mouseCoords.x * xRatio;
    gd.mouseCoords.y = gd.mouseCoords.y * yRatio;
    SDL_FRect dst{
        .x = gd.mouseCoords.x - OFFSET,
        .y = gd.mouseCoords.y - OFFSET,
        .w = CROSSHAIR_SIZE,
        .h = CROSSHAIR_SIZE};
    SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255); // draw line to crosshair
    glm::vec2 pOffset = findCenterOfSprite(gd.player);
    // printf("x: %d, y: %d\n", pOffset.x, pOffset.y);
    SDL_RenderLine(state.renderer, gd.player.pos.x - gd.mapViewport.x + pOffset.x, gd.player.pos.y - gd.mapViewport.y + pOffset.y, gd.mouseCoords.x, gd.mouseCoords.y);
    SDL_SetRenderDrawColor(state.renderer, 64, 51, 83, 255);

    // printf("mouseX: %f, mouseY: %f\n", gd.mouseCoords.x, gd.mouseCoords.y);
    SDL_RenderTexture(state.renderer, res.texCrosshair, nullptr, &dst); // src is for sprite stripping, dest is for where sprite should be drawn*/
}

void handleKeyInput(const SDLState &state, GameData &gd, Resources &res,
                    SDL_KeyboardEvent key, bool keyDown, float deltaTime)
{

    if (key.scancode == SDL_SCANCODE_F12 && key.down && !key.repeat)
    { // debug
        gd.debugMode = !gd.debugMode;
    }
    if (key.scancode == SDL_SCANCODE_F11 && key.down && !key.repeat)
    { // tp to entrance portal
        gd.player.pos = gd.EntrancePortal;
        gd.player.pos.x -= TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_F10 && key.down && !key.repeat)
    { // tp to exit portal
        gd.player.pos = gd.ExitPortal;
    }
    /*if (key.scancode == SDL_SCANCODE_F2 && keyDown && !key.repeat) { // anti gravity
        gd.obj().flip = -1 * gd.obj().flip;
    }*/
    if (key.scancode == SDL_SCANCODE_F1)
    {
        running = false;
    }
    if (key.scancode == SDL_SCANCODE_D && key.down)
    {
        gd.player.pos.x += 2 * TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_A && key.down)
    {
        gd.player.pos.x -= 2 * TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_W && key.down)
    {
        gd.player.pos.y -= 2 * TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_S && key.down)
    {
        gd.player.pos.y += 2 * TILE_SIZE;
    }
    if (key.scancode == SDL_SCANCODE_F2)
    {
        printf("F2 key clicked");
        currState = changeState(currState);
        currState->init(state, gd, res);
    }

    // if (obj.type == objectType::obj) {
    //     const float JUMP_FORCE = -450.f;
    //     const auto handleJumping = [&state, &gd, &obj, res, key, keyDown, JUMP_FORCE]() {
    //         if (key.scancode == SDL_SCANCODE_SPACE && keyDown && !key.repeat) { // jumping
    //             if (obj.grounded) { // single jump
    //                 //add something for jump animation before going up
    //                 //obj.data.obj.state = objState::jumping;
    //                  obj.data.obj.state = objState::jumpLaunch; // <-- use launch state
    //                  obj.texture = res.texLaunch;
    //                  obj.curAnimation = res.ANIM_obj_LAUNCH;
    //                  obj.animations[obj.curAnimation].reset();
    //                  obj.vel.y = changeVel(JUMP_FORCE, obj);
    //             } else if (obj.data.obj.canDoubleJump) { // double jump
    //                 obj.data.obj.state = obState::jumping;
    //                 ob.vel.y = changeVel(JUMP_FORCE, ob);
    //                 ob.data.ob.canDoubleJump = false;
    //                 ob.gravityScale = 1.0f; // reset gravity
    //             }
    //             //printf("canDoubleJump = %d\n" , ob.data.ob.canDoubleJump);
    //         }
    //         else if (!keyDown && key.scancode == SDL_SCANCODE_SPACE) { // letting go of jump
    //             /*if (ob.vel.y < 0) { // OPTION 1: Set velocity to 0 when you let go. makes sharp but precise jumps
    //                 ob.vel.y = 0;
    //             }*/
    //             float termVel = -200.0f; // option 2: Set velocity to predefined amount when you let go. makes less sharp jumps
    //             float shouldFlip = ob.flip; // there might be a more modular way to do this. idk if we will actually use the gravity flip but having it is nice and cool
    //             if (shouldFlip * ob.vel.y < shouldFlip * termVel) {
    //                 ob.vel.y = changeVel(termVel, ob);
    //             }
    //             //ob.gravityScale = 2.0f; // option 3; double their gravity until they land
    //         }
    //     };
    //     const auto handleRunning = [&state, &gd, &player, &res, key, keyDown, deltaTime]() {
    //         if (key.scancode == SDL_SCANCODE_LSHIFT) {
    //             if (keyDown) { // if held down, increase speed
    //                 player.maxSpeedX = player.data.player.maxRunX;
    //                 player.curAnimation = res.ANIM_PLAYER_RUN;
    //             } else {
    //                 player.maxSpeedX = player.data.player.maxWalkX;
    //                 player.curAnimation = res.ANIM_PLAYER_WALK;
    //                 player.data.player.sprintTimer.reset();
    //             }
    //         }
    //     };
    //     const auto handleSprinting = [&state, &gd, &player, &res, key, keyDown]() {
    //         if (key.scancode == SDL_SCANCODE_LSHIFT && !keyDown) {
    //             player.maxSpeedX = player.data.player.maxWalkX;
    //             player.curAnimation = res.ANIM_PLAYER_WALK;
    //             player.data.player.sprintTimer.reset();
    //             player.data.player.state = PlayerState::moving;
    //         }
    //     };
    //     const auto handleFalling = [&state, &gd, &player, &res, key, keyDown, deltaTime]() {
    //         if (key.scancode == SDL_SCANCODE_S && keyDown && !player.grounded) { // fastfall
    //             if (!key.repeat && !player.data.player.fastfalling) {
    //                 player.vel.y = changeVel(-250.0f, player);
    //                 player.data.player.fastfalling = true;
    //                 player.data.player.state = PlayerState::jumping;
    //                 player.curAnimation = res.ANIM_PLAYER_JUMP;
    //                 //player.data.player.canDoubleJump = false;
    //             }
    //             player.gravityScale = 3.0f;
    //         }
    //     };
    //     switch (player.data.player.state) {
    //         case PlayerState::idle:
    //         {
    //             handleJumping();
    //             handleRunning();
    //             break;
    //         }
    //         case PlayerState::moving:
    //         {
    //             handleJumping();
    //             handleRunning();
    //             handleFalling();
    //             break;
    //         }
    //         case PlayerState::jumping:
    //         {
    //             handleRunning();
    //             handleJumping();
    //             handleFalling();
    //             break;
    //         }
    //         case PlayerState::jumpLaunch:
    //         {
    //             handleRunning();
    //             handleJumping();
    //             handleFalling();
    //             break;
    //         }
    //         case PlayerState::roll:
    //         {
    //             handleRunning();
    //             handleJumping();
    //             handleFalling();
    //             break;
    //         }
    //         case PlayerState::sprinting:
    //         {
    //             handleSprinting();
    //             handleJumping();
    //             handleFalling();
    //             break;
    //         }
    //     }
    //     //printf("velX = %f, velY = %f\n", player.vel.x, player.vel.y);
    // }
}

// Input Function for level Spaceship
void levelInputs(SDLState &state, GameData &gd, Resources &res, float deltaTime)
{
    SDL_Event event{0};
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
        {
            running = false;
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED:
        {
            state.width = event.window.data1;
            state.height = event.window.data2;
            // printf("Width = %d, Height = %d", state.width, state.height);
            break;
        }
        case SDL_EVENT_KEY_DOWN:
        {
            handleKeyInput(state, gd, res, event.key, true, deltaTime);

            break;
        }
        case SDL_EVENT_KEY_UP:
        {
            handleKeyInput(state, gd, res, event.key, false, deltaTime);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        {
            // handleClick(state, gd, res, gd.player(), deltaTime);
            break;
        }
        }
    }
}