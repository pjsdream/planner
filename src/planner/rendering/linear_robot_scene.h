#ifndef PLANNER_PLANNER_RENDERING_LINEAR_ROBOT_SCENE_H_
#define PLANNER_PLANNER_RENDERING_LINEAR_ROBOT_SCENE_H_

#include "planner/robot/linear_robot_model.h"
#include "renderer/scene/scene_node.h"

namespace planner
{
class LinearRobotScene
{
public:
  LinearRobotScene() = delete;
  LinearRobotScene(const std::shared_ptr<LinearRobotModel>& robot_model,
                   const std::shared_ptr<renderer::SceneNode>& scene_node);

  void SetJointValue(int index, double value);

private:
  std::shared_ptr<LinearRobotModel> robot_model_;
  std::vector<std::shared_ptr<renderer::SceneNode>> scene_nodes_;
};
}

#endif // PLANNER_PLANNER_RENDERING_LINEAR_ROBOT_SCENE_H_
