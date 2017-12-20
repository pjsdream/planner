#ifndef PLANNER_RENDERER_GL_RESOURCE_LIGHT_H_
#define PLANNER_RENDERER_GL_RESOURCE_LIGHT_H_

#include <Eigen/Dense>

namespace renderer
{
struct Light
{
  enum class LightType
  {
    DIRECTIONAL,
    POINT,
  };

  bool use;
  LightType type;
  Eigen::Vector3f position;
  Eigen::Vector3f ambient;
  Eigen::Vector3f diffuse;
  Eigen::Vector3f specular;
  Eigen::Vector3f attenuation;
};
}

#endif // PLANNER_RENDERER_GL_RESOURCE_LIGHT_H_
