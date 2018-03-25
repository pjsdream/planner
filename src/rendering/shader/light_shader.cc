#include "rendering/shader/light_shader.h"

#include <iostream>

namespace simplan
{
LightShader::LightShader()
    : Shader()
{
#ifdef _WIN32
  LoadVertexShader("C:\\Users\\pjsdr_000\\Desktop\\documents\\planner\\src\\shader\\light.vert");
  LoadFragmentShader("C:\\Users\\pjsdr_000\\Desktop\\documents\\planner\\src\\shader\\light.frag");
#else
  LoadVertexShader("/home/jaesungp/cpp_workspace/planner/src/shader/light.vert");
  LoadFragmentShader("/home/jaesungp/cpp_workspace/planner/src/shader/light.frag");
#endif

  Link();

  location_projection_ = GetUniformLocation("projection");
  location_view_ = GetUniformLocation("view");
  location_model_ = GetUniformLocation("model");
  location_model_inverse_transpose_ = GetUniformLocation("model_inverse_transpose");

  location_has_diffuse_texture_ = GetUniformLocation("has_diffuse_texture");
  location_diffuse_texture_ = GetUniformLocation("diffuse_texture");

  for (int i = 0; i < NUM_LIGHTS_; i++)
  {
    auto uniform_prefix = std::string("lights[") + std::to_string(i) + "]";

    location_lights_use_[i] = GetUniformLocation(uniform_prefix + ".use");
    location_lights_type_[i] = GetUniformLocation(uniform_prefix + ".type");
    location_lights_position_[i] = GetUniformLocation(uniform_prefix + ".position");
    location_lights_ambient_[i] = GetUniformLocation(uniform_prefix + ".ambient");
    location_lights_diffuse_[i] = GetUniformLocation(uniform_prefix + ".diffuse");
    location_lights_specular_[i] = GetUniformLocation(uniform_prefix + ".specular");
    location_lights_attenuation_[i] = GetUniformLocation(uniform_prefix + ".attenuation");
  }

  location_material_ambient_ = GetUniformLocation("material.ambient");
  location_material_diffuse_ = GetUniformLocation("material.diffuse");
  location_material_specular_ = GetUniformLocation("material.specular");
  location_material_shininess_ = GetUniformLocation("material.shininess");
}

LightShader::~LightShader()
{
}

void LightShader::Use()
{
  Shader::Use();

  for (int i = 0; i < NUM_LIGHTS_; i++)
    glUniform1i(location_lights_use_[i], 0);
}

void LightShader::LoadCamera(const std::shared_ptr<Camera>& camera)
{
  Eigen::Matrix4f projection = camera->ProjectionMatrix().cast<float>();
  Eigen::Matrix4f view = camera->ViewMatrix().cast<float>();

  glUniformMatrix4fv(location_projection_, 1, GL_FALSE, projection.data());
  glUniformMatrix4fv(location_view_, 1, GL_FALSE, view.data());
}

void LightShader::LoadModel(const Eigen::Matrix4d& model)
{
  Eigen::Matrix4f model_f = model.cast<float>();
  Eigen::Matrix3f model_it = model_f.block(0, 0, 3, 3).inverse().transpose();

  glUniformMatrix4fv(location_model_, 1, GL_FALSE, model_f.data());
  glUniformMatrix3fv(location_model_inverse_transpose_, 1, GL_FALSE, model_it.data());

  // TODO: move this line to a proper function
  glUniform1i(location_has_diffuse_texture_, 1);
  glUniform1i(location_diffuse_texture_, 0);
}

void LightShader::LoadLight(int index, const std::shared_ptr<Light>& light)
{
  switch (light->type)
  {
  case Light::Type::DIRECTIONAL:
    glUniform1i(location_lights_type_[index], 0);
    break;

  case Light::Type::POINT:
    glUniform1i(location_lights_type_[index], 1);
    break;
  }

  glUniform1i(location_lights_use_[index], 1);
  glUniform3fv(location_lights_position_[index], 1, light->position.data());
  glUniform3fv(location_lights_ambient_[index], 1, light->ambient.data());
  glUniform3fv(location_lights_diffuse_[index], 1, light->diffuse.data());
  glUniform3fv(location_lights_specular_[index], 1, light->specular.data());
  glUniform3fv(location_lights_attenuation_[index], 1, light->attenuation.data());
}

void LightShader::LoadMaterial(const std::shared_ptr<Material>& material)
{
  glUniform3fv(location_material_ambient_, 1, material->ambient.data());
  glUniform3fv(location_material_diffuse_, 1, material->diffuse.data());
  glUniform3fv(location_material_specular_, 1, material->specular.data());
  glUniform1f(location_material_shininess_, material->shininess);
}
}
