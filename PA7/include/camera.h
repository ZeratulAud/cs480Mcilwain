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
    void SnapToPlanet(float x1, float y1, float z1,
                      float x2, float y2, float z2);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    glm::vec3 translate;
    glm::vec3 eyePos;
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
