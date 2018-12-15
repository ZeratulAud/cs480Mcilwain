#include "graphics_headers.h"
#include "audio.h"

Audio::Audio()
{
    themeTrack = NULL;
    segmentTrack = NULL;
}

bool Audio::Initialize()
{
    //if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return false;

    const char* file = (ASSET_DIR + "bacmusic.wav").c_str();
    //themeTrack = Mix_LoadMUS(file);
    if (themeTrack == NULL)
        return false;

    return true;
}

bool Audio::PlayTheme()
{
    //if (Mix_PlayMusic(themeTrack, -1) == -1)
        return false;

    while (Mix_PlayingMusic());

    return true;
}