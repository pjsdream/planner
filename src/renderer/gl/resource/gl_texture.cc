#include "renderer/gl/resource/gl_texture.h"

#include <lodepng/lodepng.h>

namespace renderer
{
GlTexture::GlTexture()
    : loading_status_(LoadingStatus::UNINITIALIZED), texture_id_(0)
{
}

GlTexture::~GlTexture()
{
  if (texture_id_ != 0)
    glDeleteTextures(1, &texture_id_);
}

void GlTexture::Bind()
{
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void GlTexture::LoadTextureFromFile(const std::string& filename)
{
  loading_status_ = LoadingStatus::LOADING;

  future_texture_ = std::async(
      std::launch::async,
      [&, filename]()
      {
        lodepng::decode(image_, width_, height_, filename.c_str());
      }
  );
}

bool GlTexture::IsReady()
{
  return loading_status_ == LoadingStatus::READY;
}

bool GlTexture::Prepare()
{
  using namespace std::chrono_literals;

  if (loading_status_ == LoadingStatus::LOADING &&
      future_texture_.valid() && future_texture_.wait_for(0s) == std::future_status::ready)
  {
    future_texture_.get();

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 width_,
                 height_,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &image_[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    loading_status_ = LoadingStatus::READY;

    return true;
  }

  return false;
}
}
