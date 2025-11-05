#pragma once
#include <SDL3/SDL.h>
#include <string>
class Sound {
    public:
        bool isMusic;
        Sound(std::string filepath, bool isMusic);
        ~Sound();
        void PlaySound();
        void StopSound();
        void SetupStream();
    private:
        SDL_AudioStream * stream;
        SDL_AudioDeviceID device;
        SDL_AudioSpec audioSpec;
        Uint8* waveStart;
        Uint32 waveLength;
};