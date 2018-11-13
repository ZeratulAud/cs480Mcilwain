#include "engine.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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

  ambientVal = 0.0;
  diffuseVal = 0.2;
  specularVal = 0.6;
  shininessVal = 125.0;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
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

  ImGui::CreateContext();
  ImGuiIO& imgui_io = ImGui::GetIO(); (void)imgui_io;
  ImGui_ImplSDL2_InitForOpenGL(m_window->getSDLWindow(), m_window->getGLContext());
  ImGui_ImplOpenGL3_Init("#version 130"); // GL 3.0 + GLSL 130
  ImGui::StyleColorsDark(); // Set

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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window->getSDLWindow());
    ImGui::NewFrame();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
      Camera();
    }

    // My menu
    ImGui::Begin("Lighting menu");

    if (ImGui::Button("Switch Shader"))
    {
      m_graphics->SwitchShader();
    }

    if (ImGui::Button("Reset Light Values"))
    {

      ambientVal = 0.0;
      diffuseVal = 0.2;
      specularVal = 0.6;
      shininessVal = 125.0;
      lightHeight = 25;
    }
    LightingUpdate();

    ImGui::SliderFloat("Ambient Value", &ambientVal, -2, 2, "%.05f");
    ImGui::SliderFloat("Diffuse Value", &diffuseVal, -2, 2, "%.05f");
    ImGui::SliderFloat("Specular Value", &specularVal, -2, 2, "%.05f");
    ImGui::SliderFloat("Shininess Value", &shininessVal, -200, 350, "%.0f");

    ImGui::End();

    // Update and render the graphics
    m_graphics->flipPaddle(m_DT);
    m_graphics->Update(m_DT);
   
    m_graphics->Render();

    LightingUpdate();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
        m_graphics->paddleFlag = true;
        break;



      // Stop program
      case SDLK_ESCAPE:
        m_running = false;
        break;
    }

  }
  if (m_event.type == SDL_KEYUP)
  {
    switch(m_event.key.keysym.sym)
    {
      case SDLK_SPACE:
        m_graphics->paddleFlag = false;
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

void Engine::LightingUpdate()
{
  for (Object *obj : m_graphics->GetObjects())
  {
    obj->ambIntensity = ambientVal;
    obj->diffIntensity = diffuseVal;
    obj->specIntensity = specularVal;
    obj->shineIntensity = shininessVal;
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
