#ifndef SIMPLAN_RESOURCE_TEXTURE_MANAGER_H_
#define SIMPLAN_RESOURCE_TEXTURE_MANAGER_H_

#include <memory>

#include "resource/texture.h"

namespace simplan
{
class TextureManager
{
public:
  static std::shared_ptr<Texture> LoadFromFile(const std::string& filename);
};
}

#endif // SIMPLAN_RESOURCE_TEXTURE_MANAGER_H_
