#ifndef PLANNER_PLANNER_CONFIG_CONFIG_LOADER_H_
#define PLANNER_PLANNER_CONFIG_CONFIG_LOADER_H_

#include <memory>

#include "planner/config/config.h"

namespace planner
{
class ConfigLoader
{
public:
  ConfigLoader() = default;

  std::shared_ptr<Config> LoadConfigFromFile(const std::string& filename);

private:
};
}

#endif //PLANNER_PLANNER_ROBOT_MOTION_LOADER_H_
