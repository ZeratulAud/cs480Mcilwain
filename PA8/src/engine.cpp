#include "engine.h"

Engine::Engine(std::string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(std::string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
      Camera();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
    m_running = false;

  // Keyboard
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      case SDLK_RIGHT:
        m_graphics->GetDynamicsWorld()->setGravity(btVector3(0, -1, 1));
        break;

      case SDLK_LEFT:
        m_graphics->GetDynamicsWorld()->setGravity(btVector3(0, -1, -1));
        break;

      case SDLK_UP:
        m_graphics->GetDynamicsWorld()->setGravity(btVector3(1, -1, 0));
        break;

      case SDLK_DOWN:
        m_graphics->GetDynamicsWorld()->setGravity(btVector3(-1, -1, 0));
        break;

      case SDLK_SPACE:
        //m_graphics->dynamicsWorld.x = 1;
        break;

      // Stop program
      case SDLK_ESCAPE:
        m_running = false;
        break;
    }
  }
}

void Engine::Camera()
{
  if(m_event.type == SDL_QUIT)
    m_running = false;

  else if (m_event.type == SDL_KEYDOWN)
  {
    switch(m_event.key.keysym.sym)
    {
      case SDLK_w:
        m_graphics->GetCamera()->translate.x -= 1.0;
        break;

      case SDLK_a:
        m_graphics->GetCamera()->translate.z += 1.0;
        break;

      case SDLK_s:
        m_graphics->GetCamera()->translate.x += 1.0;
        break;

      case SDLK_d:
        m_graphics->GetCamera()->translate.z -= 1.0;
        break;

      case SDLK_q:
        m_graphics->GetCamera()->translate.y -= 1.0;
        break;

      case SDLK_e:
        m_graphics->GetCamera()->translate.y += 1.0;
        break;
    }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
