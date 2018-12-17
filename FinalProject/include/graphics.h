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

//struct DK_Platform{
 //   Object *platform;
 //   Object *DK;
 //   btVector3 spawnlocation;
 //   float maxHeight;
//}SpawnPlatform;

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, int Level);
    void BulletInit();
    void Update(unsigned int dt);
    void Render();
    void SwitchShader();
    void spawnBarrel(btVector3 pos);
    void moveLeft();
    void moveRight();
    void climbUp();
    void climbLeft();
    void climbRight();
    void climbDown();
    void jump(unsigned int dt);
    void dropBarrel(barrel* passedBarrel);
    void resetBarrel(barrel* passedBarrel);
    void checkBarrelDrop();
    void checkLadderState(unsigned int dt);
    void checkPlayerOnLadder(unsigned int dt);
    void checkBarrelJumped(unsigned int dt);
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
    bool playerOnLadder;

    bool climbUpFlag = false;
    bool climbLeftFlag = false;
    bool climbRightFlag = false;
    bool climbDownFlag = false;

    int bottom;
    int lives;
    int gameScore;
    int despawnHeight;
    int scoreCooldown;

    float ambIntensity;
    float lightHeight;
    unsigned int SHADOW_WIDTH , SHADOW_HEIGHT;

    bool movingLeft;

    btVector3 spawnlocation;
    btVector3 playerSpawn;

  private:
    std::string ErrorString(GLenum error);
    void loadLevel0();
    void loadLevel1();
    void loadLevel2();
    void CreateObjects(int level);
    void platformSpawner(int platformSections, glm::vec3 origin, int angle);
    void barrelSpawner(unsigned int dt, float playerHeight);

    float timeBtwSpawns;
    float timeSinceSpawn;
    float timeBtwJump;
    float timeSinceJump;
    float timeBtwDrop;
    float timeSinceDrop;
    float ladderCD;
    float timeBtwScoring;



    Camera *m_camera;
    Shader *m_shader;
    Shader *otherShader;
    Shader *shadow_shader;


    btDiscreteDynamicsWorld *dynamicsWorld;

    Object* player;
    Object* myBarrel;
    Object* floor;

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
