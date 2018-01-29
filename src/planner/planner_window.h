#ifndef PLANNER_PLANNER_PLANNER_WINDOW_H_
#define PLANNER_PLANNER_PLANNER_WINDOW_H_

#include "robot/robot_model.h"
#include "renderer/window/renderer_window.h"
#include "renderer/scene/scene.h"
#include "planner/rendering/robot_scene.h"
#include "planner/planner.h"
#include "planner/environment/environment.h"

namespace planner
{
class PlannerWindow : public renderer::RendererWindow
{
public:
  PlannerWindow();

  void SetConfig(const std::shared_ptr<Config>& config);

protected:
  void Idle() override;

private:
  std::shared_ptr<renderer::Scene> scene_;

  std::shared_ptr<Planner> planner_;
};
}

#endif // PLANNER_PLANNER_PLANNER_WINDOW_H_
