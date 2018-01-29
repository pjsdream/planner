#include "planner/rendering/robot_scene.h"

namespace planner
{
RobotScene::RobotScene(const std::shared_ptr<robot::RobotModel>& robot_model,
                       const std::shared_ptr<renderer::SceneNode>& scene_node)
    : scene_node_(scene_node)
{
  AddRobotScene(scene_node, robot_model->GetBaseLink());
}

void RobotScene::AddRobotScene(const std::shared_ptr<renderer::SceneNode>& scene_node,
                               const std::shared_ptr<robot::RobotLink>& robot_link)
{
  int counter = 0;
  for (const auto& visual : robot_link->GetVisuals())
  {
    // Prepare mesh
    scene_node->LoadMeshObject(robot_link->GetName() + "[" + std::to_string(counter) + "]", visual.mesh_filename);

    // Create a scene node for transform
    auto transformed_node = scene_node->CreateChild();
    transformed_node->SetTransform(visual.origin);

    // Attach the scene object
    auto object = std::make_shared<renderer::SceneObject>(robot_link->GetName() + "[" + std::to_string(counter) + "]");
    //object->SetGlobalColor(visual.color.block(0, 0, 3, 1));
    transformed_node->AttachObject(object);

    counter++;
  }

  for (const auto& child_joint : robot_link->GetChildJoints())
  {
    const auto& joint_name = child_joint->GetName();
    const Eigen::Affine3d child_joint_transform = child_joint->Transform(0.);

    auto child_scene_node = scene_node->CreateChild();
    child_scene_node->SetTransform(child_joint_transform);

    // Cache joint name and the scene node pairs
    joint_name_to_joint_[joint_name] = child_joint;
    joint_name_to_scene_[joint_name] = child_scene_node;

    // Traverse to child link
    AddRobotScene(child_scene_node, child_joint->GetChildLink());
  }
}

void RobotScene::SetJointValue(const std::string& joint, double value)
{
  joint_name_to_scene_[joint]->SetTransform(joint_name_to_joint_[joint]->Transform(value));
}
}
