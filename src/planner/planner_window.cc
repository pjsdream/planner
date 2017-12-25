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

void PlannerWindow::SetRobot(const std::shared_ptr<robot::RobotModel>& robot_model,
                             const std::shared_ptr<RobotMotion>& motion)
{
  robot_model_ = robot_model;
  planner_->SetRobot(robot_model, motion);

  // Debug: create robot scene
  auto rand01 = []()
  { return rand() / 2147483647.; };
  auto rand = [rand01](double low, double high)
  { return low + rand01() * (high - low); };
  auto rand_range = [rand01](double low, double range)
  { return low + rand01() * range; };

  for (int i = 0; i < 10; i++)
  {
    auto robot_scene = std::make_shared<RobotScene>(robot_model_, scene_->GetRootNode());
    for (const auto& joint_name : motion->GetBodyJoints())
    {
      auto joint = robot_model->GetJoint(joint_name);
      robot_scene->SetJointValue(joint_name, rand(joint->GetLowerLimit(), joint->GetUpperLimit()));
    }

    robot_scenes_.push_back(robot_scene);
  }

  UpdateScene(scene_);
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
