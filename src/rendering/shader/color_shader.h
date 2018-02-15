#ifndef SIMPLAN_RENDERING_SHADER_COLOR_SHADER_H_
#define SIMPLAN_RENDERING_SHADER_COLOR_SHADER_H_

#include <memory>

#include "rendering/shader/shader.h"
#include "rendering/camera.h"

namespace simplan
{
class ColorShader : public Shader
{
public:
  ColorShader();
  ~ColorShader() override;

  void LoadCamera(const std::shared_ptr<Camera>& camera);
  void LoadModel(const Eigen::Matrix4d& model);

private:
  GLint location_projection_;
  GLint location_model_;
  GLint location_view_;
};
}

#endif // SIMPLAN_RENDERING_SHADER_COLOR_SHADER_H_
