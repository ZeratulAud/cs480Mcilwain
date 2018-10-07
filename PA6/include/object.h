#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include <Magick++.h>

#include "graphics_headers.h"

class Object
{
  public:
    Object(std::string objFilePath, float radius);
    ~Object();
    void AddChild();
    void Update(unsigned int dt, glm::mat4 origin);
    void Render(GLint& m_modelMatrix);
    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    GLuint texture;

    float angle;
    float orbitRadius;

    bool LoadObjFile(std::string objFilePath);
    bool LoadTexFile(std::string texFilePath);
    glm::vec3 RandomColor();
    std::vector<Object*> children;

};

#endif /* OBJECT_H */
