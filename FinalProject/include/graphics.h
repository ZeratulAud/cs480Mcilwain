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
    void spawnBarrel();
    void moveLeft();
    void moveRight();
    void moveDown();
    void dropBarrel();
    void resetBarrel();
    void descendBarrel(unsigned int dt);
    void jump(unsigned int dt);
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
    bool moveLeftFlag;
    bool moveRightFlag;
    bool jumpFlag;
    bool dropBarrelFlag;
    int lives;
    int gameScore;
    int despawnHeight;

  private:
    std::string ErrorString(GLenum error);
    void CreateObjects();

    void barrelSpawner(unsigned int dt);
    float timeBtwSpawns;
    float timeSinceSpawn;
    float timeBtwJump;
    float timeSinceJump;
    float timeBtwDrop;
    float timeSinceDrop;

    Camera *m_camera;
    Shader *m_shader;
    Shader *otherShader;

    btDiscreteDynamicsWorld *dynamicsWorld;

    std::vector<Object*> Objects;
    std::vector <Object*>::iterator it;

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
    Object* myBarrel;


    btHingeConstraint* constraintR;
    btHingeConstraint* constraintL;

    GLint other_projectionMatrix;
    GLint other_viewMatrix;
    GLint other_modelMatrix;
};

#endif /* GRAPHICS_H */
