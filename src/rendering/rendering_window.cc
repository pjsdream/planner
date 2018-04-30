#include "rendering/rendering_window.h"

#include <unordered_map>
#include <iostream>

#include "resource/raw_mesh_manager.h"
#include "resource/texture_manager.h"

#include "rendering/object/array_object.h"
#include "rendering/object/buffer_object.h"
#include "rendering/object/texture_object.h"

#include "rendering/light.h"

#include "vision/kinect.h"

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
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
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
  pointcloud_shader_ = std::make_shared<PointcloudShader>();
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

  // Lights
  std::shared_ptr<Light> lights[5];

  for (int i = 0; i < 5; i++)
    lights[i] = std::make_shared<Light>();

  lights[0]->type = Light::Type::DIRECTIONAL;
  lights[0]->position = Eigen::Vector3f(-0.2, -1.0, -0.3);
  lights[0]->ambient = Eigen::Vector3f(0.05, 0.05, 0.05);
  lights[0]->diffuse = Eigen::Vector3f(0.4, 0.4, 0.4);
  lights[0]->specular = Eigen::Vector3f(0.5, 0.5, 0.5);

  lights[1]->type = Light::Type::POINT;
  lights[1]->position = Eigen::Vector3f(0.7, 0.2, 2.0);
  lights[1]->ambient = Eigen::Vector3f(0.05, 0.05, 0.05);
  lights[1]->diffuse = Eigen::Vector3f(0.8, 0.8, 0.8);
  lights[1]->specular = Eigen::Vector3f(1.0, 1.0, 1.0);
  lights[1]->attenuation = Eigen::Vector3f(1.0, 0.09, 0.032);

  lights[2]->type = Light::Type::POINT;
  lights[2]->position = Eigen::Vector3f(2.3, -3.3, -4.0);
  lights[2]->ambient = Eigen::Vector3f(0.05, 0.05, 0.05);
  lights[2]->diffuse = Eigen::Vector3f(0.8, 0.8, 0.8);
  lights[2]->specular = Eigen::Vector3f(1.0, 1.0, 1.0);
  lights[2]->attenuation = Eigen::Vector3f(1.0, 0.09, 0.032);

  lights[3]->type = Light::Type::POINT;
  lights[3]->position = Eigen::Vector3f(-4.0, 2.0, -12.0);
  lights[3]->ambient = Eigen::Vector3f(0.05, 0.05, 0.05);
  lights[3]->diffuse = Eigen::Vector3f(0.4, 0.4, 0.4);
  lights[3]->specular = Eigen::Vector3f(0.5, 0.5, 0.5);
  lights[3]->attenuation = Eigen::Vector3f(1.0, 0.09, 0.032);

  lights[4]->type = Light::Type::POINT;
  lights[4]->position = Eigen::Vector3f(0.0, 0.0, -3.0);
  lights[4]->ambient = Eigen::Vector3f(0.05, 0.05, 0.05);
  lights[4]->diffuse = Eigen::Vector3f(0.4, 0.4, 0.4);
  lights[4]->specular = Eigen::Vector3f(0.5, 0.5, 0.5);
  lights[4]->attenuation = Eigen::Vector3f(1.0, 0.09, 0.032);

  auto material = std::make_shared<Material>();
  material->ambient = Eigen::Vector3f(0.f, 0.f, 0.f);
  material->diffuse = Eigen::Vector3f(1.0f, 0.5f, 0.31f);
  material->specular = Eigen::Vector3f(0.5f, 0.5f, 0.5f);
  material->shininess = 32.0f;

  auto vao = std::make_shared<ArrayObject>();
  auto vbo0 = std::make_shared<BufferObject>();
  auto vbo1 = std::make_shared<BufferObject>();

  vbo0->SetArrayBuffer(sizeof vertices, vertices);
  vbo1->SetArrayBuffer(sizeof colors, colors);

  vao->SetAttribute(0, 3, vbo0);
  vao->SetAttribute(1, 3, vbo1);
  vao->SetMode(GL_LINES);
  vao->SetNumElements(6);

  Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

  // mesh
  auto mesh_vao = std::make_shared<ArrayObject>();
  auto mesh_vbo0 = std::make_shared<BufferObject>();
  auto mesh_vbo1 = std::make_shared<BufferObject>();
  auto mesh_vbo2 = std::make_shared<BufferObject>();
  auto mesh_vbo3 = std::make_shared<BufferObject>();
  bool vao_set = false;
  std::shared_ptr<TextureObject> texture_object;

  // Kinect
  Kinect kinect;
  kinect.InitializeSensor();
  auto kinect_vao = std::make_shared<ArrayObject>();
  auto kinect_vbo0 = std::make_shared<BufferObject>();
  auto kinect_vbo1 = std::make_shared<BufferObject>();

  while (!ShouldClose())
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width_, height_);

    color_shader_->Use();
    color_shader_->LoadCamera(camera_);
    color_shader_->LoadModel(Eigen::Matrix4d::Identity());

    vao->Draw();

    light_shader_->Use();
    light_shader_->LoadCamera(camera_);
    light_shader_->LoadModel(Eigen::Matrix4d::Identity());

    for (int i = 0; i < 5; i++)
      light_shader_->LoadLight(i, lights[i]);

    light_shader_->LoadMaterial(material);

    // Mesh loader
#ifdef _WIN32
    auto mesh = RawMeshManager::LoadFromFile(
      "C:\\Users\\pjsdr\\Desktop\\documents\\fetch_ros\\fetch_description\\meshes\\torso_lift_link.dae");
#else
    auto mesh = RawMeshManager::LoadFromFile(
      "/home/jaesungp/catkin_ws/src/fetch_ros/fetch_description/meshes/torso_lift_link.dae");
#endif

    if (mesh != nullptr && !vao_set)
    {
      mesh_vbo0->SetArrayBuffer(sizeof(float) * mesh->vertices.cols() * 3, mesh->vertices.data());
      mesh_vbo1->SetArrayBuffer(sizeof(float) * mesh->normals.cols() * 3, mesh->normals.data());
      mesh_vbo2->SetArrayBuffer(sizeof(float) * mesh->texture_coordinates.cols() * 2, mesh->texture_coordinates.data());
      mesh_vbo3->SetElementArrayBuffer(sizeof(int) * mesh->triangles.cols() * 3, mesh->triangles.data());

      mesh_vao->SetAttribute(0, 3, mesh_vbo0);
      mesh_vao->SetAttribute(1, 3, mesh_vbo1);
      mesh_vao->SetAttribute(2, 2, mesh_vbo2);
      mesh_vao->SetElementArrayBuffer(mesh_vbo3);
      mesh_vao->SetMode(GL_TRIANGLES);
      mesh_vao->SetNumElements(mesh->triangles.cols() * 3);

      vao_set = true;
    }

    if (vao_set)
    {
      // Texture loader
      auto texture = TextureManager::LoadFromFile(mesh->texture_filename);

      if (texture != nullptr && texture_object == nullptr)
      {
        texture_object = std::make_shared<TextureObject>();
        texture_object->SetTexture(texture);
      }

      if (texture_object != nullptr)
        light_shader_->LoadTexture(texture_object);

      mesh_vao->Draw();
    }

    // Kinect
    kinect.Update();
    auto position_buffer = kinect.GetPointCloudPositionBuffer();
    auto color_buffer = kinect.GetPointCloudColorBuffer();

    if (!position_buffer.empty())
    {
      kinect_vbo0->AllocateDynamicArrayBuffer(position_buffer.size() * sizeof(float));
      kinect_vbo0->UpdateArrayBuffer(position_buffer.size() * sizeof(float), position_buffer.data());

      kinect_vbo1->AllocateDynamicArrayBuffer(color_buffer.size() * sizeof(float));
      kinect_vbo1->UpdateArrayBuffer(color_buffer.size() * sizeof(float), color_buffer.data());

      kinect_vao->SetAttribute(0, 3, kinect_vbo0);
      kinect_vao->SetAttribute(1, 3, kinect_vbo1);
      kinect_vao->SetMode(GL_POINTS);
      kinect_vao->SetNumElements(position_buffer.size() / 3);

      pointcloud_shader_->Use();
      pointcloud_shader_->LoadCamera(camera_);
      pointcloud_shader_->LoadModel(Eigen::Matrix4d::Identity());

      kinect_vao->Draw();
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
