#include "renderer/gl/resource/gl_object.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace renderer
{
namespace
{
static std::string getDirectory(const std::string filename)
{
  return filename.substr(0, filename.find_last_of('/'));
}
}

GlObject::GlObject()
    : loading_status_(LoadingStatus::UNINITIALIZED),
      vao_(0),
      has_elements_(false),
      num_vertices_(0),
      draw_mode_(GL_TRIANGLES)
{
}

GlObject::~GlObject()
{
  if (vao_)
    glDeleteVertexArrays(1, &vao_);

  glDeleteBuffers(vbos_.size(), vbos_.data());
}

void GlObject::LoadMeshFromFile(const std::string& filename)
{
  loading_status_ = LoadingStatus::LOADING;

  future_mesh_ = std::async(
      std::launch::async,
      [&, filename]()
      {
        Assimp::Importer assimp_importer{};
        const aiScene* scene = assimp_importer.ReadFile(filename, aiProcess_Triangulate);

        if (scene != 0)
        {
          aiMesh* mesh = scene->mMeshes[0];

          std::copy(reinterpret_cast<float*>(mesh->mVertices),
                    reinterpret_cast<float*>(mesh->mVertices + mesh->mNumVertices),
                    std::back_inserter(vertex_buffer_));

          std::copy(reinterpret_cast<float*>(mesh->mNormals),
                    reinterpret_cast<float*>(mesh->mNormals + mesh->mNumVertices),
                    std::back_inserter(normal_buffer_));

          if (mesh->HasTextureCoords(0))
          {
            for (int i = 0; i < mesh->mNumVertices; i++)
            {
              texture_buffer_.push_back(mesh->mTextureCoords[0][i][0]);
              texture_buffer_.push_back(1.f - mesh->mTextureCoords[0][i][1]);
            }

            auto material = scene->mMaterials[0];
            aiString path;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

            std::string texture_filename = getDirectory(filename) + "/" + path.C_Str();

            // TODO: re-use same texture objects instead of creating texture every time
            texture_ = std::make_shared<GlTexture>();
            texture_->LoadTextureFromFile(texture_filename);
          }

          else
          {
            // It doesn't have a texture. Use per-vertex or global color
          }

          element_buffer_.reserve(mesh->mNumFaces * 3);
          for (int i = 0; i < mesh->mNumFaces; i++)
          {
            element_buffer_.push_back(mesh->mFaces[i].mIndices[0]);
            element_buffer_.push_back(mesh->mFaces[i].mIndices[1]);
            element_buffer_.push_back(mesh->mFaces[i].mIndices[2]);
          }
        }
      });
}

bool GlObject::HasTexture()
{
  if (loading_status_ != LoadingStatus::READY)
    return false;

  if (texture_ == nullptr)
    return false;

  if (texture_->IsReady())
    return true;

  // Return true on successful preparation of GL texture
  return texture_->Prepare();
}

void GlObject::CreateSphere()
{
  std::vector<float> vertex_buffer;
  std::vector<float> normal_buffer;
  std::vector<int> element_buffer;

  constexpr int discretization = 32;

  // Add north and south pole vertices
  vertex_buffer.push_back(0.f);
  vertex_buffer.push_back(0.f);
  vertex_buffer.push_back(1.f);
  normal_buffer.push_back(0.f);
  normal_buffer.push_back(0.f);
  normal_buffer.push_back(1.f);

  vertex_buffer.push_back(0.f);
  vertex_buffer.push_back(0.f);
  vertex_buffer.push_back(-1.f);
  normal_buffer.push_back(0.f);
  normal_buffer.push_back(0.f);
  normal_buffer.push_back(-1.f);

  // Add vertices
  for (int i = 0; i < discretization; i++)
  {
    double u = static_cast<double>(i + 1) / (discretization + 1);
    u = (1. - u) * (M_PI / 2.) + u * (-M_PI / 2.);
    const double cu = std::cos(u);
    const double su = std::sin(u);

    for (int j = 0; j < discretization; j++)
    {
      const double v = static_cast<double>(j) / (discretization - 1) * (M_PI * 2.);
      const double cv = std::cos(v);
      const double sv = std::sin(v);

      vertex_buffer.push_back(cv * cu);
      vertex_buffer.push_back(sv * cu);
      vertex_buffer.push_back(su);
      normal_buffer.push_back(cv * cu);
      normal_buffer.push_back(sv * cu);
      normal_buffer.push_back(su);
    }
  }

  // Fill up element buffer
  for (int i = 0; i < discretization; i++)
  {
    element_buffer.push_back(0);

    for (int j = 0; j < discretization; j++)
    {
      element_buffer.push_back(2 + j * discretization + i);
      element_buffer.push_back(2 + j * discretization + ((i + 1) % discretization));
    }

    element_buffer.push_back(1);
    element_buffer.push_back(-1);
  }

  // Create gl objects
  glGenVertexArrays(1, &vao_);
  vbos_.resize(3);
  glGenBuffers(3, vbos_.data());

  glBindVertexArray(vao_);

  // Vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), vertex_buffer.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  // Normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
  glBufferData(GL_ARRAY_BUFFER, normal_buffer.size() * sizeof(float), normal_buffer.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(1);

  // Element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer.size() * sizeof(int), element_buffer.data(), GL_STATIC_DRAW);

  // Unbind objects
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  has_elements_ = true;
  draw_mode_ = GL_TRIANGLE_STRIP;
  num_vertices_ = element_buffer.size();

  loading_status_ = LoadingStatus::READY;
}

void GlObject::CreateCylinder()
{
  std::vector<float> vertex_buffer;
  std::vector<float> normal_buffer;
  std::vector<int> element_buffer;

  constexpr int discretization = 32;

  // Add cap ertices
  vertex_buffer.push_back(0.);
  vertex_buffer.push_back(0.);
  vertex_buffer.push_back(1.);
  normal_buffer.push_back(0.);
  normal_buffer.push_back(0.);
  normal_buffer.push_back(1.);
  for (int i = 0; i < discretization; i++)
  {
    double u = static_cast<double>(i) / (discretization - 1) * (M_PI * 2.);
    const double cu = std::cos(u);
    const double su = std::sin(u);

    vertex_buffer.push_back(cu);
    vertex_buffer.push_back(su);
    vertex_buffer.push_back(1.);
    normal_buffer.push_back(0.);
    normal_buffer.push_back(0.);
    normal_buffer.push_back(1.);
  }

  vertex_buffer.push_back(0.);
  vertex_buffer.push_back(0.);
  vertex_buffer.push_back(-1.);
  normal_buffer.push_back(0.);
  normal_buffer.push_back(0.);
  normal_buffer.push_back(-1.);
  for (int i = 0; i < discretization; i++)
  {
    double u = static_cast<double>(i) / (discretization - 1) * (M_PI * 2.);
    const double cu = std::cos(u);
    const double su = std::sin(u);

    vertex_buffer.push_back(cu);
    vertex_buffer.push_back(su);
    vertex_buffer.push_back(-1.);
    normal_buffer.push_back(0.);
    normal_buffer.push_back(0.);
    normal_buffer.push_back(-1.);
  }

  // Add side vertices
  for (int i = 0; i < discretization; i++)
  {
    double u = static_cast<double>(i) / (discretization - 1) * (M_PI * 2.);
    const double cu = std::cos(u);
    const double su = std::sin(u);

    vertex_buffer.push_back(cu);
    vertex_buffer.push_back(su);
    vertex_buffer.push_back(1.);
    normal_buffer.push_back(cu);
    normal_buffer.push_back(su);
    normal_buffer.push_back(0.);
  }

  for (int i = 0; i < discretization; i++)
  {
    double u = static_cast<double>(i) / (discretization - 1) * (M_PI * 2.);
    const double cu = std::cos(u);
    const double su = std::sin(u);

    vertex_buffer.push_back(cu);
    vertex_buffer.push_back(su);
    vertex_buffer.push_back(-1.);
    normal_buffer.push_back(cu);
    normal_buffer.push_back(su);
    normal_buffer.push_back(0.);
  }

  // Fill up element buffer
  for (int i = 0; i < discretization; i++)
  {
    element_buffer.push_back(0);
    element_buffer.push_back(1 + i);
    element_buffer.push_back(1 + ((i + 1) % discretization));
  }

  for (int i = 0; i < discretization; i++)
  {
    element_buffer.push_back(1 + discretization);
    element_buffer.push_back(2 + discretization + i);
    element_buffer.push_back(2 + discretization + ((i + 1) % discretization));
  }

  for (int i = 0; i < discretization; i++)
  {
    int v1 = 2 + discretization * 2 + i;
    int v2 = 2 + discretization * 3 + i;
    int v3 = 2 + discretization * 2 + ((i + 1) % discretization);
    int v4 = 2 + discretization * 3 + ((i + 1) % discretization);

    element_buffer.push_back(v1);
    element_buffer.push_back(v2);
    element_buffer.push_back(v3);

    element_buffer.push_back(v2);
    element_buffer.push_back(v4);
    element_buffer.push_back(v3);
  }

  // Create gl objects
  glGenVertexArrays(1, &vao_);
  vbos_.resize(3);
  glGenBuffers(3, vbos_.data());

  glBindVertexArray(vao_);

  // Vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), vertex_buffer.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  // Normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
  glBufferData(GL_ARRAY_BUFFER, normal_buffer.size() * sizeof(float), normal_buffer.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(1);

  // Element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer.size() * sizeof(int), element_buffer.data(), GL_STATIC_DRAW);

  // Unbind objects
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  has_elements_ = true;
  draw_mode_ = GL_TRIANGLES;
  num_vertices_ = element_buffer.size();

  loading_status_ = LoadingStatus::READY;
}

void GlObject::CreateCone()
{
  std::vector<float> vertex_buffer;
  std::vector<float> normal_buffer;
  std::vector<int> element_buffer;

  constexpr int discretization = 32;

  // Add up vertices
  Eigen::Vector3d n = Eigen::Vector3d(2, 0, 1).normalized();
  for (int i = 0; i < discretization; i++)
  {
    double u1 = static_cast<double>(i) / (discretization - 1) * (M_PI * 2.);
    const double cu1 = std::cos(u1);
    const double su1 = std::sin(u1);

    double u2 = static_cast<double>(i + 1) / (discretization - 1) * (M_PI * 2.);
    const double cu2 = std::cos(u2);
    const double su2 = std::sin(u2);

    double um = (i + 0.5) / (discretization - 1) * (M_PI * 2.);
    const double cum = std::cos(um);
    const double sum = std::sin(um);

    vertex_buffer.push_back(cu1);
    vertex_buffer.push_back(su1);
    vertex_buffer.push_back(-1.);
    normal_buffer.push_back(cu1 * n(0));
    normal_buffer.push_back(su1 * n(0));
    normal_buffer.push_back(n(2));

    vertex_buffer.push_back(cu2);
    vertex_buffer.push_back(su2);
    vertex_buffer.push_back(-1.);
    normal_buffer.push_back(cu2 * n(0));
    normal_buffer.push_back(su2 * n(0));
    normal_buffer.push_back(n(2));

    vertex_buffer.push_back(0.);
    vertex_buffer.push_back(0.);
    vertex_buffer.push_back(1.);
    normal_buffer.push_back(cum * n(0));
    normal_buffer.push_back(sum * n(0));
    normal_buffer.push_back(n(2));

    element_buffer.push_back(3 * i);
    element_buffer.push_back(3 * i + 1);
    element_buffer.push_back(3 * i + 2);
  }

  // Add bottom vertices
  vertex_buffer.push_back(0.);
  vertex_buffer.push_back(0.);
  vertex_buffer.push_back(-1.);
  normal_buffer.push_back(0.);
  normal_buffer.push_back(0.);
  normal_buffer.push_back(-1.);
  for (int i = 0; i < discretization; i++)
  {
    double u = static_cast<double>(i) / (discretization - 1) * (M_PI * 2.);
    const double cu = std::cos(u);
    const double su = std::sin(u);

    vertex_buffer.push_back(cu);
    vertex_buffer.push_back(su);
    vertex_buffer.push_back(-1.);
    normal_buffer.push_back(0.);
    normal_buffer.push_back(0.);
    normal_buffer.push_back(-1.);

    element_buffer.push_back(3 * discretization);
    element_buffer.push_back(3 * discretization + 1 + ((i + 1) % discretization));
    element_buffer.push_back(3 * discretization + 1 + i);
  }

  // Create gl objects
  glGenVertexArrays(1, &vao_);
  vbos_.resize(3);
  glGenBuffers(3, vbos_.data());

  glBindVertexArray(vao_);

  // Vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
  glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), vertex_buffer.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  // Normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
  glBufferData(GL_ARRAY_BUFFER, normal_buffer.size() * sizeof(float), normal_buffer.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(1);

  // Element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer.size() * sizeof(int), element_buffer.data(), GL_STATIC_DRAW);

  // Unbind objects
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  has_elements_ = true;
  draw_mode_ = GL_TRIANGLES;
  num_vertices_ = element_buffer.size();

  loading_status_ = LoadingStatus::READY;
}

void GlObject::CreateCube()
{
  float vertex_buffer[] = {
      -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1, // top
      -1, -1, -1, -1, 1, -1, 1, -1, -1, 1, 1, -1, // bottom
      -1, -1, -1, -1, -1, 1, -1, 1, -1, -1, 1, 1, // left
      1, -1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1, // right
      -1, -1, -1, 1, -1, -1, -1, -1, 1, 1, -1, 1, // front
      -1, 1, -1, -1, 1, 1, 1, 1, -1, 1, 1, 1, // back
  };

  float normal_buffer[] = {
      0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, // top
      0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, // bottom
      -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, // left
      1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, // right
      0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, // front
      0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, // back
  };

  int element_buffer[] = {
      0, 1, 2, 3, -1,
      4, 5, 6, 7, -1,
      8, 9, 10, 11, -1,
      12, 13, 14, 15, -1,
      16, 17, 18, 19, -1,
      20, 21, 22, 23, -1,
  };

  // Create gl objects
  glGenVertexArrays(1, &vao_);
  vbos_.resize(3);
  glGenBuffers(3, vbos_.data());

  glBindVertexArray(vao_);

  // Vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
  glBufferData(GL_ARRAY_BUFFER, 3 * 4 * 6 * sizeof(float), vertex_buffer, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  // Normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
  glBufferData(GL_ARRAY_BUFFER, 3 * 4 * 6 * sizeof(float), normal_buffer, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(1);

  // Element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 5 * 6 * sizeof(int), element_buffer, GL_STATIC_DRAW);

  // Unbind objects
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  has_elements_ = true;
  draw_mode_ = GL_TRIANGLE_STRIP;
  num_vertices_ = 30;

  loading_status_ = LoadingStatus::READY;
}

void GlObject::ContinueLoadingMesh()
{
  using namespace std::chrono_literals;

  if (future_mesh_.valid() && future_mesh_.wait_for(0s) == std::future_status::ready)
  {
    future_mesh_.get();

    // Gl vao and vbos
    glGenVertexArrays(1, &vao_);
    vbos_.resize(4);
    glGenBuffers(4, vbos_.data());

    glBindVertexArray(vao_);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_.size() * sizeof(float), vertex_buffer_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
    glBufferData(GL_ARRAY_BUFFER, normal_buffer_.size() * sizeof(float), normal_buffer_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // Texture buffer
    if (!texture_buffer_.empty())
    {
      glBindBuffer(GL_ARRAY_BUFFER, vbos_[2]);
      glBufferData(GL_ARRAY_BUFFER, texture_buffer_.size() * sizeof(float), texture_buffer_.data(), GL_STATIC_DRAW);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
      glEnableVertexAttribArray(2);
    }

    // Element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos_[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_.size() * sizeof(int), element_buffer_.data(), GL_STATIC_DRAW);

    // Unbind objects
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    has_elements_ = true;
    draw_mode_ = GL_TRIANGLES;
    num_vertices_ = element_buffer_.size();

    loading_status_ = LoadingStatus::READY;
  }
}

void GlObject::Draw()
{
  if (loading_status_ == LoadingStatus::LOADING)
    ContinueLoadingMesh();

  if (loading_status_ == LoadingStatus::READY)
  {
    glBindVertexArray(vao_);

    if (has_elements_)
      glDrawElements(draw_mode_, num_vertices_, GL_UNSIGNED_INT, 0);

    else
      glDrawArrays(draw_mode_, 0, num_vertices_);

    glBindVertexArray(0);
  }
}
}
