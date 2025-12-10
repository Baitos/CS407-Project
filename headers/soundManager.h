#pragma once
#include "miniaudio.h"
#include <string>

class SoundManager {
public:
    bool init();
    void playMusic(const std::string& filePath, bool loop = true);
    void stopMusic();
    void fadeTo(const std::string& filePath, float seconds = 0.5f);
    void cleanup();
    void setMusicVolume(float volume);

private:
    ma_engine engine{};
    ma_sound music{};
    bool musicLoaded = false;
};