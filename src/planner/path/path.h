#ifndef PLANNER_PLANNER_PATH_PATH_H_
#define PLANNER_PLANNER_PATH_PATH_H_

#include <vector>
#include <string>

#include <Eigen/Dense>

namespace planner
{
class Path
{
public:
  Path() = delete;
  Path(const std::vector<std::string>& joint_names);

  void SetInitialPoint(const Eigen::VectorXd& point);

private:
  std::vector<std::string> joint_names_;
  Eigen::MatrixXd points_;
};
}

#endif // PLANNER_PLANNER_PATH_PATH_H_
