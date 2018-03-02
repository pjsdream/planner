#ifndef SIMPLAN_SHADER_LIGHT_SHADER_H_
#define SIMPLAN_SHADER_LIGHT_SHADER_H_

#include <memory>

#include "rendering/shader/shader.h"
#include "rendering/camera.h"
#include "rendering/light.h"
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

  void LoadLight(int index, const std::shared_ptr<Light>& light);

  void LoadTexture(const std::shared_ptr<Texture>& texture);

private:
  GLuint location_projection_;
  GLuint location_view_;
  GLuint location_model_;
  GLuint location_texture_;

  static const int NUM_LIGHTS_ = 8;
  GLuint location_lights_use_[NUM_LIGHTS_];
  GLuint location_lights_type_[NUM_LIGHTS_];
  GLuint location_lights_position_[NUM_LIGHTS_];
  GLuint location_lights_ambient_[NUM_LIGHTS_];
  GLuint location_lights_diffuse_[NUM_LIGHTS_];
  GLuint location_lights_specular_[NUM_LIGHTS_];
  GLuint location_lights_attenuation_[NUM_LIGHTS_];
};
}

#endif // SIMPLAN_SHADER_LIGHT_SHADER_H_
