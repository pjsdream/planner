#include "planner/robot/screw_robot_model_factory.h"

namespace planner
{
ScrewRobotModelFactory::ScrewRobotModelFactory(const std::shared_ptr<robot::RobotModel>& robot_model)
  : robot_model_(robot_model)
{
}

std::shared_ptr<ScrewRobotModel> ScrewRobotModelFactory::CreateScrewRobotModel()
{
  screw_robot_model_ = std::make_shared<ScrewRobotModel>(static_cast<int>(joint_names_.size()));

  CreateScrewRobotModel(robot_model_->GetBaseLink(), 0, Eigen::Affine3d::Identity());

  return screw_robot_model_;
}

void ScrewRobotModelFactory::CreateScrewRobotModel(const std::shared_ptr<robot::RobotLink>& link, int link_index, Eigen::Affine3d transform)
{
  for (auto visual : link->GetVisuals())
    screw_robot_model_->AddVisual(link_index, visual.mesh_filename, transform * visual.origin);

  for (auto collision : link->GetCollisions())
    screw_robot_model_->AddCollision(link_index, collision.mesh_filename, transform * collision.origin);

  if (link->GetName() == endeffector_link_name_)
    screw_robot_model_->SetEndeffectorTransform(transform);

  for (auto child_joint : link->GetChildJoints())
  {
    auto child_link = child_joint->GetChildLink();

    Eigen::Affine3d joint_transform;
    if (joint_values_.find(child_joint->GetName()) == joint_values_.cend())
      joint_transform = child_joint->Transform(0.);
    else
      joint_transform = child_joint->Transform(joint_values_[child_joint->GetName()]);

    if (joint_names_.find(child_joint->GetName()) == joint_names_.cend())
      CreateScrewRobotModel(child_link, link_index, transform * joint_transform);
    else
    {
      Eigen::Vector3d w, v;
      switch (child_joint->GetType())
      {
      case robot::RobotJoint::JointType::CONTINUOUS:
      case robot::RobotJoint::JointType::REVOLUTE:
        w = transform.linear() * child_joint->GetOrigin().linear() * child_joint->GetAxis();
        v = -w.cross(transform * child_joint->GetOrigin().translation());
        break;

      case robot::RobotJoint::JointType::PRISMATIC:
        w.setZero();
        v = transform.linear() * child_joint->GetOrigin().linear() * child_joint->GetAxis();
        break;

      case robot::RobotJoint::JointType::FIXED:
        w.setZero();
        v.setZero();
        break;
      }
      screw_robot_model_->SetScrewMotion(link_index, ScrewMotion(w, v));

      CreateScrewRobotModel(child_link, link_index + 1, transform * joint_transform);
    }
  }
}
}
