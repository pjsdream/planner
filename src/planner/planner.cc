#include "planner/planner.h"

namespace planner
{
void Planner::SetRobotModel(const std::shared_ptr<robot::RobotModel>& robot_model)
{
  robot_model_ = robot_model;
}

void Planner::UpdateEnvironment(const std::shared_ptr<Environment>& environment)
{
  environment_ = environment;

  // TODO: get obstacles from the environment
}
}
