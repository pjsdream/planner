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
  location_texture_ = GetUniformLocation("material_texture");

  std::cout << location_projection_ << "\n"
            << location_view_ << "\n"
            << location_model_ << "\n"
            << location_texture_ << "\n";

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

    std::cout << location_lights_use_[i] << "\n"
              << location_lights_type_[i] << "\n"
              << location_lights_position_[i] << "\n"
              << location_lights_ambient_[i] << "\n"
              << location_lights_diffuse_[i] << "\n"
              << location_lights_specular_[i] << "\n"
              << location_lights_attenuation_[i] << "\n";
  }
}

LightShader::~LightShader()
{
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

  glUniformMatrix4fv(location_model_, 1, GL_FALSE, model_f.data());
}

void LightShader::LoadTexture(const std::shared_ptr<Texture>& texture)
{
  glUniform1i(location_texture_, 0);
}
}
