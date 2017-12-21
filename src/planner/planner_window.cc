#include "planner/planner_window.h"

#include <iostream>

#include "robot/robot_model_loader.h"
#include "shape/cube.h"

namespace planner
{
PlannerWindow::PlannerWindow()
    : renderer::RendererWindow()
{
  ChangeWindowTitle("planner");

  // Create scene
  scene_ = std::make_shared<renderer::Scene>();

  // Create planner
  planner_ = std::make_shared<Planner>();
}

void PlannerWindow::SetRobotModel(const std::shared_ptr<robot::RobotModel>& robot_model)
{
  robot_model_ = robot_model;
  planner_->SetRobotModel(robot_model);

  // Create robot scene
  auto rand01 = []()
  { return rand() / 2147483647.; };
  auto rand = [rand01](double low, double high)
  { return low + rand01() * (high - low); };
  auto rand_range = [rand01](double low, double range)
  { return low + rand01() * range; };

  for (int i = 0; i < 100; i++)
  {
    auto robot_scene = std::make_shared<RobotScene>(robot_model_, scene_->GetRootNode());

    robot_scene->SetJointValue("torso_lift_joint", rand(0.2, 0.23));
    robot_scene->SetJointValue("shoulder_pan_joint", rand_range(1, 1.57 / 10.));
    robot_scene->SetJointValue("shoulder_lift_joint", rand_range(1, 1.57 / 10.));
    robot_scene->SetJointValue("upperarm_roll_joint", rand_range(-1, 1.57 / 10.));
    robot_scene->SetJointValue("elbow_flex_joint", rand_range(-1, 1.57 / 10.));
    robot_scene->SetJointValue("forearm_roll_joint", rand_range(1, 1.57 / 10.));
    robot_scene->SetJointValue("wrist_flex_joint", rand_range(-1, 1.57 / 10.));
    robot_scene->SetJointValue("wrist_roll_joint", rand_range(-1, 1.57 / 10.));

    robot_scenes_.push_back(robot_scene);
  }
}

void PlannerWindow::SetEnvironment(const std::shared_ptr<Environment>& environment)
{
  environment_ = environment;
  planner_->UpdateEnvironment(environment);

  // Create environment scene
  auto scene_node = scene_->GetRootNode();
  for (auto object : environment->GetObjects())
  {
    auto object_node = scene_node->CreateChild();
    object_node->SetTransform(object->GetTransform());

    auto shape = object->GetShape();
    if (shape->IsCube())
    {
      auto cube = std::static_pointer_cast<shape::Cube>(shape);
      auto scene_object = std::make_shared<renderer::SceneObject>("cube");
      scene_object->SetGlobalColor(object->GetColor().block(0, 0, 3, 1));
      object_node->ApplyTransform(cube->GetTransform());
      object_node->Scale(cube->GetSize() / 2.);
      object_node->AttachObject(scene_object);
    }
  }

  UpdateScene(scene_);
}

void PlannerWindow::Idle()
{
  UpdateScene(scene_);
}
}
