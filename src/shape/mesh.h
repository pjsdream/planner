#ifndef SIMPLAN_SHAPE_MESH_H_
#define SIMPLAN_SHAPE_MESH_H_

#include <string>
#include <vector>

#include <Eigen/Dense>

namespace simplan
{
class Mesh
{
public:
  Mesh();
  ~Mesh();

private:
  Eigen::Matrix3Xd vertices_;
  Eigen::Matrix3Xi triangles_;
};
}

#endif // SIMPLAN_SHAPE_MESH_H_
