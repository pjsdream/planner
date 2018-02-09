#ifndef SIMPLAN_RESOURCE_RAW_MESH_H_
#define SIMPLAN_RESOURCE_RAW_MESH_H_

#include <Eigen/Dense>

namespace simplan
{
struct RawMesh
{
  Eigen::Matrix3Xf vertices;
  Eigen::Matrix3Xf normals;
  Eigen::Matrix2Xf texture_coordinates;
  std::string texture_filename;
  Eigen::Matrix3Xi triangles;
};
}

#endif // SIMPLAN_RESOURCE_RAW_MESH_H_
