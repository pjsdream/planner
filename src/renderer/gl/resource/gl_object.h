#ifndef PLANNER_RENDERER_GL_RESOURCE_GL_OJBECT_H_
#define PLANNER_RENDERER_GL_RESOURCE_GL_OJBECT_H_

#include <vector>
#include <future>

#include <glad/glad.h>
#include <Eigen/Dense>

#include <assimp/scene.h>

#include "renderer/gl/resource/gl_texture.h"

namespace renderer
{
class GlObject
{
public:
  enum class LoadingStatus
  {
    UNINITIALIZED,
    LOADING,
    READY,
  };

public:
  GlObject();
  ~GlObject();

  // Mesh load
  void LoadMeshFromFile(const std::string& filename);
  bool HasTexture();

  auto GetTexture() const
  {
    return texture_;
  }

  // Primitive shapes
  void CreateCube();
  void CreateSphere();
  void CreateCylinder();
  void CreateCone();

  void Draw();

private:
  LoadingStatus loading_status_;

  GLuint vao_;
  std::vector<GLuint> vbos_;
  bool has_elements_;
  GLsizei num_vertices_;
  GLenum draw_mode_;

  // Texture
  std::shared_ptr<GlTexture> texture_;

  // Mesh load
  std::future<void> future_mesh_;
  void ContinueLoadingMesh();

  std::vector<float> vertex_buffer_;
  std::vector<float> normal_buffer_;
  std::vector<float> texture_buffer_;
  std::vector<int> element_buffer_;
};
}

#endif // PLANNER_RENDERER_GL_RESOURCE_GL_OJBECT_H_
