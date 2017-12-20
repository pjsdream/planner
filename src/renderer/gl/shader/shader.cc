#include "renderer/gl/shader/shader.h"

#include <iostream>

namespace renderer
{
GLuint Shader::shaderTypeToGlType(ShaderType type)
{
  switch (type)
  {
    case ShaderType::VERTEX_SHADER:
      return GL_VERTEX_SHADER;

    case ShaderType::FRAGMENT_SHADER:
      return GL_FRAGMENT_SHADER;

    case ShaderType::GEOMETRY_SHADER:
      return GL_GEOMETRY_SHADER;
  }
}

Shader::Shader()
    : program_(0)
{
}

Shader::~Shader()
{
  if (program_)
    glDeleteProgram(program_);
}

void Shader::LoadVertexShaderFromFile(const std::string& filename)
{
  LoadShaderFromFile(ShaderType::VERTEX_SHADER, filename);
}

void Shader::LoadFragmentShaderFromFile(const std::string& filename)
{
  LoadShaderFromFile(ShaderType::FRAGMENT_SHADER, filename);
}

void Shader::LoadShaderFromFile(ShaderType type, const std::string& filename)
{
  FILE* fp = fopen(filename.c_str(), "rb");
  if (fp == NULL)
  {
    std::cerr << "Shader file path \"" << filename << "\" not found\n";
    return;
  }

  fseek(fp, 0, SEEK_END);
  int len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  GLchar* source = new GLchar[len + 1];
  fread(source, 1, len, fp);
  fclose(fp);

  source[len] = 0;

  const GLchar* const_source = const_cast<const GLchar*>(source);

  GLuint shader = glCreateShader(shaderTypeToGlType(type));

  glShaderSource(shader, 1, &const_source, NULL);
  delete[] source;

  std::cout << "compiling shader id " << shader << "\n";
  glCompileShader(shader);
  std::cout << "shader id " << shader << " compile done\n";

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled)
  {
    GLsizei len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    GLchar* log = new GLchar[len + 1];
    glGetShaderInfoLog(shader, len, &len, log);
    std::cerr << "Shader compilation failed:" << "\n"
              << log << "\n";
    delete[] log;

    // Don't need the shader anymore
    glDeleteShader(shader);
  }

  shaders_.push_back(shader);
}

void Shader::Link()
{
  program_ = glCreateProgram();

  for (GLuint shader : shaders_)
    glAttachShader(program_, shader);

  glLinkProgram(program_);

  GLint linked;
  glGetProgramiv(program_, GL_LINK_STATUS, &linked);

  if (!linked)
  {
    GLsizei len;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &len);

    GLchar* log = new GLchar[len + 1];
    glGetProgramInfoLog(program_, len, &len, log);
    std::cerr << "Shader linking failed: " << "\n"
              << log << "\n";
    delete[] log;

    // Don't need shaders and program anymore
    glDeleteProgram(program_);
    for (GLuint shader : shaders_)
      glDeleteShader(shader);
    shaders_.clear();

    return;
  }

  // Detach shaders after a successful link
  for (GLuint shader : shaders_)
    glDetachShader(program_, shader);
}
}
