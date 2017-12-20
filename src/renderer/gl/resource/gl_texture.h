#ifndef PLANNER_RENDERER_GL_RESOURCE_GL_TEXTURE_H_
#define PLANNER_RENDERER_GL_RESOURCE_GL_TEXTURE_H_

#include <future>
#include <vector>

#include <GL/gl.h>

namespace renderer
{
class GlTexture
{
private:
  enum class LoadingStatus
  {
    UNINITIALIZED,
    LOADING,
    READY,
  };

public:
  GlTexture();
  ~GlTexture();

  // Load texture from file
  void LoadTextureFromFile(const std::string& filename);
  bool IsReady();
  bool Prepare();

  void Bind();

private:
  // GL object
  GLuint texture_id_;

  // Loading
  LoadingStatus loading_status_;
  std::future<void> future_texture_;

  unsigned int width_;
  unsigned int height_;
  std::vector<unsigned char> image_;
};
}

#endif // PLANNER_RENDERER_GL_RESOURCE_GL_TEXTURE_H_
