#ifndef SIMPLAN_RESOURCE_RAW_MESH_MANAGER_H_
#define SIMPLAN_RESOURCE_RAW_MESH_MANAGER_H_

#include <memory>

#include "resource/raw_mesh.h"

namespace simplan
{
class RawMeshManager
{
public:
  static std::shared_ptr<RawMesh> LoadFromFile(const std::string& filename);

public:
private:
};
}

#endif // SIMPLAN_RESOURCE_RAW_MESH_MANAGER_H_
