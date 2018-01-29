#ifndef PLANNER_PLANNER_COST_COST_GRIP_POSE_H_
#define PLANNER_PLANNER_COST_COST_GRIP_POSE_H_

#include "planner/cost/cost.h"

#include <Eigen/Dense>

namespace planner
{
class CostGripPose : public Cost
{
public:
  enum class GripType
  {
    CUBE,
    BOOK,
    CYLINDER,
  };

public:
  CostGripPose() = delete;

  CostGripPose(double weight, GripType type, const Eigen::Affine3d& pose, double angular_weight)
    : Cost(weight), type_(type), pose_(pose), angular_weight_(angular_weight)
  {
  }

  virtual ~CostGripPose() = default;

  /// Precondition: Forward kinematics has been performed on the robot state.
  double GetCost() const override;

private:
  GripType type_;

  Eigen::Affine3d pose_;
  double angular_weight_;
};
}

#endif // PLANNER_PLANNER_COST_COST_GRIP_POSE_H_
