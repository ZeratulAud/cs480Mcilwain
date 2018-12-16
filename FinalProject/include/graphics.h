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
    void spawnBarrel(btVector3 pos);
    void moveLeft();
    void moveRight();
    void jump(unsigned int dt);
    void dropBarrel(barrel* passedBarrel);
    void resetBarrel(barrel* passedBarrel);
    void checkBarrelDrop();
    void checkLadderState(unsigned int dt);
    void descendBarrel(unsigned int dt);
    bool HasDied();
    void ResetPlayer();
    float GetObjectDistance(Object* obj1, Object* obj2);
    Object* GetPlayer() const;
    std::vector<Object*> GetObjects() const;
    Camera* GetCamera() const;
    btDiscreteDynamicsWorld* GetDynamicsWorld() const;

    bool switcher;
    bool scoreFlag;
    bool moveLeftFlag;
    bool moveRightFlag;
    bool jumpFlag;
    bool dropBarrelFlag;
    int bottom;
    int lives;
    int gameScore;
    int despawnHeight;

    float ambIntensity;
    float lightHeight;
    unsigned int SHADOW_WIDTH , SHADOW_HEIGHT;

    bool movingLeft;

    btVector3 spawnlocation;

  private:
    std::string ErrorString(GLenum error);
    void CreateObjects();
    void platformSpawner(int platformSections, glm::vec3 origin, int angle);
    void barrelSpawner(unsigned int dt, float playerHeight);

    float timeBtwSpawns;
    float timeSinceSpawn;
    float timeBtwJump;
    float timeSinceJump;
    float timeBtwDrop;
    float timeSinceDrop;
    float ladderCD;



    Camera *m_camera;
    Shader *m_shader;
    Shader *otherShader;
    Shader *shadow_shader;


    btDiscreteDynamicsWorld *dynamicsWorld;

    Object* player;
    Object* myBarrel;

    std::vector<Object*> Objects;
    std::vector<barrel*> barrels;
    std::vector<ladder*> ladders;

    GLint m_lightSpaceMatrix;
    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    unsigned int depthMap;

    glm::mat4 lightSpaceMatrix;

    GLint other_projectionMatrix;
    GLint other_viewMatrix;
    GLint other_modelMatrix;

    unsigned int depthMapFBO;
};

#endif /* GRAPHICS_H */
