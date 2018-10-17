
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
        m_graphics->timeScale += 0.1;
        break;
      case SDLK_LEFT:
        m_graphics->timeScale -= 0.1;
        break;
      case SDLK_UP:
        m_graphics->orbitScale -= 0.5;
        break;
      case SDLK_DOWN:
        m_graphics->orbitScale += 0.5;
        if (m_graphics->orbitScale < 0.5)
          m_graphics->orbitScale = 0.5;
        break;
      case SDLK_SPACE:
        m_graphics->timeScale = 1;
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

  // Keyboard
  else if (m_event.type == SDL_KEYDOWN)
  {
    glm::mat4 model;
    glm::vec3 vec;

    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      case SDLK_w:
        m_graphics->m_camera->translate.x += 1.0;
        break;

      case SDLK_a:
        m_graphics->m_camera->translate.z -= 1.0;
        break;

      case SDLK_s:
        m_graphics->m_camera->translate.x -= 1.0;
        break;

      case SDLK_d:
        m_graphics->m_camera->translate.z += 1.0;
        break;

      case SDLK_0:
        model = m_graphics->GetSun()->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_1:
        model = m_graphics->GetSun()->children[0]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_2:
        model = m_graphics->GetSun()->children[1]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_3:
        model = m_graphics->GetSun()->children[2]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_4:
        model = m_graphics->GetSun()->children[3]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_5:
        model = m_graphics->GetSun()->children[4]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_6:
        model = m_graphics->GetSun()->children[5]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_7:
        model = m_graphics->GetSun()->children[6]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_8:
        model = m_graphics->GetSun()->children[7]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;

      case SDLK_9:
        model = m_graphics->GetSun()->children[8]->GetModel();
        vec = glm::vec3(model[3]);
        m_graphics->m_camera->SnapToPlanet(vec.x+100, vec.y+100, vec.z,
                                           vec.x, vec.y, vec.z,
                                           0.0f, 1.0f, 0.0f);
        break;
    }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  //assert(TimeNowMillis >= m_currentTimeMillis);
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
