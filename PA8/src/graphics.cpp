#include "graphics.h"

Graphics::Graphics()
{
  BulletInit();
  timeScale = 1;
  orbitScale = 2.5;
}

Graphics::~Graphics()
{

  delete m_camera;
  delete m_shader;
  delete OutterWalls;
  delete InnerWalls;
  delete Floor;
  delete Bumper1;
  delete Bumper2;
  delete Bumper3;
  delete Ball;
  delete Box;
  m_camera = NULL;
  m_shader = NULL;
  OutterWalls = NULL;
  InnerWalls = NULL;
  Floor = NULL;
  Bumper1 = NULL;
  Bumper2 = NULL;
  Bumper3 = NULL;
  Ball = NULL;
  Box = NULL;

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
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
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

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the object
  m_camera->Update();
  dynamicsWorld->stepSimulation(dt, 5);

  OutterWalls->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
  InnerWalls->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
  Floor->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
  Top->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
  Bumper1->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
  Bumper2->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
  Bumper3->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
  Ball->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
  Box->Update(dt,glm::mat4(1.0), timeScale, orbitScale);
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

  // Render the object
  OutterWalls->Render(m_modelMatrix);
  InnerWalls->Render(m_modelMatrix);
  Floor->Render(m_modelMatrix);
  //Top->Render(m_modelMatrix);
  Bumper1->Render(m_modelMatrix);
  Bumper2->Render(m_modelMatrix);
  Bumper3->Render(m_modelMatrix);
  Ball->Render(m_modelMatrix);
  Box->Render(m_modelMatrix);

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
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

void Graphics::CreateObjects()
{
  //InnerWalls = new Object(MODEL_DIR + "sphere.obj", 0, 1, 1, 10);
  OutterWalls = new Object(MODEL_DIR + "OutterWalls.obj", MODEL_DIR + "Paint.png", 0,0, btVector3(0,0,0));
  InnerWalls  = new Object(MODEL_DIR + "InnerWalls.obj",  MODEL_DIR + "Paint.png", 0,0, btVector3(0,0,0));
  Floor       = new Object(MODEL_DIR + "Floor.obj",       MODEL_DIR + "PlayfieldTexture.png", 0,0, btVector3(0,0,0));
  Top         = new Object(MODEL_DIR + "Top.obj",       MODEL_DIR + "PlayfieldTexture.png", 0,0, btVector3(0,-.5,0));
  Bumper1     = new Object(MODEL_DIR + "Bumper1.obj",        MODEL_DIR + "PlayfieldTexture.png", 0,0, btVector3(0,0,0));
  Bumper2     = new Object(MODEL_DIR + "Bumper2.obj",        MODEL_DIR + "PlayfieldTexture.png", 0,0, btVector3(0,0,0));
  Bumper3     = new Object(MODEL_DIR + "Bumper3.obj",        MODEL_DIR + "PlayfieldTexture.png", 0,0, btVector3(0,0,0));
  Ball        = new Object(MODEL_DIR + "Ball.obj",        MODEL_DIR + "2kSun.jpg", 5,10, btVector3(-10,.25,7.25));
  Box        = new Object(MODEL_DIR + "Cube.obj",        MODEL_DIR + "Paint.png", 5,10, btVector3(0,.5,-4));

 
  Bumper1->GetRigidBody()->setCollisionFlags(Bumper1->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  Bumper1->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);

  Ball->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  Box->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  dynamicsWorld->addRigidBody(Ball->GetRigidBody());
  dynamicsWorld->addRigidBody(Box->GetRigidBody());
  dynamicsWorld->addRigidBody(Bumper1->GetRigidBody());
  dynamicsWorld->addRigidBody(Bumper2->GetRigidBody());
  dynamicsWorld->addRigidBody(Bumper3->GetRigidBody());
  dynamicsWorld->addRigidBody(OutterWalls->GetRigidBody());
  dynamicsWorld->addRigidBody(InnerWalls->GetRigidBody());//,COLLIDE_MASK, CollidesWith);
  dynamicsWorld->addRigidBody(Floor->GetRigidBody());
  dynamicsWorld->addRigidBody(Top->GetRigidBody());

}

bool Graphics::BulletInit()
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

	dynamicsWorld->setGravity(btVector3(-1, -1, 0));

	return true;
}

Camera* Graphics::GetCamera() const
{
  return m_camera;
}

btDiscreteDynamicsWorld* Graphics::GetDynamicsWorld() const
{
  return dynamicsWorld;
}

