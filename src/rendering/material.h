#ifndef SIMPLAN_RENDERING_MATERIAL_H_
#define SIMPLAN_RENDERING_MATERIAL_H_

#include <Eigen/Dense>

namespace simplan
{
struct Material
{
  Eigen::Vector3f ambient;
  Eigen::Vector3f diffuse;
  Eigen::Vector3f specular;
  float shininess;
};
}

#endif // SIMPLAN_RENDERING_MATERIAL_H_
