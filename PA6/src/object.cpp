#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "object.h"
//using namespace Magick; 

Object::Object(std::string objFilePath, float radius)
{
  if (!LoadObjFile(objFilePath))
  {
    std::cerr << "Failure to load file" << std::endl;
    exit(1);
  }

  angle = 0.0f;
  orbitRadius = radius;

  LoadTexFile("../models/granite.jpg");

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, glm::mat4 origin)
{
  angle += dt * M_PI/10000;

  model = glm::translate(origin, glm::vec3(-2.0, -8.0, 0.0));
  if (orbitRadius>0){
    model = glm::translate(origin, glm::vec3(0.0, 2.0, 0.0));
    model = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(model, glm::vec3(0.0, 0.0, orbitRadius));

  } 
  
  for (auto &i : children) {
    i->Update(dt, origin);
  }
  model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));
  if (orbitRadius>0){
    model = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0));
  }




}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render(GLint& m_modelMatrix)
{
  for (auto &i : children) {
    i->Render(m_modelMatrix);
  }

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(GetModel()));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}

void Object::AddChild() {

  auto child = new Object("../models/sphere.obj", 10);
  child->LoadTexFile("../models/SunSurface.png");
  children.push_back(child);
}

bool Object::LoadObjFile(std::string objFilePath)
{
  Assimp::Importer importer;
  const aiScene *myScene = importer.ReadFile(objFilePath, aiProcess_Triangulate);

  if (myScene == NULL)
    return false;

  const aiMesh* mesh = myScene->mMeshes[0];

  for (int i = 0; i < mesh->mNumFaces; i++)
  {
    const aiFace* face = &mesh->mFaces[i];

    for(int vertexNum = 0; vertexNum < face->mNumIndices; vertexNum++)
    {
      const aiVector3D tempPos = mesh->mVertices[face->mIndices[vertexNum]];

      aiVector3D uv = mesh->mTextureCoords[0][face->mIndices[vertexNum]];
      Vertex tempVert(glm::vec3(tempPos.x, tempPos.y, tempPos.z), glm::vec2(uv.x, uv.y));

      Vertices.push_back(tempVert);
      Indices.push_back(face->mIndices[vertexNum]);
    }
  }
  return true;
}


bool Object::LoadTexFile(std::string texFilePath)
{
  Magick::InitializeMagick("");
  Magick::Blob blob;  
  Magick::Image *my_image; 
  my_image = new Magick::Image(texFilePath);//resize_granite.jpg");
  my_image->write(&blob, "RGBA");

  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, my_image->columns(), my_image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  delete my_image;
  return 1;
}

glm::vec3 Object::RandomColor()
{
  float rgb[3];

  for (int i = 0; i < 3; i++)
    rgb[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

  return glm::vec3(rgb[0], rgb[1], rgb[2]);
}