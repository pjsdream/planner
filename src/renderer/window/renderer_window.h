#ifndef PLANNER_RENDERER_WINDOW_RENDERER_WINDOW_H_
#define PLANNER_RENDERER_WINDOW_RENDERER_WINDOW_H_

#include <unordered_map>

#include "renderer/scene/scene.h"
#include "renderer/camera/camera.h"
#include "renderer/window/renderer_base_window.h"
#include "renderer/gl/shader/light_shader.h"
#include "renderer/gl/resource/gl_instances.h"
#include "renderer/gl/resource/gl_object_manager.h"

namespace renderer
{
class RendererWindow : public RendererBaseWindow
{
public:
  RendererWindow();

  void UpdateScene(const std::shared_ptr<Scene>& scene);

  void LoadMeshObject(const std::string& name, const std::string& filename);

protected:
  void Display() override;
  void Reshape(int width, int height) override;
  void Mouse(int button, int state, double x, double y) override;
  void Motion(double x, double y) override;
  void Idle() override;

private:
  // Shaders
  LightShader light_shader_;

  // Scene
  void TraverseScene(const std::shared_ptr<SceneNode>& node, Eigen::Affine3d transform = Eigen::Affine3d::Identity());

  // Camera
  std::shared_ptr<Camera> camera_;

  // Instances
  GlObjectManager object_manager_;
  std::unordered_map<std::string, GlInstances> instances_;
  void ClearInstances();

  // Mouse left, middle, and right buttons. true on mouse down.
  bool mouse_button_states_[3];
  double mouse_position_x_;
  double mouse_position_y_;
};
}

#endif // PLANNER_RENDERER_WINDOW_RENDERER_WINDOW_H_
