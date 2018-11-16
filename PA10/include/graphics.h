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
    void SwitchShader();
    void LaunchBall();
    void PullPlunger();
    void FlipPaddle(unsigned int dt);
    bool CheckBumperCollision();
    float GetObjectDistance(Object* obj1, Object* obj2);
    std::vector<Object*> GetObjects() const;
    Camera* GetCamera() const;
    btDiscreteDynamicsWorld* GetDynamicsWorld() const;

    float timeScale;
    float orbitScale;
    float plungerforce;
    bool blockerSpawned;
    bool switcher;
    bool scoreFlag;
    bool paddleFlagR;
    bool impulseFlagR;
    bool paddleFlagL;
    bool impulseFlagL;
    bool increasePlunger;
    int lives;
    int gameScore;

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

    Object* ball;
    Object* blocker;
    Object* bumper1;
    Object* bumper2;
    Object* bumper3;
    Object* bumper4;
    Object* bumper5;
    Object* flipperL;
    Object* flipperR;
    Object* plunger;

    btHingeConstraint* constraintR;
    btHingeConstraint* constraintL;

    GLint other_projectionMatrix;
    GLint other_viewMatrix;
    GLint other_modelMatrix;
};

#endif /* GRAPHICS_H */
