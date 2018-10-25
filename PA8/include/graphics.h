#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

// Bullet
#include <btBulletDynamicsCommon.h>

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    bool BulletInit();
    void Update(unsigned int dt);
    void Render();
    Object* GetSun() const;
    Camera* GetCamera() const;

    float timeScale;
    float orbitScale;

  private:
    std::string ErrorString(GLenum error);
    void CreateObjects();

    Camera *m_camera;
    Shader *m_shader;
    Object *m_sun;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
};

#endif /* GRAPHICS_H */
