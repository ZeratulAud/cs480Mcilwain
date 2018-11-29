#include "camera.h"

Camera::Camera()
{
  eyePos = glm::vec3(0, 0.0, -20.0);
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
  view = glm::lookAt( eyePos, //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 5000.0f); //Distance to the far plane,
  return true;
}


void Camera::Update(glm::vec3 ballPos)
{
  eyePos = glm::vec3(ballPos.x,ballPos.y,-20);
  view = glm::lookAt( eyePos, //Eye Position
                      ballPos, //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

glm::vec3 Camera::GetEyePos()
{
  return eyePos;
}


