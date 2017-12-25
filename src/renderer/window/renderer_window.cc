#include "renderer/window/renderer_window.h"

#include <iostream>

namespace renderer
{
RendererWindow::RendererWindow()
    : RendererBaseWindow(), camera_(std::make_shared<Camera>())
{
  mouse_button_states_[0] = false;
  mouse_button_states_[1] = false;
  mouse_button_states_[2] = false;

  // GL initialize
  glClearColor(0.8, 0.8, 0.8, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  // Register primitives
  {
    auto object = std::make_shared<GlObject>();
    object->CreateCube();
    object_manager_.RegisterObject("cube", object);
  }
  {
    auto object = std::make_shared<GlObject>();
    object->CreateSphere();
    object_manager_.RegisterObject("sphere", object);
  }
  {
    auto object = std::make_shared<GlObject>();
    object->CreateCylinder();
    object_manager_.RegisterObject("cylinder", object);
  }
  {
    auto object = std::make_shared<GlObject>();
    object->CreateCone();
    object_manager_.RegisterObject("cone", object);
  }
}

void RendererWindow::LoadMeshObject(const std::string& name, const std::string& filename)
{
  object_manager_.GetObjectOrCreateFromFile(name, filename);
}

void RendererWindow::UpdateScene(const std::shared_ptr<Scene>& scene)
{
  // Lights
  light_shader_.Start();
  for (int i = 0; i < LightShader::NUM_LIGHTS; i++)
    light_shader_.LoadLight(i, scene->GetLight(i));
  light_shader_.End();

  // Instances
  ClearInstances();
  TraverseScene(scene->GetRootNode());

  Display();
}

void RendererWindow::TraverseScene(const std::shared_ptr<SceneNode>& node, Eigen::Affine3d transform)
{
  // Load mesh objects
  for (auto name_and_filename : node->GetMeshNameAndFilenames())
  {
    const auto& name = name_and_filename.first;
    const auto& filename = name_and_filename.second;
    LoadMeshObject(name, filename);
  }

  transform = transform * node->GetTransform();

  for (auto object : node->GetObjects())
  {
    GlInstances::Instance instance;
    instance.transform = transform;
    instance.has_global_color = object->HasGlobalColor();
    instance.global_color = object->GetGlobalColor();

    const auto& name = object->GetName();
    auto gl_object = object_manager_.GetObject(name);

    if (instances_.find(name) == instances_.cend())
      instances_[name].SetObject(gl_object);

    instances_[name].AddInstance(instance);
  }

  for (auto child : node->GetChildren())
    TraverseScene(child, transform);
}

void RendererWindow::ClearInstances()
{
  for (auto& instances : instances_)
    instances.second.ClearInstances();
}

void RendererWindow::Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  light_shader_.Start();
  light_shader_.LoadCamera(camera_);

  int cnt = 0;
  for (auto instances : instances_)
  {
    auto object = instances.second.GetObject();

    for (const auto& instance : instances.second.GetInstances())
    {
      if (object->HasTexture())
        light_shader_.UseTexture(object->GetTexture());

      else if (instance.has_global_color)
        light_shader_.UseGlobalColor(instance.global_color);

      else // Use default color
        light_shader_.UseGlobalColor(Eigen::Vector3f(0.8, 0.8, 0.8));

      light_shader_.LoadModel(instance.transform);

      object->Draw();

      cnt++;
    }
  }

  light_shader_.End();

  SwapBuffers();
}

void RendererWindow::Reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  camera_->SetAspect((double) width / height);
}

void RendererWindow::Mouse(int button, int state, double x, double y)
{
  mouse_position_x_ = x;
  mouse_position_y_ = y;

  switch (button)
  {
    case 0:
      mouse_button_states_[0] = state == 1;
      break;

    case 1:
      mouse_button_states_[1] = state == 1;
      break;

    case 2:
      mouse_button_states_[2] = state == 1;
      break;

    default:
      std::cerr << "unknown mouse button: " << button << "\n";
      break;
  }
}

void RendererWindow::Motion(double x, double y)
{
  auto dx = x - mouse_position_x_;
  auto dy = y - mouse_position_y_;

  int state_bitset = mouse_button_states_[0] | (mouse_button_states_[1] << 1) | (mouse_button_states_[2] << 2);

  switch (state_bitset)
  {
    case 1: // rotation on left click drag
      camera_->RotatePixel(dx, dy);
      break;

    case 4: // translation on left click drag
      camera_->TranslatePixel(dx, dy);
      break;

    case 5: // zoom on left & right click drag
      camera_->ZoomPixel(dx, dy);
      break;

    default:
      break;
  }

  mouse_position_x_ = x;
  mouse_position_y_ = y;

  // TODO: redisplay
  // glutPostRedisplay();
}

void RendererWindow::Idle()
{
  // Debug: random move animation
  /*
  static auto scene = std::make_shared<Scene>();

  static auto sphere = std::make_shared<SceneObject>("sphere");
  static auto cube = std::make_shared<SceneObject>("cube");
  static auto cylinder = std::make_shared<SceneObject>("cylinder");
  static auto cone = std::make_shared<SceneObject>("cone");
  static auto mesh = std::make_shared<SceneObject>("torso_lift_link");

  static bool initialized = false;
  if (!initialized)
  {
    object_manager_.GetObjectOrCreateFromFile("torso_lift_link",
                                              "/home/jaesungp/catkin_ws/src/fetch_ros/fetch_description/meshes/torso_lift_link.dae");

    sphere->SetGlobalColor(Eigen::Vector3f(1, 0, 0));
    cube->SetGlobalColor(Eigen::Vector3f(0.8, 0.8, 0.8));
    cylinder->SetGlobalColor(Eigen::Vector3f(0, 1, 0));
    cone->SetGlobalColor(Eigen::Vector3f(0, 0, 1));

    scene->GetRootNode()->Scale(Eigen::Vector3d(0.5, 0.5, 0.5)).AttachObject(sphere);

    auto cube_node = std::make_shared<SceneNode>();
    cube_node->AttachObject(cube);
    scene->GetRootNode()->AddChild(cube_node);

    auto cylinder_node = std::make_shared<SceneNode>();
    cylinder_node->AttachObject(cylinder);
    cube_node->AddChild(cylinder_node);

    auto cone_node = std::make_shared<SceneNode>();
    cone_node->AttachObject(cone);
    cylinder_node->AddChild(cone_node);

    auto mesh_node = std::make_shared<SceneNode>();
    mesh_node->AttachObject(mesh);
    cone_node->AddChild(mesh_node);

    initialized = true;
  }

  auto cube_node = scene->GetRootNode()->GetChildren()[0];
  auto cylinder_node = cube_node->GetChildren()[0];
  auto cone_node = cylinder_node->GetChildren()[0];
  auto mesh_node = cone_node->GetChildren()[0];

  Eigen::Affine3d cube_transform = Eigen::Affine3d::Identity();
  cube_transform.translate(Eigen::Vector3d(2, std::rand() / 2147483647. - 0.5, std::rand() / 2147483647. - 0.5));
  cube_node->SetTransform(cube_transform);

  Eigen::Affine3d cylinder_transform = Eigen::Affine3d::Identity();
  cylinder_transform.translate(Eigen::Vector3d(2, std::rand() / 2147483647. - 0.5, std::rand() / 2147483647. - 0.5));
  cylinder_node->SetTransform(cylinder_transform);

  Eigen::Affine3d cone_transform = Eigen::Affine3d::Identity();
  cone_transform.translate(Eigen::Vector3d(2, std::rand() / 2147483647. - 0.5, std::rand() / 2147483647. - 0.5));
  cone_node->SetTransform(cone_transform);

  Eigen::Affine3d mesh_transform = Eigen::Affine3d::Identity();
  mesh_transform.translate(Eigen::Vector3d(2, std::rand() / 2147483647. - 0.5, std::rand() / 2147483647. - 0.5));
  mesh_node->SetTransform(mesh_transform);

  UpdateScene(scene);
   */

  // Debug: mesh load test
  static auto scene = std::make_shared<Scene>();

  static auto mesh = std::make_shared<SceneObject>("elbow_link");

  static bool initialized = false;
  if (!initialized)
  {
    scene->GetRootNode()->AttachObject(mesh);

    object_manager_.GetObjectOrCreateFromFile(
        "elbow_link", "C:/Users/pjsdr_000/Desktop/documents/fetch_ros/fetch_description/meshes/elbow_flex_link.dae");

    initialized = true;
  }

  UpdateScene(scene);
}
}
