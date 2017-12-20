#ifndef PLANNER_GL_SHADER_LIGHT_SHADER_H_
#define PLANNER_GL_SHADER_LIGHT_SHADER_H_

#include "renderer/gl/shader/shader.h"

#include <memory>

#include "renderer/camera/camera.h"
#include "renderer/light/light.h"
#include "renderer/material/material.h"
#include "renderer/gl/resource/gl_texture.h"

namespace renderer
{
class LightShader : public Shader
{
public:
  static const int NUM_LIGHTS = 8;

private:
  enum class ColorOption : unsigned char
  {
    COLOR,
    GLOBAL_COLOR,
    TEXTURE,
  };

public:
  LightShader();

  void UseTexture(const std::shared_ptr<GlTexture>& texture);
  void UseColor();
  void UseGlobalColor(const Eigen::Vector3f& color);
  void SetAlpha(float alpha);

  void LoadCamera(const std::shared_ptr<Camera>& camera);
  void LoadModel(const Eigen::Affine3d& model);
  void LoadLight(int index, const std::shared_ptr<Light>& light);
  void LoadMaterial(const std::shared_ptr<Material>& material);

private:
  void LoadView(const Eigen::Affine3d& view);
  void LoadView(const Eigen::Matrix4d& view);
  void LoadProjection(const Eigen::Affine3d& projection);
  void LoadProjection(const Eigen::Matrix4d& projection);

  // Options
  ColorOption color_option_;

  // Shader locations
  GLint loc_model_;
  GLint loc_view_;
  GLint loc_projection_;
  GLint loc_model_inverse_transpose_;

  GLint loc_eye_position_;

  GLint loc_has_texture_;
  GLint loc_has_color_;
  GLint loc_has_global_color_;
  GLint loc_global_color_;
  GLint loc_has_material_;
  GLint loc_alpha_;

  struct GlLocationLight
  {
    GLint use;
    GLint type;
    GLint position;
    GLint ambient;
    GLint diffuse;
    GLint specular;
    GLint attenuation;
  };
  GlLocationLight loc_lights_[NUM_LIGHTS];

  struct GlLocationMaterial
  {
    GLint ambient;
    GLint diffuse;
    GLint specular;
    GLint shininess;
  };
  GlLocationMaterial loc_material_;

  GLint loc_material_texture_;
};
}

#endif // PLANNER_GL_SHADER_LIGHT_SHADER_H_
