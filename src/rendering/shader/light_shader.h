#ifndef SIMPLAN_SHADER_LIGHT_SHADER_H_
#define SIMPLAN_SHADER_LIGHT_SHADER_H_

#include <memory>

#include "rendering/shader/shader.h"
#include "rendering/camera.h"
#include "resource/texture.h"

namespace simplan
{
class LightShader : public Shader
{
public:
  LightShader();
  ~LightShader() override;

  void LoadCamera(const std::shared_ptr<Camera>& camera);
  void LoadModel(const Eigen::Matrix4d& model);

  void LoadTexture(const std::shared_ptr<Texture>& texture);

private:
  GLuint location_projection_;
  GLuint location_view_;
  GLuint location_model_;
  GLuint location_texture_;
};
}

#endif // SIMPLAN_SHADER_LIGHT_SHADER_H_
