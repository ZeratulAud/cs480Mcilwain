#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    void Update();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  	glm::vec3 translate;
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
