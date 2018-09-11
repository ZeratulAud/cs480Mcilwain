#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "window.h"


class Object
{
  public:
    //Object();
    Object(float mult);
    ~Object();
    void Update(unsigned int dt);
    void Render();

    void FlipOrbitRotation();
    void FlipPlanetRotation();

    void ToggleOrbitRotation();
    void TogglePlanetRotation();
    glm::mat4 GetModel();


  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float orbitRotationalSpeed;
    float planetRotationalSpeed;

    float orbitAngle;
	float planetAngle;

    bool stopPlanet;
    bool stopOrbit;

	SDL_Event event;
    //const Uint8* state;
};

#endif /* OBJECT_H */
