#include "graphics.h"

Graphics::Graphics()
{
  BulletInit();
  timeScale = 1;
  switcher = 0;
  orbitScale = 2.5;
}

Graphics::~Graphics()
{
  delete m_camera;
  delete m_shader;
  m_camera = NULL;
  m_shader = NULL;

  for (Object *obj : Objects)
    dynamicsWorld->removeRigidBody(obj->GetRigidBody());

  delete dynamicsWorld;
  dynamicsWorld = NULL;

  for (Object *obj : Objects)
    delete obj;

  Objects.clear();
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
  m_camera->Update();
  dynamicsWorld->stepSimulation(dt, 5);

  for (auto &i : Objects) {
    i->Update(dt);
  }
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(switcher == 0)
  {
    // Start the correct program
    m_shader->Enable();

    // Send in the projection and view to the shader
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
    for (auto &i : Objects) {
      if (i->render)
        i->Render(m_modelMatrix, m_shader);
    }
  }
  else
  {
    otherShader->Enable();

    // Send in the projection and view to the shader
    glUniformMatrix4fv(other_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(other_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
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
  Object* tempObject = new Object( "OutterWalls.obj",  "Paint.png", 0,0, btVector3(0,0,0)) ;
  Objects.push_back(tempObject);

  tempObject = new Object( "OutterWalls.obj",  "Paint.png", 0,0, btVector3(0,0,0));
  Objects.push_back(tempObject);

  tempObject = new Object( "InnerWalls.obj",   "Paint.png", 0,0, btVector3(0,0,0));
  Objects.push_back(tempObject);

  tempObject = new Object( "Floor.obj",        "PlayfieldTexture.png", 0,0, btVector3(0,0,0));
  Objects.push_back(tempObject);

  tempObject = new Object( "Top.obj",          "PlayfieldTexture.png", 0,0, btVector3(0,-.5,0));
  tempObject->render = false;
  Objects.push_back(tempObject);

  //tempObject = new Object( "Bumper1.obj",      "PlayfieldTexture.png", 0,0, btVector3(0,0,0));
  //Objects.push_back(tempObject);

  //tempObject = new Object( "Bumper2.obj",      "PlayfieldTexture.png", 0,0, btVector3(0,0,0));
  //Objects.push_back(tempObject);

  //tempObject = new Object( "Bumper3.obj",      "PlayfieldTexture.png", 0,0, btVector3(0,0,0));
  //Objects.push_back(tempObject);

  tempObject = new Object( "Ball.obj",         "2kSun.jpg", 5,10, btVector3(-10,.25,7.25));
  tempObject->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  Objects.push_back(tempObject);

  tempObject = new Object( "Cube.obj",         "Paint.png", 5,10, btVector3(0,.5,-4));
  tempObject->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  Objects.push_back(tempObject);

  //Bumper1->GetRigidBody()->setCollisionFlags(Bumper1->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  //Bumper1->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);

  for (auto &i : Objects) {
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

	dynamicsWorld->setGravity(btVector3(-1, -1, 0));
}

Object* Graphics::GetObject(int objIndex) const
{
  return Objects[objIndex];
}

Camera* Graphics::GetCamera() const
{
  return m_camera;
}

btDiscreteDynamicsWorld* Graphics::GetDynamicsWorld() const
{
  return dynamicsWorld;
}

void Graphics::SwitchShader()
{
  switcher ? switcher = false : switcher = true;
}