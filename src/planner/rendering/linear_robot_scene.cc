#include "planner/rendering/linear_robot_scene.h"

namespace planner
{
LinearRobotScene::LinearRobotScene(const std::shared_ptr<LinearRobotModel>& robot_model,
                                   const std::shared_ptr<renderer::SceneNode>& scene_node)
  : robot_model_(robot_model)
{
  const auto& links = robot_model->GetLinks();
  const auto& joints = robot_model->GetJoints();

  scene_nodes_.push_back(scene_node);

  for (int i = 0; i < links.size(); i++)
  {
    const auto& link = links[i];
    auto node = scene_nodes_[i];

    int j = 0;
    for (const auto& visual : link.GetVisuals())
    {
      // Prepare mesh
      const std::string object_name = "link[" + std::to_string(i) + "].visual[" + std::to_string(j) + "]";
      node->LoadMeshObject(object_name, visual.filename);

      // Create a scene node for transform
      auto transformed_node = node->CreateChild();
      transformed_node->SetTransform(visual.origin);

      // Attach the scene object
      auto object = std::make_shared<renderer::SceneObject>(object_name);
      transformed_node->AttachObject(object);

      j++;
    }

    if (i < links.size() - 1)
    {
      auto next_node = node->CreateChild();
      next_node->SetTransform(joints[i].Transform(0.));
      scene_nodes_.push_back(next_node);
    }
  }
}

void LinearRobotScene::SetJointValue(int index, double value)
{
  auto joint = robot_model_->GetJoints()[index];
  scene_nodes_[index + 1]->SetTransform(joint.Transform(value));
}
}
