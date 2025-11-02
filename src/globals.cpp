#include "../headers/globals.h"

int TILE_SIZE = 32;
int HOOK_SIZE = 16;
bool running = true;
float portalID = 0;
int selectedItem = 0;
bool waitingForKey = false;
bool hasNewKey = false;
float masterVolume = 100.0f;
float musicVolume = 100.0f;
float sfxVolume = 100.0f;
const float EPSILON = 0.01;
std::string username = "";
