#ifndef PLANNER_PLANNER_PLANNER_H_
#define PLANNER_PLANNER_PLANNER_H_

#include "robot/robot_model.h"
#include "planner/environment/environment.h"

namespace planner
{
class Planner
{
public:
  Planner() = default;

  void SetRobotModel(const std::shared_ptr<robot::RobotModel>& robot_model);

  void UpdateEnvironment(const std::shared_ptr<Environment>& environment);

private:
  std::shared_ptr<robot::RobotModel> robot_model_;
  std::shared_ptr<Environment> environment_;
};
}

#endif // PLANNER_PLANNER_H_
