#ifndef SIMPLAN_NLP_ENVIRONMENT_H_
#define SIMPLAN_NLP_ENVIRONMENT_H_

#include <string>
#include <vector>

namespace simplan
{
class Environment
{
public:
  Environment();
  ~Environment();

  void SetHoldObject(bool flag)
  {
    hold_object_ = flag;
  }

  void SetJointValues(const std::vector<double>& joint_values)
  {
    joint_values_ = joint_values;
  }

private:
  bool hold_object_ = false;
  std::vector<double> joint_values_;
};
}

#endif // SIMPLAN_NLP_ENVIRONMENT_H_
