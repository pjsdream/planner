#include "robot/robot_model_loader.h"
#include "renderer/window/renderer_context.h"
#include "planner/planner_window.h"

int main(int argc, char** argv)
{
  renderer::RendererContext::Init(argc, argv);

  planner::PlannerWindow window;

  // Load robot
  robot::RobotModelLoader robot_model_loader;
  robot_model_loader.AddPackageDirectory("/home/jaesungp/catkin_ws/src/fetch_ros");
  auto robot_model =
      robot_model_loader.LoadFromUrdfFile("/home/jaesungp/catkin_ws/src/fetch_ros/fetch_description/robots/fetch.urdf");

  window.SetRobotModel(robot_model);

  renderer::RendererContext::Start();
  renderer::RendererContext::Finish();

  return 0;
}
