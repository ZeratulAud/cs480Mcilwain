#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "object.h"

Object::Object(std::string objFilePath, std::string texFilePath, float Mass, float Inertia, btVector3 pos)
{
  if (!LoadObjFile(MODEL_DIR + objFilePath))
  {
    std::cerr << "Failure to load file" << std::endl;
    exit(1);
  }
  LoadTexFile(MODEL_DIR + texFilePath, 0);

  render = true;

  btDefaultMotionState *shapeMotionState = NULL;
  shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion::getIdentity(),btVector3(pos)));
  btScalar mass(Mass);
  btVector3 inertia(Inertia, Inertia, Inertia);
  shape->calculateLocalInertia(mass, inertia);

  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
  if (Inertia == 0)
    shapeRigidBodyCI.m_restitution = 1000;
  rigidBody = new btRigidBody(shapeRigidBodyCI);
}

Object::~Object()
{
  modelInfo.clear();
}

void Object::Update(unsigned int dt)
{
  btTransform trans;
  btScalar m[16];
  rigidBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  model = glm::make_mat4(m);

  /*
  model = glm::scale(model, glm::vec3(planetScale, planetScale, planetScale));
  model = origin;
  angle += dt * M_PI/(1000 ) * timeScale;
  if (orbitRadius > 0) {
    model = glm::rotate(model, (angle/orbitRadius) * orbitSpeed, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(glm::mat4(1.0), glm::vec3((-orbitRadius)/orbitScale, 0.0, 0.0));
    model = glm::rotate(model, (angle/orbitRadius) * orbitSpeed, glm::vec3(0.0, 1.0, 0.0));
  }

  for (auto &i : children) {
    i->Update(dt, model, timeScale, orbitScale);
  }
  model = glm::rotate(model, angle * rotationSpeed, glm::vec3(0.0, 1.0, 0.0));
  */
}

void Object::Render(GLint& m_modelMatrix, Shader *shader)
{

  GLint temp = shader->GetUniformLocation("AmbientProduct");
  glUniform4f(temp, 0, 0, 0, 1);
  temp = shader->GetUniformLocation("DiffuseProduct");
  glUniform4f(temp, .5, .5, .5, 1);
  temp = shader->GetUniformLocation("SpecularProduct");
  glUniform4f(temp, .6, .6, .6, 1);
  temp = shader->GetUniformLocation("LightPosition");
  glUniform4f(temp, 0, 25, 0, 1);
  //temp = shader->GetUniformLocation("MaterialSpecularColor");
  //glUniform4f(temp, 1, 1, 1, 1);
  temp = shader->GetUniformLocation("Shininess");
  glUniform1f(temp, 300);

  for(int i = 0; i < VB.size(); i++)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(GetModel()));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, VB[i]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[i]);

    glDrawElements(GL_TRIANGLES, modelInfo[i].Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
}

bool Object::LoadObjFile(std::string objFilePath)
{
  GLuint tempGl;
  GLuint buffer;
  ModelInfo tempModel;
  Assimp::Importer importer;

  btVector3 triArray[3];
  btTriangleMesh *objTriMesh = new btTriangleMesh();

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
      aiFace* face = &mesh->mFaces[j];

      for (int k = 0; k < face->mNumIndices; k++)
      {
        aiVector3D tempVert = mesh->mVertices[face->mIndices[k]];
        aiVector3D normal = mesh->mNormals[face->mIndices[k]];
        aiVector3D uv = mesh->mTextureCoords[0][face->mIndices[k]];

        triArray[k] = btVector3(tempVert.x, tempVert.y, tempVert.z);

        Vertex vert(glm::vec3(tempVert.x, tempVert.y, tempVert.z),
                    glm::vec3(normal.x, normal.y, normal.z),
                    glm::vec2(uv.x, uv.y));

        modelInfo[i].Vertices.push_back(vert);
        modelInfo[i].Indices.push_back(face->mIndices[k]);
      }
      objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
    }

    btCollisionShape *tempShape = new btBvhTriangleMeshShape(objTriMesh, true);

    if(mesh->mNumFaces == 224 )
      tempShape = new btSphereShape(btScalar(.25));
    else if(mesh->mNumFaces == 12 )
      tempShape = new btBoxShape(btVector3(.5,.5,.5));

    shape = tempShape;

    VB.push_back(buffer);
    glGenBuffers(1, &VB[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VB[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * modelInfo[i].Vertices.size(), &modelInfo[i].Vertices[0], GL_STATIC_DRAW);

    IB.push_back(buffer);
    glGenBuffers(1, &IB[i]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);

    IB.push_back(buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * modelInfo[i].Indices.size(), &modelInfo[i].Indices[0], GL_STATIC_DRAW);
  }
  return true;
}

bool Object::LoadTexFile(std::string texFilePath, int count)
{
  Magick::InitializeMagick("");
  Magick::Blob blob;
  Magick::Image *my_image = new Magick::Image(texFilePath);
  my_image->write(&blob, "RGBA");

  glGenTextures(1, &texture[count]);
  glBindTexture(GL_TEXTURE_2D, texture[count]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, my_image->columns(), my_image->rows(),
               0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  delete my_image;
  return true;
}

glm::mat4 Object::GetModel()
{
  return model;
}

btCollisionShape* Object::GetShape()
{
  return shape;
}

btRigidBody* Object::GetRigidBody()
{
  return rigidBody;
}