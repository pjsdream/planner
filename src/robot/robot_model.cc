#include "robot/robot_model.h"

namespace robot
{
RobotModel::RobotModel(const std::string& name, const std::shared_ptr<RobotLink> base_link)
    : name_(name), base_link_(base_link)
{
  InitializeTraverse(base_link);
}

void RobotModel::InitializeTraverse(const std::shared_ptr<RobotLink>& link)
{
  links_[link->GetName()] = link;

  for (const auto& child_joint : link->GetChildJoints())
  {
    joints_[child_joint->GetName()] = child_joint;

    auto child_link = child_joint->GetChildLink();
    InitializeTraverse(child_link);
  }
}
}
