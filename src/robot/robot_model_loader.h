#ifndef PLANNER_ROBOT_ROBOT_MODEL_LOADER_H_
#define PLANNER_ROBOT_ROBOT_MODEL_LOADER_H_

#include <memory>

#include "robot/robot_model.h"

namespace robot
{
class RobotModelLoader
{
public:
  void AddPackageDirectory(const std::string& package_directory)
  {
    package_directories_.push_back(package_directory);
  }

  std::shared_ptr<RobotModel> LoadFromUrdfFile(const std::string& filename);

private:
  std::vector<std::string> package_directories_;

  std::string SubstitutePackageDirectory(const std::string& filename);

  std::unordered_map<std::string, std::shared_ptr<RobotLink>> links_;
  std::unordered_map<std::string, std::shared_ptr<RobotJoint>> joints_;
  std::unordered_map<std::string, std::vector<std::string>> link_child_joint_names_;
  std::unordered_map<std::string, std::string> joint_child_link_name_;

  void ConstructTree(const std::shared_ptr<RobotLink>& link);
};
}

#endif // PLANNER_ROBOT_ROBOT_MODEL_LOADER_H_
