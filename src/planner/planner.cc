#include "planner/planner.h"

namespace planner
{
void Planner::SetRobot(const std::shared_ptr<robot::RobotModel>& robot_model,
                       const std::shared_ptr<RobotMotion>& motion)
{
  robot_model_ = robot_model;
  motion_ = motion;

  path_ = std::make_shared<Path>(motion->GetBodyJoints());
}

void Planner::UpdateEnvironment(const std::shared_ptr<Environment>& environment)
{
  environment_ = environment;

  // TODO: get obstacles from the environment
}
}
