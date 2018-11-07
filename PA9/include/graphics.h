#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <vector>

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
    void BulletInit();
    void Update(unsigned int dt);
    void Render();
    Object* GetObject(int objIndex) const;
    Camera* GetCamera() const;
    btDiscreteDynamicsWorld* GetDynamicsWorld() const;
    void SwitchShader();

    float timeScale;
    float orbitScale;
    bool switcher;

  private:
    std::string ErrorString(GLenum error);
    void CreateObjects();

    Camera *m_camera;
    Shader *m_shader;
    Shader *otherShader;

    btDiscreteDynamicsWorld *dynamicsWorld;

    std::vector<Object*> Objects;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint other_projectionMatrix;
    GLint other_viewMatrix;
    GLint other_modelMatrix;
};

#endif /* GRAPHICS_H */
