#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    void Update();
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    glm::vec3 GetEyePos();

    glm::vec3 translate;
    glm::vec3 eyePos;

  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
