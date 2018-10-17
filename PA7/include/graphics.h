#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
//using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render();
    Object* GetCube() const;

    float timeScale;
    float orbitScale;

    Camera *m_camera;

  private:
    std::string ErrorString(GLenum error);


    Shader *m_shader;
    Object *m_cube;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
};

#endif /* GRAPHICS_H */
