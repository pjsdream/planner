#include "robot/robot_model_loader.h"
#include "renderer/window/renderer_context.h"
#include "planner/planner_window.h"
#include "planner/environment/environment_loader.h"
#include "planner/config/config_loader.h"

int main(int argc, char** argv)
{
  renderer::RendererContext::Init(argc, argv);

  planner::PlannerWindow window;

  // Config
  planner::ConfigLoader config_loader;
  auto config = config_loader.LoadConfigFromFile("C:/Users/pjsdr_000/Desktop/documents/planner/config/planning_config.json");

  window.SetConfig(config);

  renderer::RendererContext::PrintVersion();
  renderer::RendererContext::Start();
  renderer::RendererContext::Finish();

  return 0;
}
