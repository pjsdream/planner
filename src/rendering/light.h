#ifndef SIMPLAN_RENDERING_LIGHT_H_
#define SIMPLAN_RENDERING_LIGHT_H_

#include <Eigen/Dense>

namespace simplan
{
struct Light
{
  enum class Type
  {
    DIRECTIONAL,
    POINT,
  };

  Type type;

  Eigen::Vector3f position;

  Eigen::Vector3f ambient;
  Eigen::Vector3f diffuse;
  Eigen::Vector3f specular;

  Eigen::Vector3f attenuation;
};
}

#endif // SIMPLAN_RENDERING_LIGHT_H_
