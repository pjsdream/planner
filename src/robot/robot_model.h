#ifndef SIMPLAN_ROBOT_ROBOT_MODEL_H_
#define SIMPLAN_ROBOT_ROBOT_MODEL_H_

#include <string>

namespace simplan
{
class RobotModel
{
public:
  RobotModel();

  void SetName(const std::string& name)
  {
    name_ = name;
  }

  const auto& GetName() const
  {
    return name_;
  }

private:
  std::string name_;
};
}

#endif // SIMPLAN_ROBOT_ROBOT_MODEL_H_
