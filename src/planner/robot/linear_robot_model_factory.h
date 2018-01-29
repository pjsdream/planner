#ifndef PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_MODEL_FACTORY_H_
#define PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_MODEL_FACTORY_H_

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "robot/robot_model.h"
#include "planner/robot/linear_robot_model.h"

namespace planner
{
class LinearRobotModelFactory
{
public:
  LinearRobotModelFactory(const std::shared_ptr<robot::RobotModel>& robot_model);

  void AddActiveJoint(const std::string& joint_name)
  {
    joint_names_.insert(joint_name);
  }

  void SetJointValue(const std::string& joint_name, double joint_value)
  {
    joint_values_[joint_name] = joint_value;
  }

  void SetEndeffectorLink(const std::string& link_name)
  {
    endeffector_link_name_ = link_name;
  }

  std::shared_ptr<LinearRobotModel> CreateRobotModel();

private:
  void CreateRobotModel(const std::shared_ptr<robot::RobotLink>& link, int link_index, Eigen::Affine3d transform);

  std::shared_ptr<robot::RobotModel> robot_model_;

  std::unordered_set<std::string> joint_names_;
  std::unordered_map<std::string, double> joint_values_;
  std::string endeffector_link_name_;
  Eigen::Affine3d endeffector_transform_;

  std::vector<LinearRobotLink> links_;
  std::vector<LinearRobotJoint> joints_;
};
}

#endif // PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_MODEL_FACTORY_H_
