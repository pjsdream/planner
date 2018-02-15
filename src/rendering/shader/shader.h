#ifndef SIMPLAN_RENDERING_SHADER_SHADER_H_
#define SIMPLAN_RENDERING_SHADER_SHADER_H_

#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace simplan
{
class Shader
{
public:
  Shader();
  virtual ~Shader();

  void LoadVertexShader(const std::string& filename);
  void LoadFragmentShader(const std::string& filename);
  void Link();

  void Use();

protected:
  auto Program() const
  {
    return program_;
  }

  GLuint GetUniformLocation(const char* name);

private:
  GLuint LoadShaderFromFile(const std::string& filename, GLenum type);

  GLuint program_ = 0;

  std::vector<GLuint> shaders_;
};
}

#endif // SIMPLAN_RENDERING_SHADER_SHADER_H_
