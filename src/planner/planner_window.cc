#include "planner/planner_window.h"

#include <iostream>

#include "robot/robot_model_loader.h"

namespace planner
{
PlannerWindow::PlannerWindow()
    : renderer::RendererWindow()
{
  ChangeWindowTitle("planner");

  // Create scene
  scene_ = std::make_shared<renderer::Scene>();
}

void PlannerWindow::SetRobotModel(const std::shared_ptr<robot::RobotModel>& robot_model)
{
  robot_model_ = robot_model;

  // Create robot scene
  robot_scene_ = std::make_shared<RobotScene>(robot_model_, scene_->GetRootNode());
}

void PlannerWindow::Idle()
{
  robot_scene_->SetJointValue("torso_lift_joint", rand() / 2147483647. * 0.3);

  UpdateScene(scene_);
}
}
