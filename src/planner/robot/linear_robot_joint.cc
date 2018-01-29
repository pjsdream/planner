#include "planner/robot/linear_robot_joint.h"

#include <iostream>

namespace planner
{
double LinearRobotJoint::Clamp(double joint_value) const
{
  switch (type_)
  {
  case JointType::REVOLUTE:
  case JointType::PRISMATIC:
    return std::min(GetUpperLimit(), std::max(GetLowerLimit(), joint_value));

  case JointType::FIXED:
    return 0.;

  case JointType::CONTINUOUS:
    return joint_value;
  }

  return 0.;
}

Eigen::Affine3d LinearRobotJoint::Transform(double joint_value) const
{
  if (type_ == JointType::FIXED)
    return origin_;

  Eigen::Affine3d m = origin_;

  joint_value = Clamp(joint_value);

  switch (type_)
  {
  case JointType::REVOLUTE:
  case JointType::CONTINUOUS:
    m.rotate(Eigen::AngleAxisd(joint_value, axis_));
    break;

  case JointType::PRISMATIC:
    m.translate(joint_value * axis_);
    break;

  default:
    break;
  }

  return m;
}

Eigen::Matrix4d LinearRobotJoint::TransformDerivative(double joint_value) const
{
  Eigen::Matrix4d m;

  joint_value = Clamp(joint_value);

  switch (type_)
  {
  case JointType::FIXED:
    m = Eigen::Matrix4d::Zero();
    break;

  case JointType::REVOLUTE:
  case JointType::CONTINUOUS:
  {
    const double c = std::cos(joint_value);
    const double s = std::sin(joint_value);
    const double dc = -s;
    const double ds = c;

    const double x = axis_(0);
    const double y = axis_(1);
    const double z = axis_(2);

    m << dc + x * x * (-dc), x * y * (-dc) - z * ds, x * z * (-dc) + y * ds, 0.,
      y * x * (-dc) + z * ds, dc + y * y * (-dc), y * z * (-dc) - x * ds, 0.,
      z * x * (-dc) - y * ds, z * y * (-dc) + x * ds, dc + z * z * (-dc), 0.,
      0., 0., 0., 0.;
  }
  break;

  case JointType::PRISMATIC:
  {
    if (joint_value == GetLowerLimit() || joint_value == GetUpperLimit())
    {
      m.setZero();
    }
    else
    {
      m << 0., 0., 0., axis_(0),
        0., 0., 0., axis_(1),
        0., 0., 0., axis_(2),
        0., 0., 0., 0.;
    }
  }
    break;

  default:
    break;
  }

  return origin_.matrix() * m;
}

Eigen::Vector3d LinearRobotJoint::Translation(double joint_value) const
{
  Eigen::Matrix4d m;

  joint_value = Clamp(joint_value);

  switch (type_)
  {
  case JointType::FIXED:
  case JointType::REVOLUTE:
  case JointType::CONTINUOUS:
    return Eigen::Vector3d::Zero();

  case JointType::PRISMATIC:
    return axis_ * joint_value;
  }
}

Eigen::Quaterniond LinearRobotJoint::Quaternion(double joint_value) const
{
  Eigen::Matrix4d m;

  joint_value = Clamp(joint_value);

  switch (type_)
  {
  case JointType::FIXED:
  case JointType::PRISMATIC:
    return Eigen::Quaterniond(0., 0., 0., 0.);

  case JointType::REVOLUTE:
  case JointType::CONTINUOUS:
  {
    const double sin = std::sin(joint_value / 2.);
    const double cos = std::cos(joint_value / 2.);
    return Eigen::Quaterniond(cos, axis_(0) * sin, axis_(1) * sin, axis_(2) * sin);
  }
  }
}

Eigen::Vector3d LinearRobotJoint::TranslationDerivative(double joint_value) const
{
  Eigen::Matrix4d m;

  joint_value = Clamp(joint_value);

  switch (type_)
  {
  case JointType::FIXED:
  case JointType::REVOLUTE:
  case JointType::CONTINUOUS:
    return Eigen::Vector3d::Zero();

  case JointType::PRISMATIC:
    return axis_;
  }
}

Eigen::Quaterniond LinearRobotJoint::QuaternionDerivative(double joint_value) const
{
  Eigen::Matrix4d m;

  joint_value = Clamp(joint_value);

  switch (type_)
  {
  case JointType::FIXED:
  case JointType::PRISMATIC:
    return Eigen::Quaterniond(0., 0., 0., 0.);

  case JointType::REVOLUTE:
  case JointType::CONTINUOUS:
  {
    const double sin = std::sin(joint_value / 2.);
    const double cos = std::cos(joint_value / 2.);
    return Eigen::Quaterniond(0.5 * -sin, 0.5 * axis_(0) * cos, 0.5 * axis_(1) * cos, 0.5 * axis_(2) * cos);
  }
  }
}
}
