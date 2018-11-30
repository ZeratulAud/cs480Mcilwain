#include "graphics.h"

Graphics::Graphics()
{
  BulletInit();
  plungerforce = 0;
  timeScale = 1;
  orbitScale = 2.5;
  switcher = false;
  scoreFlag = false;
  moveLeftFlag = false;
  moveRightFlag = false;
  jumpFlag = false;
  dropBarrelFlag = false;
  lives = 3;
  gameScore = 0;
  timeBtwJump = 1000.0;
  timeSinceJump = 0.0;
  timeBtwSpawns = 10000.0;
  timeSinceSpawn = 0.0;
  timeBtwDrop = 750.0;
  timeSinceDrop = 0.0;
  despawnHeight =-25;
}

Graphics::~Graphics()
{
  delete m_camera;
  delete m_shader;
  m_camera = NULL;
  m_shader = NULL;

  for (Object *obj : Objects)
    dynamicsWorld->removeRigidBody(obj->GetRigidBody());

  for (Object *obj : Barrels)
    dynamicsWorld->removeRigidBody(obj->GetRigidBody());

  delete dynamicsWorld;
  dynamicsWorld = NULL;

  for (Object *obj : Objects)
    delete obj;

  for (Object *obj : Barrels)
    delete obj;

  Objects.clear();
  Barrels.clear();
}

bool Graphics::Initialize(int width, int height)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create all objects
  CreateObjects();

  // Set up the shaders
  m_shader = new Shader();
  otherShader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, "../shaders/vert_shader_phong"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, "../shaders/frag_shader_phong"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  if(!otherShader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!otherShader->AddShader(GL_VERTEX_SHADER, "../shaders/vert_shader_gourand"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!otherShader->AddShader(GL_FRAGMENT_SHADER, "../shaders/frag_shader_gourand"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!otherShader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  other_projectionMatrix = otherShader->GetUniformLocation("projectionMatrix");
  if (other_projectionMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("other_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  other_viewMatrix = otherShader->GetUniformLocation("viewMatrix");
  if (other_viewMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("other_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  other_modelMatrix = otherShader->GetUniformLocation("modelMatrix");
  if (other_modelMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("other_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the object
  m_camera->Update(glm::vec3(ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().x(),
                      		 ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().y(),
                    		 ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().z()));

  //flipperR->GetRigidBody()->applyTorque(btVector3(1,1,1));
  barrelSpawner(dt);
  for (auto &i : Objects) {
  	if(i->GetRigidBody()->getCenterOfMassTransform().getOrigin().y() < despawnHeight){
  		i->destroy = true;
  	} else {
  		i->Update(dt);
  	}
  }

  for (auto it = Objects.begin(); it != Objects.end(); ) {
  	if ((*it)->destroy) {
  	  dynamicsWorld->removeRigidBody((*it)->GetRigidBody());
  	  Objects.erase(it);
  	} else {
  	  ++it;
  	}
  }
  if(moveRightFlag == true)
    moveRight();
  if(moveLeftFlag == true)
    moveLeft();

  jump(dt);
  descendBarrel(dt);

  dynamicsWorld->stepSimulation(dt, 5);
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!switcher)
  {
    // Start the correct program
    m_shader->Enable();

    // Send in the projection and view to the shader
    GLint temp = m_shader->GetUniformLocation("eyePos");
    glUniform3f(temp, m_camera->eyePos.x, m_camera->eyePos.y, m_camera->eyePos.z);

    temp = m_shader->GetUniformLocation("ballPos");
    glUniform3f(temp, (float) ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().x(),
                      (float) ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().y(),
                      (float) ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().z());


    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
    for (auto &i : Objects) {
      if (i->render)
        i->Render(m_modelMatrix, m_shader);
    }

    for (auto &i : Barrels) {
      if (i->render)
        i->Render(m_modelMatrix, m_shader);
    }
  }
  else
  {
    otherShader->Enable();

    // Send in the projection and view to the shader
    GLint temp = otherShader->GetUniformLocation("eyePos");
    glUniform3f(temp, m_camera->eyePos.x, m_camera->eyePos.y, m_camera->eyePos.z);

    temp = otherShader->GetUniformLocation("ballPos");
    glUniform3f(temp, (float) ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().x(),
                      (float) ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().y(),
                      (float) ball->GetRigidBody()->getCenterOfMassTransform().getOrigin().z());

    glUniformMatrix4fv(other_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(other_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
    for (auto &i : Objects) {
      if (i->render)
        i->Render(other_modelMatrix, otherShader);
    }

    for (auto &i : Objects) {
      if (i->render)
        i->Render(other_modelMatrix, otherShader);
    }
  }

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    std::string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";

  else if(error == GL_INVALID_VALUE)
    return "GL_INVALID_VALUE: A numeric argument is out of range.";

  else if(error == GL_INVALID_OPERATION)
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";

  else if(error == GL_OUT_OF_MEMORY)
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";

  else
    return "None";
}

void Graphics::CreateObjects()
{
  Object* tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(8,2,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(4,1,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(0,0,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(-4,-1,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(-8,-2,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);

  tempObject = new Object("Ramp2x1Flipped.obj", "reddy.jpg", 0,0, btVector3(-16,-7,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1Flipped.obj", "reddy.jpg", 0,0, btVector3(-12,-8,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1Flipped.obj", "reddy.jpg", 0,0, btVector3(-8,-9,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1Flipped.obj", "reddy.jpg", 0,0, btVector3(-4,-10,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1Flipped.obj", "reddy.jpg", 0,0, btVector3(0,-11,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);

  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(8,-16,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(4,-17,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(0,-18,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(-4,-19,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);
  tempObject = new Object("Ramp2x1.obj", "reddy.jpg", 0,0, btVector3(-8,-20,0));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Objects.push_back(tempObject);

  tempObject = new Object("LevelWall.obj", "reddy.jpg", 0,0, btVector3(0,0,1.1));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  tempObject->render = false;
  Objects.push_back(tempObject);
  tempObject = new Object("LevelWall.obj", "reddy.jpg", 0,0, btVector3(0,0,-1.1));
  //tempObject->GetRigidBody()->setRestitution(1.0);
  tempObject->render = false;
  Objects.push_back(tempObject);

  myBarrel = tempObject = new Object("Barrel.obj", "rednice.jpg", 5,1, btVector3(2, 5, 0));
  tempObject->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  //tempObject->GetRigidBody()->setRestitution(1.0);
  Barrels.push_back(tempObject);

  ball = tempObject = new Object("Ball.obj", "greybaby.jpg", 1,5, btVector3(5, 11, 0));
  tempObject->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  tempObject->GetRigidBody()->setAngularFactor(btVector3(0,1,0));
  Objects.push_back(tempObject);

  //tempObject = new Object("OutterWalls.obj", "rednice.jpg", 0,0, btVector3(0,0,0));

  /*bumper1 = tempObject = new Object("Bumper1.obj", "rednice.jpg", 5,10, btVector3(1, 0,-.3));
  tempObject->GetRigidBody()->setCollisionFlags(tempObject->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  tempObject->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  tempObject->GetRigidBody()->setRestitution(7.0);
  Objects.push_back(tempObject);*/


  for (auto &i : Objects) {
    dynamicsWorld->addRigidBody(i->GetRigidBody());
  }

  for (auto &i : Barrels) {
    dynamicsWorld->addRigidBody(i->GetRigidBody());
  }
}

void Graphics::BulletInit()
{
	btBroadphaseInterface *broadphase =
		new btDbvtBroadphase();

	btDefaultCollisionConfiguration *collisionConfiguration =
		new btDefaultCollisionConfiguration();

	btCollisionDispatcher *dispatcher =
		new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver *solver =
		new btSequentialImpulseConstraintSolver();

	dynamicsWorld =
		new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -3, 0));
}

void Graphics::barrelSpawner(unsigned int dt){
	timeSinceSpawn += dt;

	if (timeBtwSpawns<timeSinceSpawn){
		timeSinceSpawn = 0;
		spawnBarrel();
	}
}

void Graphics::spawnBarrel()
{

  Object* tempObject = new Object("Barrel.obj", "rednice.jpg", 5,1, btVector3(2, 5, 0));
  tempObject->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  Barrels.push_back(tempObject);
  dynamicsWorld->addRigidBody(tempObject->GetRigidBody());


}

void Graphics::SwitchShader()
{
  switcher ? switcher = false : switcher = true;
}



float Graphics::GetObjectDistance(Object* obj1, Object* obj2)
{
  float x = obj1->GetRigidBody()->getCenterOfMassTransform().getOrigin().x()
            - obj2->GetRigidBody()->getCenterOfMassTransform().getOrigin().x();

  float z = obj1->GetRigidBody()->getCenterOfMassTransform().getOrigin().z()
            - obj2->GetRigidBody()->getCenterOfMassTransform().getOrigin().z();

  return sqrt(pow(x, 2.0) + pow(z, 2.0));
}

std::vector<Object*> Graphics::GetObjects() const
{
  return Objects;
}

Camera* Graphics::GetCamera() const
{
  return m_camera;
}

btDiscreteDynamicsWorld* Graphics::GetDynamicsWorld() const
{
  return dynamicsWorld;
}

void Graphics::moveLeft()
{
    btVector3 tempbtVec3 =  ball->GetRigidBody()->getLinearVelocity();
    if(tempbtVec3.x() < 0)
       tempbtVec3 = btVector3(0, 0, 0);
    tempbtVec3 = tempbtVec3 + btVector3(.4, 0, 0);
    if(tempbtVec3.x() > 1)
      tempbtVec3 = btVector3(1, tempbtVec3.y(), 0);
    ball->GetRigidBody()->setLinearVelocity(tempbtVec3);
}
void Graphics::moveRight()
{
    btVector3 tempbtVec3 =  ball->GetRigidBody()->getLinearVelocity();
    if(tempbtVec3.x() > 0)
       tempbtVec3 = btVector3(0, 0, 0);
    tempbtVec3 = tempbtVec3 + btVector3(-.4, 0, 0);
    if(tempbtVec3.x() < -1)
      tempbtVec3 = btVector3(-1, tempbtVec3.y(), 0);
    /*if(tempbtVec3.y() < -2)
      tempbtVec3 = btVector3(tempbtVec3.x(), -2, 0);*/
    ball->GetRigidBody()->setLinearVelocity(btVector3(tempbtVec3));
}
void Graphics::moveDown()
{

}
void Graphics::jump(unsigned int dt)
{
  timeSinceJump += dt;

  if (timeBtwJump<timeSinceJump && jumpFlag == true){
    timeSinceJump = 0;
    ball->GetRigidBody()->applyCentralImpulse( btVector3( 0, 6.5, 0 ) );

  }
}

void Graphics::dropBarrel()
{

    myBarrel->GetRigidBody()->setCollisionFlags(myBarrel->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    timeSinceDrop = 0;
    dropBarrelFlag = true;

}



void Graphics::descendBarrel(unsigned int dt)
{
  timeSinceDrop += dt;
  btTransform newTrans;
  if(dropBarrelFlag == true)
  {
    myBarrel->GetRigidBody()->getMotionState()->getWorldTransform(newTrans);
    newTrans.getOrigin() += (btVector3(0, -.1, 0));
    myBarrel->GetRigidBody()->getMotionState()->setWorldTransform(newTrans);
  }
  if (timeBtwDrop<timeSinceDrop && dropBarrelFlag == true){
    resetBarrel();
    timeSinceDrop = 0;
  }

}
void Graphics::resetBarrel()
{
    myBarrel->GetRigidBody()->setCollisionFlags(myBarrel->GetRigidBody()->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
    dropBarrelFlag = false;
}


