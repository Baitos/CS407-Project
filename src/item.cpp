#include "../headers/gameData.h"
#include "../headers/resources.h"
#include "../headers/playerState.h"
#include "../headers/helper.h"
void useBomb(const SDLState &state, GameData &gd, Resources &res) {
    Item item = gd.player.item;
    int xdiff;
    // Set item 1 tile behind player, but we want it to be drawn at the beginning of tile
    if (gd.player.dir >= 0) { // moving right, place to left
        xdiff = 32 + (int)gd.player.pos.x % 32;
        item.pos.x = gd.player.pos.x - xdiff;
    }
    else {
        xdiff = 32 + (32- (int)gd.player.pos.x % 32);
        item.pos.x = gd.player.pos.x + xdiff;
    }
    item.pos.y = gd.player.pos.y;
    gd.items_.push_back(item);
}
void useBoombox(const SDLState &state, GameData &gd, Resources &res) {
    Item item = gd.player.item;

}
void useBouncyBall(const SDLState &state, GameData &gd, Resources &res) {}
void useFog(const SDLState &state, GameData &gd, Resources &res) {}
void useIce(const SDLState &state, GameData &gd, Resources &res) {}
void useMissile(const SDLState &state, GameData &gd, Resources &res) {}
void useSugar(const SDLState &state, GameData &gd, Resources &res) {
    printf("COKE\n");
}
void usePie(const SDLState &state, GameData &gd, Resources &res) {}

void clearItem(const SDLState &state, GameData &gd, Resources &res) {
    gd.itemStorage_.texture = res.texItemStorage;
    gd.itemStorage_.curAnimation = res.ANIM_ITEM_EMPTY;
    gd.itemStorage_.animations[gd.itemStorage_.curAnimation].reset();
    
}

void setItemPicked(GameData &gd, Resources &res, int index) {
        gd.itemStorage_.texture = res.itemTextures[index];
		gd.itemStorage_.curAnimation = res.ANIM_ITEM_PICKED;
        gd.itemStorage_.animations[gd.itemStorage_.curAnimation].reset();
}

void ItemStorage::update(const SDLState &state, GameData &gd, Resources &res, float deltaTime) {
    if (this->curAnimation != -1) {
        this->animations[this->curAnimation].step(deltaTime);
    }
    this->cycleTimer.step(deltaTime);
    if (this->cycleTimer.isTimeOut()) {
        // Reset timer and set item
        this->cycleTimer.reset();
        Item item = gd.player.nextItem;
        this->item = item;
        gd.player.item = item;
        gd.player.pickingItem = false;
        gd.player.hasItem = true;

        setItemPicked(gd, res, item.index);
    }
}

