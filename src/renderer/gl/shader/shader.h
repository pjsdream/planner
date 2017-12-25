#ifndef PLANNER_RENDERER_GL_SHADER_SHADER_H_
#define PLANNER_RENDERER_GL_SHADER_SHADER_H_

#include <string>
#include <vector>

#include <Eigen/Dense>

#include <glad/glad.h>

namespace renderer
{
class Shader
{
private:
  enum class ShaderType : unsigned char
  {
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER,
  };

  static GLuint shaderTypeToGlType(ShaderType type);

public:
  Shader();
  ~Shader();

  void LoadVertexShaderFromFile(const std::string& filename);
  void LoadFragmentShaderFromFile(const std::string& filename);

  void Link();

  void Start()
  {
    glUseProgram(program_);
  }

  void End()
  {
    glUseProgram(0);
  }

  void Uniform(GLint location, int v)
  {
    glUniform1i(location, v);
  }

  void Uniform(GLint location, float v)
  {
    glUniform1f(location, v);
  }

  void Uniform3f(GLint location, const Eigen::Vector3f& v)
  {
    glUniform3fv(location, 1, v.data());
  }

  void UniformMatrix3f(GLint location, const Eigen::Matrix3f& matrix)
  {
    glUniformMatrix3fv(location, 1, GL_FALSE, matrix.data());
  }

  void UniformMatrix4f(GLint location, const Eigen::Matrix4f& matrix)
  {
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data());
  }

  GLint GetUniformLocation(const std::string& name)
  {
    return glGetUniformLocation(program_, name.c_str());
  }

  void BindAttribLocation(GLuint index, const std::string& name)
  {
    glBindAttribLocation(program_, index, name.c_str());
  }

private:
  GLuint program_;
  std::vector<GLuint> shaders_;

  void LoadShaderFromFile(ShaderType type, const std::string& filename);
};
}

#endif // PLANNER_RENDERER_GL_SHADER_SHADER_H_
