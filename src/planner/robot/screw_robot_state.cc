#include "planner/robot/screw_robot_state.h"

#include <iostream>

namespace planner
{
ScrewRobotState::ScrewRobotState(const std::shared_ptr<ScrewRobotModel> robot_model)
  : robot_model_(robot_model), joint_values_(robot_model->GetNumJoints()), fk_transforms_(robot_model->GetNumJoints())
{
  joint_values_.setZero();

  jacobian_.resize(Eigen::NoChange, robot_model->GetNumJoints());
}

void ScrewRobotState::PerformForwardKinematics()
{
  fk_transforms_[0] = robot_model_->GetScrewMotion(0).Exponential(joint_values_[0]);
  for (int i = 1; i < robot_model_->GetNumJoints(); i++)
    fk_transforms_[i] = fk_transforms_[i - 1] * robot_model_->GetScrewMotion(i).Exponential(joint_values_[i]);
}

void ScrewRobotState::PerformDifferentialKinematics()
{
  PerformForwardKinematics();

  jacobian_.col(0) = robot_model_->GetScrewMotion(0).Vector();
  for (int i = 1; i < robot_model_->GetNumJoints(); i++)
    jacobian_.col(i) = robot_model_->GetScrewMotion(i).Adjoint(fk_transforms_[i-1]).Vector();
}

Eigen::Affine3d ScrewRobotState::EndeffectorPose()
{
  return fk_transforms_.back() * robot_model_->GetEndeffectorTransform();
}

std::vector<ScrewRobotModel::Link::Visual> ScrewRobotState::GetVisuals(int link_index) const
{
  std::vector<ScrewRobotModel::Link::Visual> visuals;

  for (const auto& visual : robot_model_->GetVisuals(link_index))
  {
    ScrewRobotModel::Link::Visual transformed_visual;
    transformed_visual.filename = visual.filename;

    if (link_index == 0)
      transformed_visual.transform = visual.transform;
    else
      transformed_visual.transform = fk_transforms_[link_index - 1] * visual.transform;

    visuals.push_back(transformed_visual);
  }

  return visuals;
}

void ScrewRobotState::InverseKinematics(const Eigen::Affine3d& endeffector_transform)
{
  auto endeffector_transform_inverse = endeffector_transform.inverse();

  for (int i = 0; i < 10; i++)
  {
    PerformDifferentialKinematics();

    Eigen::Matrix4d m((EndeffectorPose() * endeffector_transform_inverse).matrix() - Eigen::Matrix4d::Identity());
    Eigen::Vector3d w = Eigen::Vector3d(m(2, 1), m(0, 2), m(1, 0)).normalized();
    Eigen::Vector3d v(m.block(0, 3, 3, 1));
    ScrewMotion s(w, v);
    Eigen::VectorXd dtheta = jacobian_.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(s.Vector());
    std::cout << "jacobian:\n" << jacobian_ << "\ns:\n" << s.Vector() << "\ndtheta:\n" << dtheta << "\n\n";
    joint_values_.block(0, 0, dtheta.rows(), 1) += dtheta;
  }
}
}
