#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include "graphics_headers.h"

class Object
{
  public:
    Object(std::string objFilePath);
    ~Object();
    void Update(unsigned int dt);
    void Render();
    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    float angle;

    bool LoadObjFile(std::string objFilePath);
    glm::vec3 RandomColor();
};

#endif /* OBJECT_H */
