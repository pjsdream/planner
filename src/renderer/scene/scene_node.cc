#include "renderer/scene/scene_node.h"

namespace renderer
{
SceneNode::SceneNode()
    : transform_(Eigen::Affine3d::Identity()), color_(0.8f, 0.8f, 0.8f)
{
}


void SceneNode::AddArrow(const Eigen::Vector3d& p, const Eigen::Vector3d& q, const Eigen::Vector3f& color)
{
  constexpr double radius_ratio = 0.01;
  constexpr double cylinder_length_ratio = 0.9;

  const double l = (p - q).norm();
  const double radius = radius_ratio * l;

  const Eigen::Vector3d z = (q - p) / l;
  Eigen::Vector3d y;
  if (std::abs(z(0)) >= 1. - 1e-3)
    y = z.cross(Eigen::Vector3d(0., 1., 0.)).normalized();
  else
    y = z.cross(Eigen::Vector3d(1., 0., 0.)).normalized();
  const Eigen::Vector3d x = y.cross(z).normalized();

  Eigen::Affine3d transform = Eigen::Affine3d::Identity();
  transform.translate(p);
  transform.linear().col(0) = x;
  transform.linear().col(1) = y;
  transform.linear().col(2) = z;

  auto local_coordinate_node = CreateChild();
  local_coordinate_node->SetTransform(transform);

  const double cylinder_length = cylinder_length_ratio * l;
  auto cylinder = std::make_shared<renderer::SceneObject>("cylinder");
  cylinder->SetGlobalColor(color);
  auto cylinder_node = local_coordinate_node->CreateChild();
  cylinder_node->AttachObject(cylinder);
  cylinder_node->Translate(Eigen::Vector3d(0., 0., cylinder_length / 2.))
      .Scale(Eigen::Vector3d(radius, radius, cylinder_length / 2.));

  const double cone_length = l - cylinder_length;
  const double cone_radius = 2. * radius;
  auto cone = std::make_shared<renderer::SceneObject>("cone");
  cone->SetGlobalColor(color);
  auto cone_node = local_coordinate_node->CreateChild();
  cone_node->AttachObject(cone);
  cone_node->Translate(Eigen::Vector3d(0., 0., cylinder_length + cone_length / 2.))
      .Scale(Eigen::Vector3d(cone_radius, cone_radius, cone_length / 2.));

}

void SceneNode::AddLocalCoodinate(const Eigen::Affine3d& transform, double length)
{
  const Eigen::Vector3d p = transform.translation();
  AddArrow(p, p + transform.linear().col(0) * length, Eigen::Vector3f(1.f, 0.f, 0.f));
  AddArrow(p, p + transform.linear().col(1) * length, Eigen::Vector3f(0.f, 1.f, 0.f));
  AddArrow(p, p + transform.linear().col(2) * length, Eigen::Vector3f(0.f, 0.f, 1.f));
}
}
