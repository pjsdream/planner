#ifndef PLANNER_RENDERER_SCENE_SCENE_OBJECT_H_
#define PLANNER_RENDERER_SCENE_SCENE_OBJECT_H_

#include <string>

namespace renderer
{
class SceneObject
{
public:
  SceneObject() = delete;
  explicit SceneObject(const std::string& name)
      : name_(name), has_global_color_(true), global_color_{0.8f, 0.8f, 0.8f}
  {
  }

  const auto& GetName() const
  {
    return name_;
  }

  auto HasGlobalColor() const
  {
    return has_global_color_;
  }

  void SetGlobalColor(const Eigen::Vector3f& color)
  {
    has_global_color_ = true;
    global_color_ = color;
  }

  const auto& GetGlobalColor() const
  {
    return global_color_;
  }

private:
  std::string name_;
  bool has_global_color_;
  Eigen::Vector3f global_color_;
};
}

#endif // PLANNER_RENDERER_SCENE_SCENE_OBJECT_H_
