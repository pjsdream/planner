#ifndef PLANNER_ROBOT_ROBOT_LINK_H_
#define PLANNER_ROBOT_ROBOT_LINK_H_

#include <memory>
#include <vector>

#include <Eigen/Dense>

namespace robot
{
class RobotJoint;
class RobotLink
{
public:
  struct Inertial
  {
    Eigen::Affine3d origin = Eigen::Affine3d::Identity();
    double mass;
    Eigen::Matrix3d inertia;
  };

  struct Visual
  {
    Eigen::Affine3d origin;
    std::string mesh_filename;
    Eigen::Vector4f color;
  };

  struct Collision
  {
    Eigen::Affine3d origin;
    std::string mesh_filename;
  };

public:
  RobotLink() = delete;
  explicit RobotLink(std::string name)
      : name_(std::move(name))
  {
  }

  const auto& GetName() const
  {
    return name_;
  }

  void SetInertial(const Eigen::Affine3d& origin, double mass, const Eigen::Matrix3d& inertia)
  {
    inertial_.origin = origin;
    inertial_.mass = mass;
    inertial_.inertia = inertia;
  }

  const auto& GetInertial()
  {
    return inertial_;
  }

  void AddVisual(const Eigen::Affine3d& origin, const std::string& mesh_filename)
  {
    Visual visual;
    visual.origin = origin;
    visual.mesh_filename = mesh_filename;
    visual.color = Eigen::Vector4f(0.8f, 0.8f, 0.8f, 1.f);
    visuals_.push_back(visual);
  }

  void AddVisual(const Eigen::Affine3d& origin, const std::string& mesh_filename, const Eigen::Vector4f& color)
  {
    Visual visual;
    visual.origin = origin;
    visual.mesh_filename = mesh_filename;
    visual.color = color;
    visuals_.push_back(visual);
  }

  const auto& GetVisuals() const
  {
    return visuals_;
  }

  void AddCollision(const Eigen::Affine3d& origin, const std::string& mesh_filename)
  {
    Collision collision;
    collision.origin = origin;
    collision.mesh_filename = mesh_filename;
    collisions_.push_back(collision);
  }

  const auto& GetCollisions() const
  {
    return collisions_;
  }

  //
  // Tree structure
  //
  void AddChildJoint(const std::shared_ptr<RobotJoint>& joint)
  {
    child_joints_.push_back(joint);
  }

  const auto& GetChildJoints() const
  {
    return child_joints_;
  }

  void SetParentJoint(const std::shared_ptr<RobotJoint>& joint)
  {
    parent_joint_ = joint;
  }

  std::shared_ptr<RobotJoint> GetParentJoint()
  {
    if (parent_joint_.expired())
      return nullptr;
    return parent_joint_.lock();
  }

private:
  std::string name_;
  Inertial inertial_;
  std::vector<Visual> visuals_;
  std::vector<Collision> collisions_;

  std::vector<std::shared_ptr<RobotJoint>> child_joints_;
  std::weak_ptr<RobotJoint> parent_joint_;
};
}

#endif // PLANNER_ROBOT_ROBOT_LINK_H_
