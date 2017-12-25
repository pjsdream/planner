#ifndef PLANNER_PLANNER_PLANNER_WINDOW_H_
#define PLANNER_PLANNER_PLANNER_WINDOW_H_

#include "robot/robot_model.h"
#include "renderer/window/renderer_window.h"
#include "renderer/scene/scene.h"
#include "planner/rendering/robot_scene.h"
#include "planner/planner.h"
#include "planner/environment/environment.h"
#include "planner/motion/robot_motion.h"

namespace planner
{
class PlannerWindow : public renderer::RendererWindow
{
public:
  PlannerWindow();

  void SetRobot(const std::shared_ptr<robot::RobotModel>& robot_model, const std::shared_ptr<RobotMotion>& motion);
  void SetEnvironment(const std::shared_ptr<Environment>& environment);

protected:
  void Idle() override;

private:
  std::shared_ptr<renderer::Scene> scene_;

  std::shared_ptr<Planner> planner_;

  std::shared_ptr<robot::RobotModel> robot_model_;
  std::vector<std::shared_ptr<RobotScene>> robot_scenes_;

  std::shared_ptr<Environment> environment_;
};
}

#endif // PLANNER_PLANNER_PLANNER_WINDOW_H_
