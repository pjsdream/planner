#include "rendering/object/texture_object.h"

namespace simplan
{
TextureObject::TextureObject()
{
  glGenTextures(1, &id_);
}

TextureObject::~TextureObject()
{
  glDeleteTextures(1, &id_);
}

void TextureObject::SetTexture(const std::shared_ptr<Texture>& texture)
{
  glBindTexture(GL_TEXTURE_2D, id_);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               texture->width,
               texture->height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               (void*)texture->image.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
}
}
