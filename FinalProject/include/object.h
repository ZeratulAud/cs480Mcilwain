#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "shader.h"
#include "graphics_headers.h"

struct ModelInfo
{
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
};

class Object
{
  public:
    Object(std::string objFilePath, std::string texFilePath, float Mass, float Inertia, btVector3 pos);
    Object(const Object& other, float radius, float speed, float rotation, float scale);
    ~Object();
    void Update(unsigned int dt);
    void Render(GLint& m_modelMatrix, Shader *shader);
    glm::mat4 GetModel();
    btCollisionShape* GetShape();
    btRigidBody* GetRigidBody();
    glm::vec3 GetPosition();

    float ambIntensity;
    float diffIntensity;
    float specIntensity;
    int shineIntensity;
    float lightHeight;
    
    bool render;

  private:
    bool LoadObjFile(std::string objFilePath);
    bool LoadTexFile(std::string texFilePath, int count);
    glm::vec3 position;
    glm::mat4 model;
    std::vector<ModelInfo> modelInfo;
    std::vector<GLuint> VB;
    std::vector<GLuint> IB;
    std::vector<GLuint> texture;
    std::vector<GLuint> normal;
    btCollisionShape *shape;
    btRigidBody *rigidBody;
};

#endif /* OBJECT_H */
