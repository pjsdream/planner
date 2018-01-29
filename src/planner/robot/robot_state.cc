#include "planner/robot/robot_state.h"

#include <iostream>

namespace planner
{
RobotState::RobotState(const std::shared_ptr<LinearRobotModel>& robot_model)
  : robot_model_(robot_model)
{
  joint_values_.resize(robot_model_->GetJoints().size());
  joint_values_.setZero();
  link_poses_.resize(robot_model_->GetLinks().size());
  forward_kinematics_derivatives_.resize(robot_model_->GetJoints().size());
}

void RobotState::SetJointValue(int index, double joint_value)
{
  const auto& joint = robot_model_->GetJoint(index);
  if (joint.HasLimits())
    joint_values_(index) = std::min(joint.GetUpperLimit(), std::max(joint.GetLowerLimit(), joint_value));
  else
    joint_values_(index) = joint_value;
}

void RobotState::PerformForwardKinematics()
{
  link_poses_[0] = Eigen::Affine3d::Identity();

  const auto& links = robot_model_->GetLinks();
  const auto& joints = robot_model_->GetJoints();

  for (int i = 1; i < links.size(); i++)
  {
    const auto& link = links[i];
    link_poses_[i] = link_poses_[i - 1] * joints[i - 1].Transform(GetJointValue(i - 1));
  }
}

void RobotState::PerformDerivativeForwardKinematics()
{
  PerformForwardKinematics();

  const auto& links = robot_model_->GetLinks();
  const auto& joints = robot_model_->GetJoints();

  VectorMatrix4d forward_accumulations(joints.size());
  VectorMatrix4d backward_accumulations(joints.size());

  for (int i = 0; i < joints.size(); i++)
  {
    const auto& joint = joints[i];
    if (i == 0)
      forward_accumulations[i] = joint.Transform(GetJointValue(i)).matrix();
    else
      forward_accumulations[i] = forward_accumulations[i - 1] * joint.Transform(GetJointValue(i)).matrix();
  }

  for (int i = joints.size() - 1; i >= 0; i--)
  {
    const auto& joint = joints[i];
    if (i == joints.size() - 1)
      backward_accumulations[i] = joint.Transform(GetJointValue(i)).matrix();
    else
      backward_accumulations[i] = joint.Transform(GetJointValue(i)).matrix() * backward_accumulations[i + 1];
  }

  for (int i = 0; i < joints.size(); i++)
  {
    const auto& joint = joints[i];

    if (i == 0)
      forward_kinematics_derivatives_[i] = Eigen::Matrix4d::Identity();
    else
      forward_kinematics_derivatives_[i] = forward_accumulations[i - 1];

    forward_kinematics_derivatives_[i] *= joint.TransformDerivative(GetJointValue(i));

    if (i != joints.size() - 1)
      forward_kinematics_derivatives_[i] *= backward_accumulations[i + 1];
  }
}

Eigen::Affine3d RobotState::GetLinkPose(int link_index)
{
  return link_poses_[link_index];
}

Eigen::Affine3d RobotState::GetEndeffectorLinkPose()
{
  return link_poses_.back() * robot_model_->GetEndeffectorTransform();
}

void RobotState::MoveToGripPose(const Eigen::Affine3d& link_pose)
{
  Eigen::Vector3d target_position = link_pose.translation();
  Eigen::Vector3d target_normal = -link_pose.linear().col(2);

  constexpr int max_iterations = 10;
  constexpr double alpha = 0.1;

  for (int iteration = 0; iteration < max_iterations; iteration++)
  {
    PerformDerivativeForwardKinematics();

    auto ee_pose = GetEndeffectorLinkPose();
    Eigen::Vector3d ee_position = ee_pose.translation();
    Eigen::Vector3d ee_normal = ee_pose.linear().col(0);
    Eigen::Vector3d position_difference = target_position - ee_position;
    Eigen::Vector3d normal_difference = target_normal - ee_normal;

    Eigen::Matrix<double, 3, -1> jacobian;
    jacobian.resize(Eigen::NoChange, robot_model_->GetJoints().size());

    Eigen::Matrix<double, 3, 1> b;
    b = position_difference;

    for (int i = 0; i < robot_model_->GetJoints().size(); i++)
    {
      const auto& derivative_matrix = forward_kinematics_derivatives_[i];
      jacobian.col(i) = derivative_matrix.block(0, 3, 3, 1);
    }

    Eigen::VectorXd solution = jacobian.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

    for (int i = 0; i < robot_model_->GetJoints().size(); i++)
      SetJointValue(i, GetJointValue(i) + solution(i));
  }

  for (int iteration = 0; iteration < max_iterations; iteration++)
  {
    PerformDerivativeForwardKinematics();

    auto ee_pose = GetEndeffectorLinkPose();
    Eigen::Vector3d ee_position = ee_pose.translation();
    Eigen::Vector3d ee_normal = ee_pose.linear().col(0);
    Eigen::Vector3d position_difference = target_position - ee_position;
    Eigen::Vector3d normal_difference = target_normal - ee_normal;

    Eigen::Matrix<double, 6, -1> jacobian;
    jacobian.resize(Eigen::NoChange, robot_model_->GetJoints().size());

    Eigen::Matrix<double, 6, 1> b;
    b.block(0, 0, 3, 1) = position_difference;
    b.block(3, 0, 3, 1) = normal_difference;

    for (int i = 0; i < robot_model_->GetJoints().size(); i++)
    {
      const auto& derivative_matrix = forward_kinematics_derivatives_[i];
      jacobian.block(0, i, 3, 1) = derivative_matrix.block(0, 3, 3, 1);
      jacobian.block(3, i, 3, 1) = derivative_matrix.block(0, 0, 3, 1);
    }

    Eigen::VectorXd solution = jacobian.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

    for (int i = 0; i < robot_model_->GetJoints().size(); i++)
      SetJointValue(i, GetJointValue(i) + solution(i));
  }

  /*
  for (int i = 0; i < robot_model_->GetJoints().size(); i++)
    std::cout << "joint[" << i << "] = " << GetJointValue(i) << "\n";
    */
}
}
