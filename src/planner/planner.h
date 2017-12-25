#ifndef PLANNER_PLANNER_PLANNER_H_
#define PLANNER_PLANNER_PLANNER_H_

#include "robot/robot_model.h"
#include "planner/environment/environment.h"
#include "planner/motion/robot_motion.h"
#include "planner/path/path.h"

namespace planner
{
class Planner
{
public:
  Planner() = default;

  void SetRobot(const std::shared_ptr<robot::RobotModel>& robot_model, const std::shared_ptr<RobotMotion>& motion);

  void UpdateEnvironment(const std::shared_ptr<Environment>& environment);

private:
  std::shared_ptr<robot::RobotModel> robot_model_;
  std::shared_ptr<Environment> environment_;
  std::shared_ptr<RobotMotion> motion_;

  std::shared_ptr<Path> path_;
};
}

#endif // PLANNER_PLANNER_H_
