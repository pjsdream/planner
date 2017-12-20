#ifndef PLANNER_SCENE_SCENE_NODE_H_
#define PLANNER_SCENE_SCENE_NODE_H_

#include <memory>
#include <unordered_map>

#include <Eigen/Dense>

#include "scene_object.h"

namespace renderer
{
class SceneNode
{
public:
  SceneNode();

  //
  // Transforms
  //
  void SetTransform(const Eigen::Affine3d& transform)
  {
    transform_ = transform;
  }

  const auto& GetTransform() const
  {
    return transform_;
  }

  SceneNode& Scale(double scale_factor)
  {
    transform_.scale(scale_factor);
    return *this;
  }

  SceneNode& Scale(const Eigen::Vector3d& scale)
  {
    transform_.scale(scale);
    return *this;
  }

  SceneNode& Translate(const Eigen::Vector3d& translation)
  {
    transform_.translate(translation);
    return *this;
  }

  //
  // Objects
  //
  void LoadMeshObject(const std::string& name, const std::string& filename)
  {
    mesh_name_to_filenames_[name] = filename;
  }

  const auto& GetMeshNameAndFilenames() const
  {
    return mesh_name_to_filenames_;
  }

  void AttachObject(const std::shared_ptr<SceneObject>& object)
  {
    objects_.push_back(object);
  }

  const auto& GetObjects() const
  {
    return objects_;
  }

  //
  // Tree structure
  //
  std::shared_ptr<SceneNode> CreateChild()
  {
    auto child = std::make_shared<SceneNode>();
    children_.push_back(child);
    return child;
  }

  const auto& GetChildren() const
  {
    return children_;
  }

  //
  // Shapes
  //
  void AddArrow(const Eigen::Vector3d& p, const Eigen::Vector3d& q, const Eigen::Vector3f& color);
  void AddLocalCoodinate(const Eigen::Affine3d& transform, double length);

private:
  // Mesh filenames
  std::unordered_map<std::string, std::string> mesh_name_to_filenames_;

  std::vector<std::shared_ptr<SceneObject>> objects_;

  // Operations
  Eigen::Affine3d transform_;
  Eigen::Vector3f color_;

  std::vector<std::shared_ptr<SceneNode>> children_;
};
}

#endif //PLANNER_SCENE_SCENE_NODE_H_
