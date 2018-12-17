#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"
#include "audio.h"

class Engine
{
  public:
    Engine(std::string name, int width, int height, int level);
    Engine(std::string name);
    ~Engine();
    bool Initialize();
    bool GameOver();
    void Run();
    void Keyboard();
    void LightingUpdate();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

  private:
    float ambientVal = 0;
    float diffuseVal = 0.2;
    float specularVal = 0.6;
    float shininessVal = 125;
    float lightHeight = 10;
    float plungerPower = 0;

    // Window related variables
    Window *m_window;
    std::string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;

    int currentLevel;

    SDL_Event m_event;

    Graphics *m_graphics;
    Audio *m_audio;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;
};

#endif // ENGINE_H
