#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <vector>

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

struct barrel{
    Object *object;
    int dropTimer;
    bool hasDropped;
};

struct ladder{
    Object *object;
    int ladderCooldown;
    bool cooldownFlag;
};

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
    void jump(unsigned int dt);
    void dropBarrel();
    void resetBarrel();
    void descendBarrel(unsigned int dt);
    float GetObjectDistance(Object* obj1, Object* obj2);
    std::vector<Object*> GetObjects() const;
    Camera* GetCamera() const;
    btDiscreteDynamicsWorld* GetDynamicsWorld() const;

    bool switcher;
    bool scoreFlag;
    bool moveLeftFlag;
    bool moveRightFlag;
    bool jumpFlag;
    bool dropBarrelFlag;
    int lives;
    int gameScore;
    int despawnHeight;

    float ambIntensity;
    float lightHeight;

  private:
    std::string ErrorString(GLenum error);
    void CreateObjects();
    void platformSpawner(int platformSections, glm::vec3 origin, int angle);
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

    Object* ball;
    Object* myBarrel;

    std::vector<Object*> Objects;
    std::vector<barrel*> barrels;
    std::vector<ladder*> ladders;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint other_projectionMatrix;
    GLint other_viewMatrix;
    GLint other_modelMatrix;
};

#endif /* GRAPHICS_H */
