#ifndef PLANNER_ROBOT_SCREW_ROBOT_STATE_H_
#define PLANNER_ROBOT_SCREW_ROBOT_STATE_H_

#include <memory>

#include "planner/robot/screw_robot_model.h"

namespace planner
{
class ScrewRobotState
{
private:
  using VectorAffine3d = std::vector<Eigen::Affine3d, Eigen::aligned_allocator<Eigen::Affine3d>>;
  using VectorMatrix4d = std::vector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d>>;

public:
  ScrewRobotState() = delete;

  ScrewRobotState(const std::shared_ptr<ScrewRobotModel> robot_model);

  void SetJointValue(int joint_index, double joint_value)
  {
    joint_values_[joint_index] = joint_value;
  }

  std::vector<ScrewRobotModel::Link::Visual> GetVisuals(int link_index) const;

  void PerformForwardKinematics();
  void PerformDifferentialKinematics();

  Eigen::Affine3d EndeffectorPose();

  void InverseKinematics(const Eigen::Affine3d& endeffector_transform);

private:
  std::shared_ptr<ScrewRobotModel> robot_model_;
  Eigen::VectorXd joint_values_;

  VectorAffine3d fk_transforms_;
  Eigen::Matrix<double, 6, -1> jacobian_;
};
}

#endif // PLANNER_ROBOT_SCREW_ROBOT_STATE_H_