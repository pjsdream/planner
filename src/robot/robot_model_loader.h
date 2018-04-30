#ifndef SIMPLAN_ROBOT_ROBOT_MODEL_LOADER_H_
#define SIMPLAN_ROBOT_ROBOT_MODEL_LOADER_H_

#include <string>
#include <memory>

#include "robot/robot_model.h"

namespace simplan
{
class RobotModelLoader
{
public:
  RobotModelLoader();

  std::shared_ptr<RobotModel> LoadFromFile(const std::string& filename);

private:
};
}

#endif // SIMPLAN_ROBOT_ROBOT_MODEL_LOADER_H_
