#ifndef SIMPLAN_LIGHT_H
#define SIMPLAN_LIGHT_H

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

  Eigen::Vector3d position;

  Eigen::Vector3d ambient;
  Eigen::Vector3d diffuse;
  Eigen::Vector3d specular;

  Eigen::Vector3d attenuation;
};
}

#endif //SIMPLAN_LIGHT_H
