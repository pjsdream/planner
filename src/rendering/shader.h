#ifndef SIMPLAN_RENDERING_SHADER_H_
#define SIMPLAN_RENDERING_SHADER_H_

#include <string>

#include <GLFW/glfw3.h>

namespace simplan
{
class Shader
{
public:
  Shader();
  ~Shader();

  void LoadVertexShader(const std::string& filename);
  void LoadFragmentShader(const std::string& filename);
  void Link();

private:
  GLuint program_;
};
}

#endif // SIMPLAN_RENDERING_SHADER_H_
