#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "object.h"

Object::Object(std::string objFilePath, float radius)
{
  if (!LoadObjFile(objFilePath))
  {
    std::cerr << "Failure to load file" << std::endl;
    exit(1);
  }

  angle = 0.0f;
  orbitRadius = radius;

 // LoadTexFile("../models/granite.jpg");


}

Object::~Object()
{
  modelInfo.clear();
}

void Object::Update(unsigned int dt, glm::mat4 origin)
{
  if (orbitRadius>0){
    angle += dt * M_PI/(1000 * orbitRadius);
  }

  model = glm::translate(origin, glm::vec3(-2.0, -8.0, 0.0));
  if (orbitRadius>0){
    model = glm::translate(origin, glm::vec3(0.0, 2.0, 0.0));
    model = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(model, glm::vec3(0.0, 0.0, orbitRadius));

  } 
  
  for (auto &i : children) {
    i->Update(dt, origin);
  }
  //model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));
  if (orbitRadius>0){
    model = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0));
  } else {
    model = glm::scale(model, glm::vec3(2.0, 2.0, -2.0));
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
  for(int i=0;i< VB.size();i++)
  {
    //std::cout << i << "Rendering Meshes" << std::endl;
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(GetModel()));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VB[i]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[i]);

    glDrawElements(GL_TRIANGLES, modelInfo[i].Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  }

}

Object* Object::AddChild(float radius , std::string texture) {
  auto child = new Object("../models/sphere.obj", radius);
  child->LoadTexFile(texture, 0);
  children.push_back(child);
  return child;
}


bool Object::LoadObjFile(std::string objFilePath)
{
  GLuint tempGl;

  GLuint buffer;
  Assimp::Importer importer;
  ModelInfo tempModel;
  const aiScene *myScene = importer.ReadFile(objFilePath, aiProcess_Triangulate);

  if (myScene == NULL)
    return false;

  
  for (int i = 0; i < myScene->mNumMeshes; i++)
  {

    texture.push_back(tempGl);
    aiMesh* mesh = myScene->mMeshes[i];
    modelInfo.push_back(tempModel);


    for (int j = 0; j < mesh->mNumFaces; j++)
    {
      /*if(i>0)
      std::cout << objFilePath << "..Building Faces ..." << std::endl;*/

      aiFace* face = &mesh->mFaces[j];

      for (int k = 0; k < face->mNumIndices; k++)
      {
         

        aiVector3D tempPos = mesh->mVertices[face->mIndices[k]];

        aiVector3D uv = mesh->mTextureCoords[0][face->mIndices[k]];
        Vertex tempVert(glm::vec3(tempPos.x, tempPos.y, tempPos.z), glm::vec2(uv.x, uv.y));

        modelInfo[i].Vertices.push_back(tempVert);
         modelInfo[i].Indices.push_back(face->mIndices[k]);
      }
    }
    VB.push_back(buffer);
    glGenBuffers(1, &VB[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VB[i]);
   
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * modelInfo[i].Vertices.size(), &modelInfo[i].Vertices[0], GL_STATIC_DRAW);
    IB.push_back(buffer);
    glGenBuffers(1, &IB[i]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);
    IB.push_back(buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * modelInfo[i].Indices.size(), &modelInfo[i].Indices[0], GL_STATIC_DRAW);
    if(i>=1)
     LoadTexFile("../models/checker.jpg", i);
    else
     LoadTexFile("../models/granite.jpg", i);

  }


  return true;
}


bool Object::LoadTexFile(std::string texFilePath, int count)
{
  
  Magick::InitializeMagick("");
  Magick::Blob blob;
  Magick::Image *my_image;
  my_image = new Magick::Image(texFilePath);//resize_granite.jpg");
  my_image->write(&blob, "RGBA");

  glGenTextures(1, &texture[count]);

  glBindTexture(GL_TEXTURE_2D, texture[count]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, my_image->columns(), my_image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  delete my_image;
  return true;
}

glm::vec3 Object::RandomColor()
{
  float rgb[3];

  for (int i = 0; i < 3; i++)
    rgb[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

  return glm::vec3(rgb[0], rgb[1], rgb[2]);
}
