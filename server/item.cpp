#include "../serverHeaders/gameData.h"
#include "../serverHeaders/resources.h"
void useBomb(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    printf("BOOOOOOM\n");
}

void useBoombox(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    printf("BUMP THAT SHIT\n");
}
void useBouncyBall(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useFog(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useIce(const SDLState &state, GameData &gd, Resources &res, Player &p) {}
void useMissile(const SDLState &state, GameData &gd, Resources &res, Player &p) {}

void useSugar(const SDLState &state, GameData &gd, Resources &res, Player &p) {
    ((Sugar *) (&p.item))->sugarTimer.reset();
    p.usingSugar = true;
    //printf("COKE\n");
}
void usePie(const SDLState &state, GameData &gd, Resources &res, Player &p) {}

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

void ItemStorage::update(const SDLState &state, GameData &gd, Resources &res, Player &p, float deltaTime) {
    if (this->curAnimation != -1) {
        this->animations[this->curAnimation].step(deltaTime);
    }
    this->cycleTimer.step(deltaTime);
    if (this->cycleTimer.isTimeOut()) {
        // Reset timer and set item
        this->cycleTimer.reset();
        Item item = p.nextItem;
        this->item = item;
        p.item = item;
        p.pickingItem = false;
        p.hasItem = true;

        setItemPicked(gd, res, item.index);
    }
}