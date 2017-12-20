#ifndef PLANNER_PLANNER_RENDERING_ROBOT_SCENE_H_
#define PLANNER_PLANNER_RENDERING_ROBOT_SCENE_H_

#include "robot/robot_model.h"
#include "renderer/scene/scene_node.h"

namespace planner
{
class RobotScene
{
public:
  RobotScene() = delete;
  RobotScene(const std::shared_ptr<robot::RobotModel>& robot_model,
             const std::shared_ptr<renderer::SceneNode>& scene_node);

  void SetJointValue(const std::string& joint, double value);

private:
  void AddRobotScene(const std::shared_ptr<renderer::SceneNode>& scene_node,
                     const std::shared_ptr<robot::RobotLink>& robot_link);

  std::shared_ptr<renderer::SceneNode> scene_node_;

  std::unordered_map<std::string, std::shared_ptr<renderer::SceneNode>> joint_name_to_scene_;
  std::unordered_map<std::string, std::shared_ptr<robot::RobotJoint>> joint_name_to_joint_;
};
}

#endif // PLANNER_RENDERING_ROBOT_SCENE_H_
