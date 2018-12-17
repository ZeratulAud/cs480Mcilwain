#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "object.h"

Object::Object(std::string objFilePath, std::string texFilePath, float Mass, float Inertia, btVector3 pos)
{
  if (!LoadObjFile(ASSET_DIR + objFilePath))
  {
    std::cerr << "Failure to load file" << std::endl;
    exit(1);
  }
  for (int i=0; i<VB.size();i++){
      LoadTexFile(ASSET_DIR + texFilePath, i);
  }
  if (objFilePath == "DK_Arm_UP.obj"){
    LoadTexFile(ASSET_DIR + "c07_eye00.png", 0);
  }
  //position = glm::vec3(pos.x(), pos.y(), pos.z());
  diffIntensity = .2;
  specIntensity = .6;
  shineIntensity = 125;

  render = true;
  destroy = false;

  if (Mass>=0&&Inertia>=0){
    btDefaultMotionState *shapeMotionState = NULL;
    shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion::getIdentity(),btVector3(pos)));
    btScalar mass(Mass);
    btVector3 inertia(Inertia, Inertia, Inertia);
    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

    rigidBody = new btRigidBody(shapeRigidBodyCI);
  }


}

Object::Object(Object& copy, btVector3 pos){

    diffIntensity = copy.diffIntensity;
    specIntensity = copy.specIntensity;
    shineIntensity = copy.shineIntensity;

    model = copy.model;
    modelInfo = copy.modelInfo;
    shape = copy.shape;
    GLuint tempGl;
    GLuint buffer;
    texture.push_back(tempGl);
    
    VB = copy.VB;
    IB = copy.IB;

    for (int i=0; i<VB.size();i++){
      LoadTexFile(ASSET_DIR + "DKBarrel.png", i);
    }
    LoadTexFile(ASSET_DIR + "DK.png", 1);
    LoadTexFile(ASSET_DIR + "DK.png", 3);

    btDefaultMotionState *shapeMotionState = NULL;
    shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion::getIdentity(),btVector3(pos)));
    btScalar mass(25);
    btVector3 inertia(10, 10, 10);
    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

    rigidBody = new btRigidBody(shapeRigidBodyCI);
    rigidBody->setAngularFactor(btVector3(0,0,1));

    //rigidBody = copy.GetRigidBody();

    /*btTransform transform;
    transform.setOrigin(pos);
    transform.setRotation(btQuaternion::getIdentity());

    rigidBody->setWorldTransform(transform);*/

    render = true;
    destroy = false;

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
}

void Object::Render(GLint& m_modelMatrix, Shader *shader)
{
  GLint temp = shader->GetUniformLocation("DiffuseProduct");
  glUniform4f(temp, diffIntensity, diffIntensity, diffIntensity, 1);

  temp = shader->GetUniformLocation("SpecularProduct");
  glUniform4f(temp, specIntensity, specIntensity, specIntensity, 1);

  temp = shader->GetUniformLocation("Shininess");
  glUniform1f(temp, shineIntensity);

  //glAlphaFunc(GL_GREATER, 0.1);
  //glEnable(GL_ALPHA_TEST);

  for(int i = 0; i < VB.size(); i++)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(GetModel()));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    
    //glActiveTexture(GL_TEXTURE0+1);
    //glBindTexture(GL_TEXTURE_2D, ShadowMap);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, VB[i]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);

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
    modelInfo.push_back(tempModel);
    aiMesh* mesh = myScene->mMeshes[i];

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
    //std::cout << mesh->mNumFaces << std::endl;
    if(mesh->mNumFaces == 224 )
      tempShape = new btSphereShape(btScalar(.25));
    else if(objFilePath == (ASSET_DIR + "PlayerSprite.obj"))
      tempShape = new btCapsuleShape (.5, .5);
    else if(objFilePath == (ASSET_DIR + "Floor.obj")){
      tempShape = new btSphereShape(btScalar(.25));
    }
    else if(objFilePath == (ASSET_DIR + "Barrel2.obj") )
      tempShape = new btCylinderShapeZ(btVector3(1,1,1));//btConvexTriangleMeshShape(objTriMesh, true);
    else if(mesh->mNumFaces == 124 )
      tempShape = new btCylinderShapeZ(btVector3(1,1,1));//btConvexTriangleMeshShape(objTriMesh, true);
    else if(mesh->mNumFaces == 316)
      tempShape = new btCylinderShapeZ(btVector3(1,1,1));//btConvexTriangleMeshShape(objTriMesh, true);
    else if(mesh->mNumFaces == 260 )
      tempShape = new btConvexTriangleMeshShape(objTriMesh, true);//btBoxShape(btVector3(.3,.3,.6));
    else if(objFilePath == (ASSET_DIR + "Cube.obj") )
      tempShape = new btBoxShape(btVector3(.3,.3,.6));

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

bool Object::LoadShadowMap(GLuint shadowMap, int count)
{
  ShadowMap = shadowMap;
  //glGenTextures(1, &ShadowMap);
  //glBindTexture(GL_TEXTURE_2D, ShadowMap);
    //glActiveTexture(GL_TEXTURE0+1);
    //glBindTexture(GL_TEXTURE_2D, shadowMap);

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

glm::vec3 Object::GetPosition()
{

  return position;
}