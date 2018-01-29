#ifndef PLANNER_PLANNER_COST_COST_H_
#define PLANNER_PLANNER_COST_COST_H_

#include <memory>

#include "planner/robot/robot_state.h"

namespace planner
{
class Cost
{
public:
  Cost() = delete;
  Cost(double weight)
    : weight_(weight)
  {
  }

  virtual ~Cost() = default;

  virtual double GetCost() const
  {
    return 0.;
  }

  void SetRobotState(const std::shared_ptr<RobotState>& state)
  {
    state_ = state;
  }

  auto GetRobotState() const
  {
    return state_;
  }

  auto GetWeight() const
  {
    return weight_;
  }

private:
  double weight_;
  std::shared_ptr<RobotState> state_;
};
}

#endif // PLANNER_PLANNER_COST_COST_H_
