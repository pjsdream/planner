#ifndef PLANNER_PLANNER_ROBOT_ROBOT_STATE_H_
#define PLANNER_PLANNER_ROBOT_ROBOT_STATE_H_

#include <vector>
#include <map>
#include <memory>

#include <Eigen/Dense>

#include "planner/robot/linear_robot_model.h"

namespace planner
{
class RobotState
{
private:
  using MapAffine3d = std::map<std::string, Eigen::Affine3d, std::less<std::string>, Eigen::aligned_allocator<Eigen::Affine3d>>;
  using MapMatrix4d = std::map<std::string, Eigen::Matrix4d, std::less<std::string>, Eigen::aligned_allocator<Eigen::Matrix4d>>;
  using VectorAffine3d = std::vector<Eigen::Affine3d, Eigen::aligned_allocator<Eigen::Affine3d>>;
  using VectorMatrix4d = std::vector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d>>;
  using VectorQuaterniond = std::vector<Eigen::Quaterniond, Eigen::aligned_allocator<Eigen::Quaterniond>>;

public:
  RobotState() = delete;
  RobotState(const std::shared_ptr<LinearRobotModel>& robot_model);

  void SetJointValue(int index, double joint_value);

  auto GetJointValue(int index)
  {
    return joint_values_(index);
  }

  void PerformForwardKinematics();
  void PerformDerivativeForwardKinematics();
  Eigen::Affine3d GetLinkPose(int link_index);
  Eigen::Affine3d GetEndeffectorLinkPose();

  void MoveToGripPose(const Eigen::Affine3d& link_pose);

private:
  std::shared_ptr<LinearRobotModel> robot_model_;
  Eigen::VectorXd joint_values_;

  // Forward kinematics
  VectorAffine3d link_poses_;
  VectorMatrix4d forward_kinematics_derivatives_;
};
}

#endif // PLANNER_PLANNER_ROBOT_ROBOT_STATE_H_
