#ifndef PLANNER_ROBOT_ROBOT_JOINT_H_
#define PLANNER_ROBOT_ROBOT_JOINT_H_

#include <string>
#include <memory>

#include <Eigen/Dense>

namespace robot
{
class RobotLink;
class RobotJoint
{
public:
  enum class JointType
  {
    REVOLUTE,
    CONTINUOUS,
    PRISMATIC,
    FIXED,
  };

  struct Limit
  {
    double lower = 0.;
    double upper = 0.;
  };

public:
  RobotJoint() = delete;
  RobotJoint(std::string name, JointType type)
      : name_(std::move(name)), type_(type), origin_(Eigen::Affine3d::Identity()), axis_{0., 0., 1.}
  {
  }

  const auto& GetName() const
  {
    return name_;
  }

  void SetOrigin(const Eigen::Affine3d& origin)
  {
    origin_ = origin;
  }

  const auto& GetOrigin() const
  {
    return origin_;
  }

  void SetAxis(const Eigen::Vector3d& axis)
  {
    axis_ = axis;
  }

  const auto& GetAxis() const
  {
    return axis_;
  }

  void SetLimit(double lower, double upper)
  {
    limit_.lower = lower;
    limit_.upper = upper;
  }

  auto GetLowerLimit() const
  {
    return limit_.lower;
  }

  auto GetUpperLimit() const
  {
    return limit_.upper;
  }

  double Clamp(double joint_value) const;
  Eigen::Affine3d Transform(double joint_value) const;

  //
  // Tree structure
  //
  void SetChildLink(const std::shared_ptr<RobotLink>& link)
  {
    child_link_ = link;
  }

  auto GetChildLink() const
  {
    return child_link_;
  }

  void SetParentLink(const std::shared_ptr<RobotLink>& link)
  {
    parent_link_ = link;
  }

  std::shared_ptr<RobotLink> GetParentLink()
  {
    return parent_link_.lock();
  }

private:
  std::string name_;
  JointType type_;

  Eigen::Affine3d origin_;
  Eigen::Vector3d axis_;
  Limit limit_;

  std::shared_ptr<RobotLink> child_link_;
  std::weak_ptr<RobotLink> parent_link_;
};
}

#endif // PLANNER_ROBOT_ROBOT_JOINT_H_
