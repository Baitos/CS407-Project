#include "../headers/sound.h"

Sound::Sound(std::string filepath, bool isMusic) {
    if(!SDL_LoadWAV(filepath.c_str(), &audioSpec, &waveStart, &waveLength)) {
        printf("Couldn't load audio\n");
        exit(1);
    }
    this->isMusic = isMusic;
}
Sound::~Sound() {
    SDL_free(waveStart);
    SDL_DestroyAudioStream(stream);
}

void Sound::PlaySound() {
    //int status = SDL_QueueAudio
    SDL_ResumeAudioStreamDevice(stream);
}

void Sound::SetupStream() {
    // device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec);
    // if (!device) {
    //     printf("error setting up audio device\n");
    //     exit(1);
    // }
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec, NULL, NULL);
}

// static void SDLCALL MyAudioCallback(void *userdata, SDL_AudioStream * stream, int len)
// {
//      // I have audio data in a global, MyAudioBuffer, that I want to adjust and copy into stream...
//     Uint8 *buffer = (Uint8 *) SDL_calloc(1, len);  // SDL_calloc zeroes out the buffer after allocating.
//     if (buffer) {
//         SDL_MixAudio(buffer, stream, MyAudioBufferFormat, len, MyVolume);
//         // okay, _now_ we're ready to put the mixed data into the stream!
//         SDL_PutAudioStreamData(stream, buffer, len);
//         SDL_free(buffer);
//     }
// }