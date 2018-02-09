#include "rendering/rendering_window.h"

#include <unordered_map>
#include <iostream>
#include <cstdio>

namespace simplan
{
namespace
{
std::unordered_map<GLFWwindow*, RenderingWindow*> window_map;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  window_map[window]->MouseButton(button, action, mods, xpos, ypos);
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
  window_map[window]->CursorPos(xpos, ypos);
}
}

RenderingWindow::RenderingWindow()
{
  if (!glfwInit())
  {
    std::cerr << "Widget Init Failed: GLFW Init failed\n";
    return;
  }

  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  std::cout << "GLFW Version " << major << "." << minor << "." << revision << "\n";

  // Create GLFW window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  window_ = glfwCreateWindow(1024, 768, "Simplan", NULL, NULL);

  // Initialize OpenGL via glad
  glfwMakeContextCurrent(window_);
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
  }

  // Register event callbacks
  window_map[window_] = this;
  glfwSetMouseButtonCallback(window_, MouseButtonCallback);
  glfwSetCursorPosCallback(window_, CursorPosCallback);

  // Load shaders
  LoadShaders();

  // Initialize camera
  camera_ = std::make_shared<Camera>();
}

bool RenderingWindow::ShouldClose() const
{
  return glfwWindowShouldClose(window_) == 1;
}

void RenderingWindow::LoadShaders()
{
  auto light_vert = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\simplan\\src\\shader\\light.vert", GL_VERTEX_SHADER);
  auto light_frag = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\simplan\\src\\shader\\light.frag", GL_FRAGMENT_SHADER);
  std::vector<GLuint> light_shaders{light_vert, light_frag};
  light_program_ = LinkShaders(light_shaders);

  auto color_vert = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\simplan\\src\\shader\\color.vert", GL_VERTEX_SHADER);
  auto color_frag = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\simplan\\src\\shader\\color.frag", GL_FRAGMENT_SHADER);
  std::vector<GLuint> color_shaders{ light_vert, light_frag };
  color_program_ = LinkShaders(color_shaders);
}

GLuint RenderingWindow::LoadShaderFromFile(const std::string& filename, GLenum type)
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

  return shader;
}

GLuint RenderingWindow::LinkShaders(const std::vector<GLuint>& shaders)
{
  GLuint program = glCreateProgram();

  for (GLuint shader : shaders)
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
    for (GLuint shader : shaders)
      glDeleteShader(shader);

    return 0;
  }

  // Detach shaders after a successful link
  for (GLuint shader : shaders)
    glDetachShader(program, shader);

  return program;
}

void RenderingWindow::Run()
{
  glClearColor(0.8f, 0.8f, 0.8f, 1.f);

  while (!glfwWindowShouldClose(window_))
  {
    glViewport(0, 0, 800, 600);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  glfwDestroyWindow(window_);
  glfwTerminate();
}

void RenderingWindow::MouseButton(int button, int action, int mods, double xpos, double ypos)
{
  mouse_button_ = button;
  mouse_pressed_ = action;

  last_mouse_xpos_ = xpos;
  last_mouse_xpos_ = ypos;
}

void RenderingWindow::CursorPos(double xpos, double ypos)
{
  if (mouse_pressed_)
  {
    auto dx = static_cast<int>(xpos - last_mouse_xpos_);
    auto dy = static_cast<int>(ypos - last_mouse_ypos_);

    camera_->MoveByPixel(xpos, ypos);
  }

  last_mouse_xpos_ = xpos;
  last_mouse_xpos_ = ypos;
}
}
