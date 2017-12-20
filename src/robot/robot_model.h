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
  explicit RobotModel(const std::string& name, const std::shared_ptr<RobotLink> base_link)
      : name_(name), base_link_(base_link)
  {
  }

  auto GetBaseLink()
  {
    return base_link_;
  }

private:
  std::string name_;

  std::shared_ptr<RobotLink> base_link_;
};
}

#endif // PLANNER_ROBOT_ROBOT_MODEL_H_
