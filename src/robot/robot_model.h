#ifndef PLANNER_ROBOT_ROBOT_MODEL_H_
#define PLANNER_ROBOT_ROBOT_MODEL_H_

#include <unordered_map>
#include <memory>

#include "robot/robot_joint.h"
#include "robot/robot_link.h"

namespace robot
{
class RobotModel
{
public:
  RobotModel() = delete;
  explicit RobotModel(const std::string& name, const std::shared_ptr<RobotLink> base_link);

  auto GetBaseLink()
  {
    return base_link_;
  }

  auto GetLink(const std::string& link_name)
  {
    return links_[link_name];
  }

  auto GetJoint(const std::string& joint_name)
  {
    return joints_[joint_name];
  }

private:
  std::string name_;

  std::shared_ptr<RobotLink> base_link_;

  std::unordered_map<std::string, std::shared_ptr<RobotLink>> links_;
  std::unordered_map<std::string, std::shared_ptr<RobotJoint>> joints_;

  void InitializeTraverse(const std::shared_ptr<RobotLink>& link);
};
}

#endif // PLANNER_ROBOT_ROBOT_MODEL_H_
