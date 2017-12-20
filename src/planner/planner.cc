#include "planner/planner.h"

namespace planner
{
void Planner::SetRobotModel(const std::shared_ptr<robot::RobotModel>& robot_model)
{
  robot_model_ = robot_model;
}

void Planner::SetEnvironment(const std::shared_ptr<Environment>& environment)
{
  environment_ = environment;
}
}
