#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"

class Engine
{
  public:
    Engine(std::string name, int width, int height);
    Engine(std::string name);
    ~Engine();
    bool Initialize(std::string objFilePath);
    void Run();
    void Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

  private:
    // Window related variables
    Window *m_window;
    std::string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;
};

#endif // ENGINE_H
