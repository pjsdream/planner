#include "rendering/shader/shader.h"

#include <iostream>

namespace simplan
{
Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::LoadVertexShader(const std::string& filename)
{
  shaders_.push_back(LoadShaderFromFile(filename, GL_VERTEX_SHADER));
}

void Shader::LoadFragmentShader(const std::string& filename)
{
  shaders_.push_back(LoadShaderFromFile(filename, GL_FRAGMENT_SHADER));
}

GLuint Shader::LoadShaderFromFile(const std::string& filename, GLenum type)
{
  FILE* fp = fopen(filename.c_str(), "rb");
  if (fp == NULL)
  {
    std::cerr << "Shader file path \"" << filename << "\" not found\n";
    return 0;
  }

  fseek(fp, 0, SEEK_END);
  int len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  GLchar* source = new GLchar[len + 1];
  fread(source, 1, len, fp);
  fclose(fp);

  source[len] = 0;

  const GLchar* const_source = const_cast<const GLchar*>(source);

  GLuint shader = glCreateShader(type);

  glShaderSource(shader, 1, &const_source, NULL);
  delete[] source;

  glCompileShader(shader);

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

  return shader;
}

void Shader::Link()
{
  GLuint program = glCreateProgram();

  for (GLuint shader : shaders_)
    glAttachShader(program, shader);

  glLinkProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);

  if (!linked)
  {
    GLsizei len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

    GLchar* log = new GLchar[len + 1];
    glGetProgramInfoLog(program, len, &len, log);
    std::cerr << "Shader linking failed: " << "\n"
              << log << "\n";
    delete[] log;

    // Don't need shaders and program anymore
    glDeleteProgram(program);
    for (GLuint shader : shaders_)
      glDeleteShader(shader);

    return;
  }

  // Detach shaders after a successful link
  for (GLuint shader : shaders_)
    glDetachShader(program, shader);

  program_ = program;
}

void Shader::Use()
{
  glUseProgram(program_);
}

GLuint Shader::GetUniformLocation(const char* name)
{
  return glGetUniformLocation(Program(), name);
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
  return glGetUniformLocation(Program(), name.c_str());
}
}
