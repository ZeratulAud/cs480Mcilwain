#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Audio
{
    public:
        Audio();
        bool Initialize();
        bool PlayTheme();
        bool PlaySegment();

    private:
        Mix_Music *themeTrack;
        Mix_Chunk *segmentTrack;
};

#endif // AUDIO_H