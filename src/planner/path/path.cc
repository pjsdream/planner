#include "planner/path/path.h"

namespace planner
{
Path::Path(const std::vector<std::string>& joint_names)
    : joint_names_(joint_names)
{
  points_.resize(joint_names.size(), 1);
  points_.setZero();
}

void Path::SetInitialPoint(const Eigen::VectorXd& point)
{
  points_.col(0) = point;
}
}
