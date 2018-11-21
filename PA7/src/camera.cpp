#include "camera.h"

Camera::Camera()
{
  eyePos = glm::vec3(100, 100.0, 0.0);
  translate = glm::vec3(0.0,0.0,0.0);
}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(100, 100.0, 0.0), //Eye Position
                      glm::vec3(0.0, 1.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 5000.0f); //Distance to the far plane,
  return true;
}


void Camera::Update()
{
  view = glm::translate(view, translate);
  eyePos -= translate;
  translate = glm::vec3(0.0,0.0,0.0);
}

void Camera::SnapToPlanet(float x1, float y1, float z1,
                          float x2, float y2, float z2)
{
  eyePos = glm::vec3(x1, y1, z1);

  view = glm::lookAt(glm::vec3(x1, y1, z1),
                     glm::vec3(x2, y2, z2),
                     glm::vec3(0.0, 1.0, 0.0));
}
glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


