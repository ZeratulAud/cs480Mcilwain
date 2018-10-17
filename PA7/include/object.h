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
    Object(std::string objFilePath, float radius, float speed, float rotation, float scale);
    Object(const Object& other, float radius, float speed, float rotation, float scale);
    ~Object();
    Object* AddChild(std::string texture, float radius, float speed, float rotation, float scale);
    Object* AddRing(float speed, float scale);
    void Update(unsigned int dt, glm::mat4 origin, float timeScale, float orbitScale);
    void Render(GLint& m_modelMatrix);
    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<ModelInfo> modelInfo;
    std::vector<GLuint> VB;
    std::vector<GLuint> IB;
    std::vector<GLuint> texture;
    std::vector<Object*> children;

    float angle;
    float orbitRadius;
    float orbitSpeed;
    float rotationSpeed;
    float planetScale;

    bool LoadObjFile(std::string objFilePath);
    bool LoadTexFile(std::string texFilePath, int count);
    glm::vec3 RandomColor();
};

#endif /* OBJECT_H */
