#ifndef PLANNER_MOTION_LOADER_H
#define PLANNER_MOTION_LOADER_H

#include <memory>

#include "planner/motion/robot_motion.h"

namespace planner
{
class RobotMotionLoader
{
public:
  RobotMotionLoader() = default;

  std::shared_ptr<RobotMotion> LoadRobotMotionFromFile(const std::string& filename);

private:
};
}

#endif //PLANNER_MOTION_LOADER_H
