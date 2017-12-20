#include "robot/robot_joint.h"

#include <iostream>

namespace robot
{
double RobotJoint::Clamp(double joint_value) const
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
}

Eigen::Affine3d RobotJoint::Transform(double joint_value) const
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
}
