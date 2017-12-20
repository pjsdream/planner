#ifndef PLANNER_RENDERER_SCENE_SCENE_H_
#define PLANNER_RENDERER_SCENE_SCENE_H_

#include <vector>
#include <memory>
#include <unordered_map>

#include "renderer/scene/scene_node.h"
#include "renderer/light/light.h"

namespace renderer
{
class Scene
{
public:
  Scene();

  auto GetRootNode()
  {
    return root_;
  }

  auto GetLight(int index)
  {
    return lights_[index];
  }

private:
  // Lights
  std::vector<std::shared_ptr<Light>> lights_;

  // Root node
  std::shared_ptr<SceneNode> root_;
};
}

#endif // PLANNER_RENDERER_SCENE_SCENE_H_
