#include "../headers/soundManager.h"
#include <cstdio>

bool SoundManager::init() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        printf("Failed to init miniaudio engine.\n");
        return false;
    }
    return true;
}

void SoundManager::playMusic(const std::string& filePath, bool loop) {
    if (musicLoaded) {
        ma_sound_uninit(&music);
        musicLoaded = false;
    }

    if (ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &music) != MA_SUCCESS) {
        printf("Failed to load %s\n", filePath.c_str());
        return;
    }

    ma_sound_set_looping(&music, loop);
    ma_sound_start(&music);

    musicLoaded = true;
}

void SoundManager::stopMusic() {
    if (musicLoaded) {
        ma_sound_stop(&music);
    }
}

// dafe from one song to another
void SoundManager::fadeTo(const std::string& filePath, float seconds) {
    if (!musicLoaded) {
        playMusic(filePath, true);
        return;
    }

    ma_sound_set_fade_in_milliseconds(&music, 1.0f, 0, 0);
    ma_sound_set_fade_in_milliseconds(&music, 0.0f, 1.0f, seconds);
    ma_sound_stop(&music);

    playMusic(filePath, true);
}

void SoundManager::cleanup() {
    if (musicLoaded) {
        ma_sound_uninit(&music);
        musicLoaded = false;
    }
    ma_engine_uninit(&engine);
}