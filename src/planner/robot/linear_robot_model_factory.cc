#include "planner/robot/linear_robot_model_factory.h"

namespace planner
{
namespace
{
LinearRobotJoint::JointType ConvertJointType(robot::RobotJoint::JointType type)
{
  switch (type)
  {
  case robot::RobotJoint::JointType::FIXED:
    return LinearRobotJoint::JointType::FIXED;

  case robot::RobotJoint::JointType::CONTINUOUS:
    return LinearRobotJoint::JointType::CONTINUOUS;

  case robot::RobotJoint::JointType::PRISMATIC:
    return LinearRobotJoint::JointType::PRISMATIC;

  case robot::RobotJoint::JointType::REVOLUTE:
    return LinearRobotJoint::JointType::REVOLUTE;

  default:
    return LinearRobotJoint::JointType::FIXED;
  }
}
}

LinearRobotModelFactory::LinearRobotModelFactory(const std::shared_ptr<robot::RobotModel>& robot_model)
  : robot_model_(robot_model)
{
}

std::shared_ptr<LinearRobotModel> LinearRobotModelFactory::CreateRobotModel()
{
  links_.push_back(LinearRobotLink());
  CreateRobotModel(robot_model_->GetBaseLink(), 0, Eigen::Affine3d::Identity());

  return std::make_shared<LinearRobotModel>(std::move(links_), std::move(joints_), endeffector_transform_);
}

void LinearRobotModelFactory::CreateRobotModel(const std::shared_ptr<robot::RobotLink>& link, int link_index, Eigen::Affine3d transform)
{
  for (auto visual : link->GetVisuals())
    links_[link_index].AddVisual(visual.mesh_filename, transform * visual.origin, visual.color);

  if (link->GetName() == endeffector_link_name_)
    endeffector_transform_ = transform;

  for (auto child_joint : link->GetChildJoints())
  {
    auto child_link = child_joint->GetChildLink();

    if (joint_names_.find(child_joint->GetName()) == joint_names_.cend())
    {
      Eigen::Affine3d joint_transform;
      if (joint_values_.find(child_joint->GetName()) == joint_values_.cend())
        joint_transform = child_joint->Transform(0.);
      else
        joint_transform = child_joint->Transform(joint_values_[child_joint->GetName()]);

      CreateRobotModel(child_link, link_index, transform * joint_transform);
    }
    else
    {
      links_.push_back(LinearRobotLink());

      LinearRobotJoint new_joint(ConvertJointType(child_joint->GetType()));
      new_joint.SetAxis(child_joint->GetAxis());
      new_joint.SetLimit(child_joint->GetLowerLimit(), child_joint->GetUpperLimit());
      new_joint.SetOrigin(transform * child_joint->GetOrigin());
      joints_.push_back(new_joint);

      CreateRobotModel(child_link, links_.size() - 1, Eigen::Affine3d::Identity());
    }
  }
}
}
