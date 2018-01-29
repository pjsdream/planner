#ifndef PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_MODEL_H_
#define PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_MODEL_H_

#include <vector>

#include "planner/robot/linear_robot_joint.h"
#include "planner/robot/linear_robot_link.h"

namespace planner
{
class LinearRobotModel
{
public:
  LinearRobotModel();
  LinearRobotModel(std::vector<LinearRobotLink>&& links, std::vector<LinearRobotJoint>&& joints, const Eigen::Affine3d& endeffector_transform);

  const auto& GetLinks() const
  {
    return links_;
  }

  const auto& GetJoints() const
  {
    return joints_;
  }

  const auto& GetJoint(int index) const
  {
    return joints_[index];
  }

  const auto& GetEndeffectorTransform() const
  {
    return endeffector_transform_;
  }

private:
  std::vector<LinearRobotLink> links_;
  std::vector<LinearRobotJoint> joints_;
  Eigen::Affine3d endeffector_transform_;
};
}

#endif // PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_MODEL_H_
