#include "planner/robot/linear_robot_model.h"

#include <iostream>

namespace planner
{
LinearRobotModel::LinearRobotModel()
{
}

LinearRobotModel::LinearRobotModel(std::vector<LinearRobotLink>&& links, std::vector<LinearRobotJoint>&& joints, const Eigen::Affine3d& endeffector_transform)
  : links_(std::move(links)), joints_(std::move(joints)), endeffector_transform_(endeffector_transform)
{
}
}
