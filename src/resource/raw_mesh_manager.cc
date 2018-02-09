#include "resource/raw_mesh_manager.h"

#include <unordered_map>
#include <future>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

namespace simplan
{
namespace
{
std::unordered_map<std::string, std::shared_ptr<RawMesh>> filename_to_mesh;
std::unordered_map<std::string, std::future<RawMesh>> filename_to_future_mesh;

std::string GetDirectory(const std::string& filename)
{
  return filename.substr(0, filename.find_last_of('/'));
}
}

std::shared_ptr<RawMesh> RawMeshManager::LoadFromFile(const std::string& filename)
{
  if (filename_to_mesh.find(filename) != filename_to_mesh.cend())
  {
    return filename_to_mesh[filename];
  }

  if (filename_to_future_mesh.find(filename) != filename_to_future_mesh.cend())
  {
    auto& f = filename_to_future_mesh[filename];
    if (f.valid() && f.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
      filename_to_mesh[filename] = std::make_shared<RawMesh>(f.get());
      filename_to_future_mesh.erase(filename);

      return filename_to_mesh[filename];
    }
  }

  filename_to_future_mesh.insert(std::make_pair(filename, std::async(std::launch::async, [filename]()
  {
    RawMesh raw_mesh;

    Assimp::Importer assimp_importer{};
    const aiScene* scene =
        assimp_importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if (scene != 0)
    {
      aiMesh* mesh = scene->mMeshes[0];

      raw_mesh.vertices.resize(Eigen::NoChange, mesh->mNumVertices);
      std::memcpy(raw_mesh.vertices.data(), (float*) mesh->mVertices, sizeof(float) * mesh->mNumVertices * 3);

      raw_mesh.normals.resize(Eigen::NoChange, mesh->mNumVertices);
      std::memcpy(raw_mesh.normals.data(), (float*) mesh->mNormals, sizeof(float) * mesh->mNumVertices * 3);

      if (mesh->HasTextureCoords(0))
      {
        raw_mesh.texture_coordinates.resize(Eigen::NoChange, mesh->mNumVertices);
        for (int i = 0; i < mesh->mNumVertices; i++)
        {
          raw_mesh.texture_coordinates(0, i) = mesh->mTextureCoords[0][i][0];
          raw_mesh.texture_coordinates(1, i) = 1. - mesh->mTextureCoords[0][i][1];
        }

        auto material = scene->mMaterials[0];
        aiString path;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

        raw_mesh.texture_filename = GetDirectory(filename) + "/" + path.C_Str();
      }

      raw_mesh.triangles.resize(Eigen::NoChange, mesh->mNumFaces);
      for (int i = 0; i < mesh->mNumFaces; i++)
      {
        raw_mesh.triangles(0, i) = mesh->mFaces[i].mIndices[0];
        raw_mesh.triangles(1, i) = mesh->mFaces[i].mIndices[1];
        raw_mesh.triangles(2, i) = mesh->mFaces[i].mIndices[2];
      }
    }

    return raw_mesh;
  })));

  return nullptr;
}
}
