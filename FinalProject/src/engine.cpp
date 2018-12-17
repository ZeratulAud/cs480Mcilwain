#include "engine.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

Engine::Engine(std::string name, int width, int height, int Level)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  currentLevel = Level;
  std::cout << currentLevel << std::endl;
}

Engine::Engine(std::string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;

  ambientVal = 0.0;
  diffuseVal = 0.1;
  specularVal = 0.6;
  shininessVal = 125.0;
  lightHeight = 10;
  plungerPower = 25;
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
  std::cout << currentLevel << std::endl;
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, currentLevel))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  /*
  m_audio = new Audio();
  if (!m_audio->Initialize())
  {
    printf("The audio failed to initialize.\n");
    return false;
  }
  m_audio->PlayTheme();
  */

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

bool Engine::GameOver()
{
  return (m_graphics->lives == 0);
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
    }

    // My menu
    ImGui::Begin("Menu");

    if (ImGui::Button("Switch Shader"))
      m_graphics->SwitchShader();

    if (ImGui::Button("Reset Light Values"))
    {

      ambientVal = 0.0;
      diffuseVal = 0.1;
      specularVal = 0.6;
      shininessVal = 125.0;
      lightHeight = 10;
      plungerPower = 0;
    }

    if (m_graphics->HasDied())
    {
      m_graphics->GetPlayer()->destroy = true;
      m_graphics->ResetPlayer();
      m_graphics->lives--;
    }

    if (GameOver())
    {
      if (ImGui::Button("Game over. Play Again?"))
      {
        m_graphics->lives = 3;
        m_graphics->gameScore = 0;
      }
    }

    ImGui::Text("Score: %d", m_graphics->gameScore);
    ImGui::Text("Lives: %d", m_graphics->lives);

    ImGui::SliderFloat("Ambient Value", &ambientVal, -1, 1, "%.05f");
    ImGui::SliderFloat("Diffuse Value", &diffuseVal, -1, 1, "%.05f");
    ImGui::SliderFloat("Specular Value", &specularVal, -1, 1, "%.05f");
    ImGui::SliderFloat("Shininess Value", &shininessVal, -200, 350, "%.0f");
    ImGui::SliderFloat("Light Height", &lightHeight, 0.1, 150, "%.0f");

    ImGui::End();

    // Update and render the graphics

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
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
      m_running = false;

    if (GameOver())
      return;

    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      case SDLK_DOWN:
        m_graphics->spawnBarrel(m_graphics->spawnlocation);
        break;

      case SDLK_w:
          m_graphics->jumpFlag = true;
          m_graphics->climbUpFlag=true;
        break;

      case SDLK_a:
          m_graphics->moveLeftFlag = true;
          m_graphics->climbLeftFlag = true;
        break;

      case SDLK_d:
          m_graphics->moveRightFlag = true;
          m_graphics->climbRightFlag = true;
        break;

      case SDLK_s:
          m_graphics->climbDownFlag = true;

        //m_graphics->dropBarrel();
        break;

      case SDLK_e:
        //m_graphics->resetBarrel();
        break;
    }
  }

  else if (m_event.type == SDL_KEYUP)
  {
    switch(m_event.key.keysym.sym)
    {

      case SDLK_a:
        m_graphics->moveLeftFlag = false;
        m_graphics->climbLeftFlag = false;
        break;

      case SDLK_d:
        m_graphics->moveRightFlag = false;
        m_graphics->climbRightFlag = false;
        break;

      case SDLK_w:
        m_graphics->jumpFlag = false;
        m_graphics->climbUpFlag = false;
        break;

      case SDLK_s:
        m_graphics->climbDownFlag = false;
    }
  }
}

void Engine::LightingUpdate()
{
  for (Object *obj : m_graphics->GetObjects())
  {
    obj->diffIntensity = diffuseVal;
    obj->specIntensity = specularVal;
    obj->shineIntensity = shininessVal;
  }
  m_graphics->ambIntensity = ambientVal;
  m_graphics->lightHeight = lightHeight;
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
