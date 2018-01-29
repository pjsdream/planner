#include "planner/planner_window.h"

#include <iostream>

#include "robot/robot_model_loader.h"
#include "shape/cube.h"
#include "planner/rendering/linear_robot_scene.h"
#include "cost/cost_grip_pose.h"

namespace planner
{
namespace
{
double M_PI = 3.1415926535897932384626433832795;
}

PlannerWindow::PlannerWindow()
  : renderer::RendererWindow()
{
  ChangeWindowTitle("planner");

  // Create scene
  scene_ = std::make_shared<renderer::Scene>();

  // Create planner
  planner_ = std::make_shared<Planner>();
}

void PlannerWindow::SetConfig(const std::shared_ptr<Config>& config)
{
  planner_->SetConfig(config);

  // Create environment scene
  auto scene_node = scene_->GetRootNode();
  auto environment = planner_->GetEnvironment();
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

  // Debug: robot model
  auto rand01 = []()
  { return static_cast<double>(rand()) / RAND_MAX; };
  auto rand = [rand01](double low, double high)
  { return low + rand01() * (high - low); };
  auto rand_range = [rand01](double low, double range)
  { return low + rand01() * range; };

  auto robot_model = planner_->GetRobotModel();

  for (auto object : environment->GetObjects())
  //auto object = environment->GetEnvironmentObject("magenta block");
  {
    if (object->GetName() == "table")
      continue;

    auto robot_state = std::make_shared<RobotState>(robot_model);

    for (int i = 0; i < robot_model->GetJoints().size(); i++)
      robot_state->SetJointValue(i, rand(robot_model->GetJoint(i).GetLowerLimit(), robot_model->GetJoint(i).GetUpperLimit()));

    Eigen::Affine3d target = object->GetTransform();
    target.translate(Eigen::Vector3d(0., 0., 0.1));
    robot_state->MoveToGripPose(target);

    auto robot_scene = std::make_shared<LinearRobotScene>(robot_model, scene_->GetRootNode());
    for (int i = 0; i < robot_model->GetJoints().size(); i++)
      robot_scene->SetJointValue(i, robot_state->GetJointValue(i));
  }

  /*
  auto object = environment_->GetEnvironmentObject("red block");
  CostGripPose cost(1., CostGripPose::GripType::CYLINDER, object->GetTransform(), 1.);

  std::vector<std::pair<double, std::shared_ptr<RobotState>>> states;

  for (int i = 0; i < 100000; i++)
  {
    auto robot_state = std::make_shared<RobotState>(robot_model_, motion_);

    for (const auto& joint_name : motion_->GetBodyJoints())
    {
      auto joint = robot_model_->GetJoint(joint_name);
      double joint_value = 0.;
      if (joint->IsContinuous())
        joint_value = rand(-M_PI, M_PI);
      else
        joint_value = rand(joint->GetLowerLimit(), joint->GetUpperLimit());
      robot_state->SetJointValue(joint_name, joint_value);
    }

    robot_state->PerformForwardKinematics();
    cost.SetRobotState(robot_state);
    states.push_back(std::make_pair(cost.GetCost(), robot_state));
  }

  std::sort(states.begin(), states.end());

  for (int i = 0; i < 1; i++)
  {
    std::cout << "cost " << i << ": " << states[i].first << "\n";

    auto robot_scene = std::make_shared<RobotScene>(robot_model_, scene_->GetRootNode());
    auto robot_state = states[i].second;

    robot_state->MoveToGripPose(object->GetTransform());

    for (const auto& joint_name : motion_->GetBodyJoints())
    {
      auto joint_value = robot_state->GetJointValue(joint_name);
      robot_scene->SetJointValue(joint_name, joint_value);
    }

    robot_scenes_.push_back(robot_scene);
  }
  */

  UpdateScene(scene_);
}

void PlannerWindow::Idle()
{
  UpdateScene(scene_);
}
}
