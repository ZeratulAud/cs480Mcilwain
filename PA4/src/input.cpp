
#include "input.h"

Input::Input()
{

}
Input::Input(Graphics *m_graphics, Engine *m_engine)
{
    graphics = m_graphics;
    engine = m_engine;
}

Input::~Input()
{
    //delete m_graphics;
    //m_graphics = NULL;
}

bool Input::Initialize()
{
    return true;
}

void Input::Keyboard(SDL_Event event)
{
    //printf( "event detected\n" );
    switch( event.type ){
        case SDL_KEYDOWN:
            printf( "KEYDOWN detected\n" );
            switch( event.key.keysym.sym ){
                case SDLK_LEFT:
                    graphics->m_planet->FlipPlanetRotation();
                    break;
                case SDLK_RIGHT:
                    graphics->m_planet->FlipOrbitRotation();
                    break;
                case SDLK_ESCAPE:
                    printf( "Trying to escape\n" );
                    engine->Quit();
                    break;
            }
            break;

        case SDL_KEYUP:
            printf( "Key release detected\n" );
            break;
        
        case SDL_MOUSEBUTTONDOWN:
            switch ( event.button.button) {
                case SDL_BUTTON_LEFT:
                    graphics->m_planet->TogglePlanetRotation();
                    break;

                case SDL_BUTTON_RIGHT:
                    graphics->m_planet->ToggleOrbitRotation();
                    break;
            }

            break;

        case SDL_MOUSEBUTTONUP:
            printf( "Mouse release detected\n" );
            switch ( event.button.button) {
                case SDL_BUTTON_LEFT:
                    //SDL_ShowSimpleMessageBox(0, "Mouse", "Left button was pressed!", window);
                    break;

                case SDL_BUTTON_RIGHT:
                    //SDL_ShowSimpleMessageBox(0, "Mouse", "Left button was pressed!", window);
                    break;
            }
            break;

        case SDL_QUIT:
            engine->Quit();
            break;
        
        default:
            break;
    }
}
