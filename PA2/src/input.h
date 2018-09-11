#ifndef INPUT_H
#define INPUT_H

#include "window.h"
#include "graphics.h"
#include "engine.h"

class Input
{
    public:
    	Input();
		Input(Graphics *m_graphics, Engine *m_engine);
		~Input();
		bool Initialize();
		void Keyboard(SDL_Event m_event);

    private:

		SDL_Event event;
		Engine *engine;
		Graphics *graphics;
};

#endif // INPUT_H