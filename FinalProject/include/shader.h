#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType, std::string fileName);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
    GLuint m_shaderProg;
  private:
    
    std::vector<GLuint> m_shaderObjList;
 
    std::string readShaderFile(const char* filePath);
};

#endif  /* SHADER_H */
