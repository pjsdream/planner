#include "robot/robot_model_loader.h"
#include "renderer/window/renderer_context.h"
#include "planner/planner_window.h"
#include "planner/environment/environment_loader.h"
#include "planner/motion/robot_motion_loader.h"

int main(int argc, char** argv)
{
  renderer::RendererContext::Init(argc, argv);

  planner::PlannerWindow window;

  // Robot
  robot::RobotModelLoader robot_model_loader;
  robot_model_loader.AddPackageDirectory("/home/jaesungp/catkin_ws/src/fetch_ros");
  auto robot_model =
      robot_model_loader.LoadFromUrdfFile("/home/jaesungp/catkin_ws/src/fetch_ros/fetch_description/robots/fetch.urdf");

  // Environment
  planner::EnvironmentLoader environment_loader;
  auto environment =
      environment_loader.LoadEnvironmentFromFile("/home/jaesungp/cpp_workspace/planner/config/environment.json");

  // Motion
  planner::RobotMotionLoader robot_motion_loader;
  auto robot_motion =
      robot_motion_loader.LoadRobotMotionFromFile("/home/jaesungp/cpp_workspace/planner/config/motion.json");

  window.SetRobot(robot_model, robot_motion);
  window.SetEnvironment(environment);

  renderer::RendererContext::PrintVersion();
  renderer::RendererContext::Start();
  renderer::RendererContext::Finish();

  return 0;
}
