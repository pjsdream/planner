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
  color_shader_ = std::make_shared<ColorShader>();
  light_shader_ = std::make_shared<LightShader>();

  /*
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
   */
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

  Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

  // mesh
  GLuint mesh_vao = 0;
  GLuint mesh_vbos[3];
  GLuint texture_id = 0;

  while (!ShouldClose())
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width_, height_);

    color_shader_->Use();
    color_shader_->LoadCamera(camera_);
    color_shader_->LoadModel(Eigen::Matrix4d::Identity());

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 6);

    light_shader_->Use();
    light_shader_->LoadCamera(camera_);
    light_shader_->LoadModel(Eigen::Matrix4d::Identity());

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
        // TODO: pass the gl functions to gl texture resource manager
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

        light_shader_->LoadTexture(texture);
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
