#ifndef PLANNER_RENDERER_GL_RESOURCE_GL_INSTANCES_H_
#define PLANNER_RENDERER_GL_RESOURCE_GL_INSTANCES_H_

#include <memory>

#include "renderer/gl/resource/gl_object.h"

namespace renderer
{
class GlInstances
{
public:
  struct Instance
  {
    bool has_global_color;
    Eigen::Vector3f global_color;
    Eigen::Affine3d transform;
  };

public:
  GlInstances() = default;

  void SetObject(const std::shared_ptr<GlObject>& object)
  {
    object_ = object;
  }

  std::shared_ptr<GlObject> GetObject()
  {
    return object_;
  }

  void AddInstance(const Instance& instance)
  {
    instances_.push_back(instance);
  }

  const std::vector<Instance>& GetInstances() const
  {
    return instances_;
  }

  void ClearInstances()
  {
    instances_.clear();
  }

private:
  std::shared_ptr<GlObject> object_;

  std::vector<Instance> instances_;
};
}

#endif // PLANNER_RENDERER_GL_RESOURCE_GL_INSTANCES_H_
