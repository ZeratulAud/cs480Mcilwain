#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include <Magick++.h>

#include "graphics_headers.h"
struct ModelInfo
{
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
};
class Object
{
  public:
    Object(std::string objFilePath, float radius);
    ~Object();
    Object* AddChild(float radius, std::string texture);
    void Update(unsigned int dt, glm::mat4 origin);
    void Render(GLint& m_modelMatrix);
    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<ModelInfo> modelInfo;
    std::vector <GLuint> VB;
    std::vector <GLuint> IB;

    std::vector <GLuint> texture;

    float angle;
    float orbitRadius;

    bool LoadObjFile(std::string objFilePath);
    bool LoadTexFile(std::string texFilePath, int count);
    glm::vec3 RandomColor();
    std::vector<Object*> children;

};

#endif /* OBJECT_H */
