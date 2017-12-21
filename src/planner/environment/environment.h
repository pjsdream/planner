#ifndef PLANNER_PLANNER_ENVIRONMENT_ENVIRONMENT_H_
#define PLANNER_PLANNER_ENVIRONMENT_ENVIRONMENT_H_

#include "planner/environment/object.h"

namespace planner
{
class Environment
{
public:
  Environment() = default;

  void AddObject(const std::shared_ptr<Object>& object)
  {
    objects_.push_back(object);
  }

  const auto& GetObjects() const
  {
    return objects_;
  }

private:
  std::vector<std::shared_ptr<Object>> objects_;
};
}

#endif // PLANNER_ENVIRONMENT_ENVIRONMENT_H_
