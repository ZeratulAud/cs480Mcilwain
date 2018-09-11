#include "object.h"
#include "engine.h"

Object::Object(float mult)
{  
	/*
	# Blender File for a Cube
	o Cube
	v 1.000000 -1.000000 -1.000000
	v 1.000000 -1.000000 1.000000
	v -1.000000 -1.000000 1.000000
	v -1.000000 -1.000000 -1.000000
	v 1.000000 1.000000 -0.999999
	v 0.999999 1.000000 1.000001
	v -1.000000 1.000000 1.000000
	v -1.000000 1.000000 -1.000000
	s off
	f 2 3 4
	f 8 7 6
	f 1 5 6
	f 2 6 7
	f 7 8 4
	f 1 4 8
	f 1 2 4
	f 5 8 6
	f 2 1 6
	f 3 2 7
	f 3 7 4
	f 5 1 8
	*/

	Vertices = {
	{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
	{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
	{{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
	{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
	{{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
	{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
	{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
	{{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
	};

	Indices = {
	2, 3, 4,
	8, 7, 6,
	1, 5, 6,
	2, 6, 7,
	7, 8, 4,
	1, 4, 8,
	1, 2, 4,
	5, 8, 6,
	2, 1, 6,
	3, 2, 7,
	3, 7, 4,
	5, 1, 8
	};

	// The index works at a 0th index
	for(unsigned int i = 0; i < Indices.size(); i++)
	{
		Indices[i] = Indices[i] - 1;
	}

	orbitRotationalSpeed = mult;
	planetRotationalSpeed = mult;
	orbitAngle = 0.0f;
	planetAngle = 0.0f;

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
	Vertices.clear();
	Indices.clear();
}

void Object::Update(unsigned int dt)
{
	/*while( SDL_PollEvent( &event ) ){
	printf( "Event detected\n" );
		switch( event.type ){
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym ){
					case SDLK_LEFT:
						planetRotationalSpeed = -planetRotationalSpeed;
						break;
					case SDLK_RIGHT:
						orbitRotationalSpeed = -orbitRotationalSpeed;
						break;
				}
				break;

			case SDL_KEYUP:
				printf( "Key release detected\n" );
				break;

			default:
				break;
		}
	}*/
	if (!stopOrbit)
		orbitAngle += dt * M_PI/1000 * orbitRotationalSpeed;
	if (!stopPlanet)
		planetAngle += dt * M_PI/1000 * planetRotationalSpeed;

	model = glm::rotate(glm::mat4(1.0f), (orbitAngle/25), glm::vec3(0.0, 1.0, 0.0))*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 10.0));
	model = glm::rotate(model, (-planetAngle/1), glm::vec3(0.0, 1.0, 0.0));
}
	/*}
	//SDL_PumpEvents() 
	state = SDL_GetKeyboardState(NULL);
	//angle += dt * M_PI/1000 * angleMultiplier;

	if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)))//((!state[SDL_SCANCODE_RETURN])||)
	{
		orbitAngle += dt * M_PI/1000 * orbitRotationalSpeed;
	}
	else
	{
		orbitAngle -= dt * M_PI/1000 * orbitRotationalSpeed;
	}
	if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)))
	{
		planetAngle += dt * M_PI/1000 * planetRotationalSpeed;
	}
	else 
	{
		planetAngle -= dt * M_PI/1000 * planetRotationalSpeed;
	}
	model = glm::rotate(glm::mat4(1.0f), (orbitAngle/25), glm::vec3(0.0, 1.0, 0.0))*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 10.0));
	model = glm::rotate(model, (-planetAngle/1), glm::vec3(0.0, 1.0, 0.0));
	//model = glm::translate(model, glm::vec3(0.0, 0.0, 2.0))*glm::rotate(glm::mat4(1.0f), (-angle/25), glm::vec3(0.0, 1.0, 0.0));
}*/

glm::mat4 Object::GetModel()
{
	return model;
}

void Object::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Object::FlipOrbitRotation(){
	orbitRotationalSpeed = -orbitRotationalSpeed;
}

void Object::FlipPlanetRotation(){
	planetRotationalSpeed = -planetRotationalSpeed;
}

void Object::ToggleOrbitRotation(){
	stopOrbit = !stopOrbit;
}

void Object::TogglePlanetRotation(){
	stopPlanet = !stopPlanet;
}

