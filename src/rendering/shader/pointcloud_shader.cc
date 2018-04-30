#include "rendering/shader/pointcloud_shader.h"

namespace simplan
{
PointcloudShader::PointcloudShader()
    : Shader()
{
#ifdef _WIN32
  LoadVertexShader("C:\\Users\\pjsdr\\Desktop\\documents\\planner\\src\\shader\\pointcloud.vert");
  LoadFragmentShader("C:\\Users\\pjsdr\\Desktop\\documents\\planner\\src\\shader\\pointcloud.frag");
#else
  LoadVertexShader("/home/jaesungp/cpp_workspace/planner/src/shader/color.vert");
  LoadFragmentShader("/home/jaesungp/cpp_workspace/planner/src/shader/color.frag");
#endif

  Link();

  location_projection_ = GetUniformLocation("projection");
  location_view_ = GetUniformLocation("view");
  location_model_ = GetUniformLocation("model");
}

PointcloudShader::~PointcloudShader()
{
}

void PointcloudShader::LoadCamera(const std::shared_ptr<Camera>& camera)
{
  Eigen::Matrix4f projection = camera->ProjectionMatrix().cast<float>();
  Eigen::Matrix4f view = camera->ViewMatrix().cast<float>();

  glUniformMatrix4fv(location_projection_, 1, GL_FALSE, projection.data());
  glUniformMatrix4fv(location_view_, 1, GL_FALSE, view.data());
}

void PointcloudShader::LoadModel(const Eigen::Matrix4d& model)
{
  Eigen::Matrix4f model_f = model.cast<float>();

  glUniformMatrix4fv(location_model_, 1, GL_FALSE, model_f.data());
}
}
