#ifndef PLANNER_PLANNER_ENVIRONMENT_ENVIRONMENT_LOADER_H_
#define PLANNER_PLANNER_ENVIRONMENT_ENVIRONMENT_LOADER_H_

#include <memory>

#include "planner/environment/environment.h"

namespace planner
{
class EnvironmentLoader
{
public:
  EnvironmentLoader() = default;

  std::shared_ptr<Environment> LoadEnvironmentFromFile(const std::string& filename);

private:
};
}

#endif // PLANNER_PLANNER_ENVIRONMENT_ENVIRONMENT_LOADER_H_
