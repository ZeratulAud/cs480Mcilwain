#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

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
    void Update(unsigned int dt, glm::mat4 origin, float timeScale, float orbitScale);
    void Render(GLint& m_modelMatrix);
    glm::mat4 GetModel();
    btCollisionShape* GetShape();
    btRigidBody* GetRigidBody();

    std::vector<Object*> children;

  private:
    bool LoadObjFile(std::string objFilePath);
    bool LoadTexFile(std::string texFilePath, int count);

    glm::mat4 model;
    std::vector<ModelInfo> modelInfo;
    std::vector<GLuint> VB;
    std::vector<GLuint> IB;
    std::vector<GLuint> texture;
    btCollisionShape *shape;
    btRigidBody *rigidBody;

    float angle;
    float orbitRadius;
    float orbitSpeed;
    float rotationSpeed;
    float planetScale;
};

#endif /* OBJECT_H */
