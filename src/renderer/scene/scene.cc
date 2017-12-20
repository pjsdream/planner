#include "renderer/scene/scene.h"
#include "renderer/gl/shader/light_shader.h"

namespace renderer
{
Scene::Scene()
{
  // Create root node
  root_ = std::make_shared<SceneNode>();

  // Create a default scene
  for (int i = 0; i < LightShader::NUM_LIGHTS; i++)
  {
    lights_.push_back(std::make_shared<Light>());
    lights_[i]->use = false;
  }

  lights_[0]->use = true;
  lights_[0]->type = Light::LightType::POINT;
  lights_[0]->position = Eigen::Vector3f(10, 0, 10);
  lights_[0]->ambient = Eigen::Vector3f(0.2, 0.2, 0.2);
  lights_[0]->diffuse = Eigen::Vector3f(0.5, 0.5, 0.5);
  lights_[0]->specular = Eigen::Vector3f(0.5, 0.5, 0.5);
  lights_[0]->attenuation = Eigen::Vector3f(0.8, 0.01, 0.001);

  lights_[1]->use = true;
  lights_[1]->type = Light::LightType::POINT;
  lights_[1]->position = Eigen::Vector3f(0, 10, 10);
  lights_[1]->ambient = Eigen::Vector3f(0.2, 0.2, 0.2);
  lights_[1]->diffuse = Eigen::Vector3f(0.4, 0.4, 0.4);
  lights_[1]->specular = Eigen::Vector3f(0.4, 0.4, 0.4);
  lights_[1]->attenuation = Eigen::Vector3f(0.8, 0.01, 0.001);
}
}
