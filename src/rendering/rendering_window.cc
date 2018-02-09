#include "rendering/rendering_window.h"

#include <unordered_map>
#include <iostream>

#include "resource/raw_mesh_manager.h"
#include "resource/texture_manager.h"

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

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  window_map[window]->Resize(width, height);
}
}

RenderingWindow::RenderingWindow()
{
  std::memset(mouse_button_pressed_, 0, sizeof mouse_button_pressed_);

  if (!glfwInit())
  {
    std::cerr << "Widget Init Failed: GLFW Init failed\n";
    return;
  }

  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  std::cout << "GLFW Version " << major << "." << minor << "." << revision << "\n";

  // Create GLFW window
  width_ = 1024;
  height_ = 768;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window_ = glfwCreateWindow(width_, height_, "Simplan", NULL, NULL);

  // Initialize OpenGL via glad
  glfwMakeContextCurrent(window_);
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return;
  }

  // Register event callbacks
  window_map[window_] = this;
  glfwSetMouseButtonCallback(window_, MouseButtonCallback);
  glfwSetCursorPosCallback(window_, CursorPosCallback);
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

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
#ifdef _WIN32
  auto light_vert = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\planner\\src\\shader\\light.vert", GL_VERTEX_SHADER);
  auto light_frag = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\planner\\src\\shader\\light.frag", GL_FRAGMENT_SHADER);
#else
  auto light_vert = LoadShaderFromFile("/home/jaesungp/cpp_workspace/planner/src/shader/light.vert", GL_VERTEX_SHADER);
  auto
      light_frag = LoadShaderFromFile("/home/jaesungp/cpp_workspace/planner/src/shader/light.frag", GL_FRAGMENT_SHADER);
#endif
  std::vector<GLuint> light_shaders{light_vert, light_frag};
  light_program_ = LinkShaders(light_shaders);

#ifdef _WIN32
  auto color_vert = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\planner\\src\\shader\\color.vert", GL_VERTEX_SHADER);
  auto color_frag = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\planner\\src\\shader\\color.frag", GL_FRAGMENT_SHADER);
#else
  auto color_vert = LoadShaderFromFile("/home/jaesungp/cpp_workspace/planner/src/shader/color.vert", GL_VERTEX_SHADER);
  auto
      color_frag = LoadShaderFromFile("/home/jaesungp/cpp_workspace/planner/src/shader/color.frag", GL_FRAGMENT_SHADER);
#endif
  std::vector<GLuint> color_shaders{color_vert, color_frag};
  color_program_ = LinkShaders(color_shaders);

#ifdef _WIN32
  auto texture_vert = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\planner\\src\\shader\\texture.vert", GL_VERTEX_SHADER);
  auto texture_frag = LoadShaderFromFile("C:\\Users\\pjsdr_000\\Desktop\\documents\\planner\\src\\shader\\texture.frag", GL_FRAGMENT_SHADER);
#else
  auto texture_vert =
      LoadShaderFromFile("/home/jaesungp/cpp_workspace/planner/src/shader/texture.vert", GL_VERTEX_SHADER);
  auto texture_frag =
      LoadShaderFromFile("/home/jaesungp/cpp_workspace/planner/src/shader/texture.frag", GL_FRAGMENT_SHADER);
#endif
  std::vector<GLuint> texture_shaders{texture_vert, texture_frag};
  texture_program_ = LinkShaders(texture_shaders);
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
  glEnable(GL_DEPTH_TEST);

  // World coordinates
  float vertices[] = {
      0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 0.f, 0.f, 1.f,
  };
  float colors[] = {
      1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);

  GLuint vbos[2];
  glGenBuffers(2, vbos);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  glEnableVertexAttribArray(1);

  GLint location_projection = glGetUniformLocation(color_program_, "projection");
  GLint location_view = glGetUniformLocation(color_program_, "view");
  GLint location_model = glGetUniformLocation(color_program_, "model");

  GLint texture_location_projection = glGetUniformLocation(texture_program_, "projection");
  GLint texture_location_view = glGetUniformLocation(texture_program_, "view");
  GLint texture_location_model = glGetUniformLocation(texture_program_, "model");
  GLint location_texture = glGetUniformLocation(texture_program_, "material_texture");

  Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

  // mesh
  GLuint mesh_vao = 0;
  GLuint mesh_vbos[3];
  GLuint texture_id = 0;

  while (!ShouldClose())
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width_, height_);

    glUseProgram(color_program_);

    Eigen::Matrix4f projection = camera_->ProjectionMatrix().cast<float>();
    Eigen::Matrix4f view = camera_->ViewMatrix().cast<float>();

    glUniformMatrix4fv(location_projection, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(location_view, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(location_model, 1, GL_FALSE, model.data());

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 6);

    glUseProgram(texture_program_);

    glUniformMatrix4fv(texture_location_projection, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(texture_location_view, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(texture_location_model, 1, GL_FALSE, model.data());

    // Mesh loader
    auto mesh = RawMeshManager::LoadFromFile(
        "/home/jaesungp/catkin_ws/src/fetch_ros/fetch_description/meshes/torso_lift_link.dae");

    if (mesh != nullptr && mesh_vao == 0)
    {
      glGenVertexArrays(1, &mesh_vao);
      glGenBuffers(3, mesh_vbos);

      glBindVertexArray(mesh_vao);

      glBindBuffer(GL_ARRAY_BUFFER, mesh_vbos[0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertices.cols() * 3, mesh->vertices.data(), GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, mesh_vbos[1]);
      glBufferData(GL_ARRAY_BUFFER,
                   sizeof(float) * mesh->texture_coordinates.cols() * 2,
                   mesh->texture_coordinates.data(),
                   GL_STATIC_DRAW);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
      glEnableVertexAttribArray(1);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_vbos[2]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   sizeof(int) * mesh->triangles.cols() * 3,
                   mesh->triangles.data(),
                   GL_STATIC_DRAW);
    }

    if (mesh_vao != 0)
    {
      // Texture loader
      auto texture = TextureManager::LoadFromFile(mesh->texture_filename);

      if (texture != nullptr && texture_id == 0)
      {
        glGenTextures(1, &texture_id);

        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     texture->width,
                     texture->height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     (void*) texture->image.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glUniform1i(location_texture, 0);
      }

      glBindVertexArray(mesh_vao);
      glDrawElements(GL_TRIANGLES, mesh->triangles.cols() * 3, GL_UNSIGNED_INT, (void*) 0);
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  glfwDestroyWindow(window_);
  glfwTerminate();
}

void RenderingWindow::MouseButton(int button, int action, int mods, double xpos, double ypos)
{
  int button_id = button == GLFW_MOUSE_BUTTON_LEFT ? 0 :
                  button == GLFW_MOUSE_BUTTON_RIGHT ? 1 : -1;

  if (button_id != -1)
    mouse_button_pressed_[button_id] = (action == GLFW_PRESS);

  last_mouse_xpos_ = xpos;
  last_mouse_ypos_ = ypos;
}

void RenderingWindow::CursorPos(double xpos, double ypos)
{
  auto dx = static_cast<int>(xpos - last_mouse_xpos_);
  auto dy = static_cast<int>(ypos - last_mouse_ypos_);

  if (mouse_button_pressed_[0] && !mouse_button_pressed_[1])
  {
    camera_->RotateByPixel(dx, dy);
  }

  else if (!mouse_button_pressed_[0] && mouse_button_pressed_[1])
  {
    camera_->TranslateByPixel(dx, dy);
  }

  else if (mouse_button_pressed_[0] && mouse_button_pressed_[1])
  {
    camera_->ZoomByPixel(dx, dy);
  }

  last_mouse_xpos_ = xpos;
  last_mouse_ypos_ = ypos;
}

void RenderingWindow::Resize(int width, int height)
{
  width_ = width;
  height_ = height;
}
}
