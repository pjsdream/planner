#ifndef PLANNER_PLANNER_PLANNER_H_
#define PLANNER_PLANNER_PLANNER_H_

#include <memory>

#include "planner/robot/linear_robot_model.h"
#include "planner/environment/environment.h"
#include "planner/path/path.h"
#include "planner/config/config.h"

namespace planner
{
class Planner
{
public:
  Planner() = default;

  void SetConfig(const std::shared_ptr<Config>& config);
  void SetInitialRobotState(const Eigen::VectorXd& state);
  void SetInitialZeroRobotState();

  auto GetEnvironment() const
  {
    return environment_;
  }

  auto GetRobotModel() const
  {
    return robot_model_;
  }

private:
  std::shared_ptr<LinearRobotModel> robot_model_;
  std::shared_ptr<Environment> environment_;

  std::shared_ptr<Path> path_;
};
}

#endif // PLANNER_PLANNER_H_
