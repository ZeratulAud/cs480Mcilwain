#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "window.h"
#include <string>
#include <fstream>


class Object
{
  public:
    //Object();
    Object();
    Object(float speed, float scale, float radius);
    //Object(Object* parentPlanet);
    ~Object();
    void Update(unsigned int dt, glm::mat4 orbitOrigin);
    void Render(GLint& m_modelMatrix);

    void FlipOrbitRotation();
    void FlipPlanetRotation();

    void ToggleOrbitRotation();
    void TogglePlanetRotation();

    void AddChild(float speed, float scale, float radius);

    void Load(std::string model);

    glm::mat4 GetModel();

    float orbitSpeed;
    float spinSpeed;
    float planetScale;
    float orbitRadius;



  private:

    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float orbitAngle;
	float spinAngle;

    bool stopSpin;
    bool stopOrbit;

    std::vector<Object*> children;
};


#endif /* OBJECT_H */
