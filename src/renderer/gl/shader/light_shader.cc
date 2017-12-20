#include "renderer/gl/shader/light_shader.h"

#include <iostream>

namespace renderer
{
LightShader::LightShader()
    : Shader()
{
  LoadVertexShaderFromFile("/home/jaesungp/cpp_workspace/planner/src/renderer/gl/shader/light_shader.vert");
  LoadFragmentShaderFromFile("/home/jaesungp/cpp_workspace/planner/src/renderer/gl/shader/light_shader.frag");
  Link();

  // Uniform locations
  loc_model_ = GetUniformLocation("model");
  loc_view_ = GetUniformLocation("view");
  loc_projection_ = GetUniformLocation("projection");
  loc_model_inverse_transpose_ = GetUniformLocation("model_inverse_transpose");

  loc_eye_position_ = GetUniformLocation("eye_position");

  loc_has_texture_ = GetUniformLocation("has_texture");
  loc_has_color_ = GetUniformLocation("has_color");
  loc_has_global_color_ = GetUniformLocation("has_global_color");
  loc_global_color_ = GetUniformLocation("global_color");
  loc_has_material_ = GetUniformLocation("has_material");
  loc_alpha_ = GetUniformLocation("alpha");

  for (int i = 0; i < NUM_LIGHTS; i++)
  {
    const std::string varname = std::string("lights[") + std::to_string(i) + "]";
    loc_lights_[i].use = GetUniformLocation(varname + ".use");
    loc_lights_[i].type = GetUniformLocation(varname + ".type");
    loc_lights_[i].position = GetUniformLocation(varname + ".position");
    loc_lights_[i].ambient = GetUniformLocation(varname + ".ambient");
    loc_lights_[i].diffuse = GetUniformLocation(varname + ".diffuse");
    loc_lights_[i].specular = GetUniformLocation(varname + ".specular");
    loc_lights_[i].attenuation = GetUniformLocation(varname + ".attenuation");
  }

  loc_material_.ambient = GetUniformLocation("material.ambient");
  loc_material_.diffuse = GetUniformLocation("material.diffuse");
  loc_material_.specular = GetUniformLocation("material.specular");
  loc_material_.shininess = GetUniformLocation("material.shininess");

  loc_material_texture_ = GetUniformLocation("material_texture");
}

void LightShader::UseTexture(const std::shared_ptr<GlTexture>& texture)
{
  if (color_option_ != ColorOption::TEXTURE)
  {
    color_option_ = ColorOption::TEXTURE;
    Uniform(loc_has_texture_, 1);
    Uniform(loc_has_color_, 0);
    Uniform(loc_has_global_color_, 0);
    Uniform(loc_has_material_, 0);
  }
  texture->Bind();
  Uniform(loc_material_texture_, 0);
}

void LightShader::UseColor()
{
  if (color_option_ != ColorOption::COLOR)
  {
    color_option_ = ColorOption::COLOR;
    Uniform(loc_has_texture_, 0);
    Uniform(loc_has_color_, 1);
    Uniform(loc_has_global_color_, 0);
    Uniform(loc_has_material_, 0);
  }
}

void LightShader::UseGlobalColor(const Eigen::Vector3f& color)
{
  if (color_option_ != ColorOption::GLOBAL_COLOR)
  {
    color_option_ = ColorOption::GLOBAL_COLOR;
    Uniform(loc_has_texture_, 0);
    Uniform(loc_has_color_, 0);
    Uniform(loc_has_global_color_, 1);
    Uniform(loc_has_material_, 0);
  }
  Uniform3f(loc_global_color_, color);
}

void LightShader::SetAlpha(float alpha)
{
  Uniform(loc_alpha_, alpha);
}

void LightShader::LoadModel(const Eigen::Affine3d& model)
{
  UniformMatrix4f(loc_model_, model.cast<float>().matrix());
  UniformMatrix3f(loc_model_inverse_transpose_, model.linear().inverse().transpose().cast<float>());
}

void LightShader::LoadView(const Eigen::Affine3d& view)
{
  UniformMatrix4f(loc_view_, view.cast<float>().matrix());
}

void LightShader::LoadView(const Eigen::Matrix4d& view)
{
  UniformMatrix4f(loc_view_, view.cast<float>());
}

void LightShader::LoadProjection(const Eigen::Affine3d& projection)
{
  UniformMatrix4f(loc_projection_, projection.cast<float>().matrix());
}

void LightShader::LoadProjection(const Eigen::Matrix4d& projection)
{
  UniformMatrix4f(loc_projection_, projection.cast<float>());
}

void LightShader::LoadCamera(const std::shared_ptr<Camera>& camera)
{
  Uniform3f(loc_eye_position_, camera->EyePosition().cast<float>());
  LoadView(camera->ViewMatrix());
  LoadProjection(camera->ProjectionMatrix());
}

void LightShader::LoadLight(int index, const std::shared_ptr<Light>& light)
{
  if (index >= 0 && index < NUM_LIGHTS)
  {
    Uniform(loc_lights_[index].use, light->use);
    Uniform3f(loc_lights_[index].position, light->position);
    Uniform3f(loc_lights_[index].ambient, light->ambient);
    Uniform3f(loc_lights_[index].diffuse, light->diffuse);
    Uniform3f(loc_lights_[index].specular, light->specular);

    switch (light->type)
    {
      case Light::LightType::DIRECTIONAL:
        Uniform(loc_lights_[index].type, 0);
        break;

      case Light::LightType::POINT:
        Uniform(loc_lights_[index].type, 1);
        Uniform3f(loc_lights_[index].attenuation, light->attenuation);
        break;
    }
  }
}

void LightShader::LoadMaterial(const std::shared_ptr<Material>& material)
{
  /*
  uniform(loc_material_.ambient, material->ambient);
  uniform(loc_material_.diffuse, material->diffuse);
  uniform(loc_material_.specular, material->specular);
  uniform(loc_material_.shininess, material->shininess);
   */
}
}
