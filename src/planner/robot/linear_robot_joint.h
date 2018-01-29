#ifndef PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_JOINT_H_
#define PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_JOINT_H_

#include <Eigen/Dense>

namespace planner
{
class LinearRobotJoint
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
  LinearRobotJoint() = delete;
  LinearRobotJoint(JointType type)
    : type_(type), origin_(Eigen::Affine3d::Identity()), axis_(0., 0., 1.)
  {
  }

  bool IsContinuous() const
  {
    return type_ == JointType::CONTINUOUS;
  }

  bool IsFixed() const
  {
    return type_ == JointType::FIXED;
  }

  bool HasLimits() const
  {
    return type_ == JointType::REVOLUTE || type_ == JointType::PRISMATIC;
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
  Eigen::Matrix4d TransformDerivative(double joint_value) const;

  Eigen::Vector3d Translation(double joint_value) const;
  Eigen::Quaterniond Quaternion(double joint_value) const;
  Eigen::Vector3d TranslationDerivative(double joint_value) const;
  Eigen::Quaterniond QuaternionDerivative(double joint_value) const;

private:
  JointType type_;

  Eigen::Affine3d origin_;
  Eigen::Vector3d axis_;
  Limit limit_;
};
}

#endif // PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_JOINT_H_
