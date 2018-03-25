#ifndef SIMPLAN_RENDERING_OBJECT_TEXTURE_OBJECT_H_
#define SIMPLAN_RENDERING_OBJECT_TEXTURE_OBJECT_H_

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "resource/texture.h"

namespace simplan
{
class TextureObject
{
public:
  TextureObject();
  ~TextureObject();

  auto GetId() const
  {
    return id_;
  }

  void SetTexture(const std::shared_ptr<Texture>& texture);

private:
  GLuint id_;
};
}

#endif // SIMPLAN_RENDERING_OBJECT_TEXTURE_OBJECT_H_
