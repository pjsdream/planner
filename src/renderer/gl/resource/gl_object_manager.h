#ifndef PLANNER_RENDERER_GL_RESOURCE_GL_OBJECT_MANAGER_H_
#define PLANNER_RENDERER_GL_RESOURCE_GL_OBJECT_MANAGER_H_

#include <string>
#include <unordered_map>

#include "renderer/gl/resource/gl_object.h"

namespace renderer
{
class GlObjectManager
{
public:
  GlObjectManager() = default;

  void RegisterObject(const std::string& name, const std::shared_ptr<GlObject>& object)
  {
    objects_[name] = object;
  }

  std::shared_ptr<GlObject> GetObject(const std::string& name)
  {
    const auto it = objects_.find(name);
    if (it != objects_.cend())
      return it->second;
    return nullptr;
  }

  std::shared_ptr<GlObject> GetObjectOrCreateFromFile(const std::string& name, const std::string& filename)
  {
    const auto it = objects_.find(name);
    if (it != objects_.cend())
      return it->second;

    auto object = std::make_shared<GlObject>();
    object->LoadMeshFromFile(filename);
    RegisterObject(name, object);
    return object;
  }

private:
  std::unordered_map<std::string, std::shared_ptr<GlObject>> objects_;
};
}

#endif // PLANNER_RENDERER_GL_RESOURCE_GL_OBJECT_MANAGER_H_
