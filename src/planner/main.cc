#include "robot/robot_model_loader.h"
#include "renderer/window/renderer_context.h"
#include "planner/planner_window.h"
#include "planner/environment/environment_loader.h"

int main(int argc, char** argv)
{
  renderer::RendererContext::Init(argc, argv);

  planner::PlannerWindow window;

  // Robot
  robot::RobotModelLoader robot_model_loader;
  //robot_model_loader.AddPackageDirectory("/home/jaesungp/catkin_ws/src/fetch_ros");
  robot_model_loader.AddPackageDirectory("C:/Users/pjsdr_000/Desktop/documents/fetch_ros");
  auto robot_model =
	  //robot_model_loader.LoadFromUrdfFile("/home/jaesungp/catkin_ws/src/fetch_ros/fetch_description/robots/fetch.urdf");
      robot_model_loader.LoadFromUrdfFile("C:/Users/pjsdr_000/Desktop/documents/fetch_ros/fetch_description/robots/fetch.urdf");

  // Environment
  planner::EnvironmentLoader environment_loader;
  auto environment =
      //environment_loader.LoadEnvironmentFromFile("/home/jaesungp/cpp_workspace/planner/config/environment.json");
      environment_loader.LoadEnvironmentFromFile("C:/Users/pjsdr_000/Desktop/documents/planner/config/environment.json");

  window.SetRobotModel(robot_model);
  window.SetEnvironment(environment);

  renderer::RendererContext::PrintVersion();
  renderer::RendererContext::Start();
  renderer::RendererContext::Finish();

  return 0;
}
