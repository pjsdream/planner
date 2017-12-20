#ifndef PLANNER_PLANNER_PLANNER_WINDOW_H_
#define PLANNER_PLANNER_PLANNER_WINDOW_H_

#include "robot/robot_model.h"
#include "renderer/window/renderer_window.h"
#include "renderer/scene/scene.h"
#include "planner/rendering/robot_scene.h"

namespace planner
{
class PlannerWindow : public renderer::RendererWindow
{
public:
  PlannerWindow();

  void SetRobotModel(const std::shared_ptr<robot::RobotModel>& robot_model);

protected:
  void Idle() override;

private:
  std::shared_ptr<renderer::Scene> scene_;

  std::shared_ptr<robot::RobotModel> robot_model_;
  std::shared_ptr<RobotScene> robot_scene_;
};
}

#endif // PLANNER_PLANNER_PLANNER_WINDOW_H_
