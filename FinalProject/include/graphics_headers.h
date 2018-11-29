#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

// need for experimental version of glm headers
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

// File path for all models
#define MODEL_DIR std::string("../models/")

// ASSIMP headers
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

// ImageMagick header
#include <Magick++.h>

// Bullet header
#include <btBulletDynamicsCommon.h>

struct Vertex
{
  glm::vec3 vertex;
  glm::vec3 normal;
  glm::vec2 texture;

  Vertex() {}
  Vertex(glm::vec3 v, glm::vec3 n, glm::vec2 t):
    vertex(v), normal(n), texture(t)
  {}
};

#endif /* GRAPHICS_HEADERS_H */
