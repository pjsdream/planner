#include "rendering/camera.h"

namespace simplan
{
Camera::Camera()
    : eye_(1., 1., 1.),
      up_(0., 0., 1.),
      center_(0., 0., 0.),
      translation_speed_(0.001),
      rotation_speed_(0.001),
      zoom_speed_(0.001),
      fovy_(60.),
      aspect_(4. / 3.),
      near_(-10.),
      far_(10.),
      projection_type_(ProjectionType::ORTHO)
{
}

void Camera::TranslateByPixel(int dx, int dy)
{
  const Eigen::Vector3d n = (eye_ - center_).normalized();
  const Eigen::Vector3d u = up_.cross(n).normalized();
  const Eigen::Vector3d v = n.cross(u);

  eye_ += translation_speed_ * -dx * u;
  eye_ -= translation_speed_ * -dy * v;

  center_ += translation_speed_ * -dx * u;
  center_ -= translation_speed_ * -dy * v;
}

void Camera::RotateByPixel(int dx, int dy)
{
  const Eigen::Vector3d n = (eye_ - center_).normalized();
  const Eigen::Vector3d u = up_.cross(n).normalized();
  const Eigen::Vector3d v = n.cross(u);

  double y_diff = -rotation_speed_ * dy;

  // y angle
  const double angle = std::acos(up_.dot(n));
  const double angle_delta = 0.01;

  if (angle + y_diff < angle_delta)
    y_diff = angle_delta - angle;

  else if (angle + y_diff > M_PI - angle_delta)
    y_diff = M_PI - angle_delta - angle;

  Eigen::AngleAxisd rotation_x(-rotation_speed_ * dx, up_);
  Eigen::AngleAxisd rotation_y(y_diff, u);

  eye_ = center_ + rotation_x * (eye_ - center_);
  eye_ = center_ + rotation_y * (eye_ - center_);
}

void Camera::ZoomByPixel(int dx, int dy)
{
  const Eigen::Vector3d n = (eye_ - center_).normalized();
  const Eigen::Vector3d u = up_.cross(n).normalized();
  const Eigen::Vector3d v = n.cross(u);

  const double distance = zoom_speed_ * (-dy);
  const double minimum_distance = 0.01;

  if ((eye_ - center_).norm() - distance >= minimum_distance)
    eye_ -= distance * n;

  else
    eye_ = center_ + minimum_distance * n;
}

Eigen::Matrix4d Camera::ProjectionMatrix() const
{
  switch (projection_type_)
  {
    case ProjectionType::ORTHO:
      return Ortho();

    case ProjectionType::PERSPECTIVE:
      return Perspective();
  }
}

Eigen::Matrix4d Camera::ViewMatrix() const
{
  const Eigen::Vector3d n = (eye_ - center_).normalized();
  const Eigen::Vector3d u = up_.cross(n).normalized();
  const Eigen::Vector3d v = n.cross(u);

  Eigen::Matrix4d mat = Eigen::Matrix4d::Identity();
  mat.block(0, 0, 1, 3) = u.transpose();
  mat.block(1, 0, 1, 3) = v.transpose();
  mat.block(2, 0, 1, 3) = n.transpose();
  mat(0, 3) = -eye_.dot(u);
  mat(1, 3) = -eye_.dot(v);
  mat(2, 3) = -eye_.dot(n);

  return mat;
}

Eigen::Matrix4d Camera::Perspective() const
{
  const double t = tan((fovy_ / 180. * M_PI) / 2.);

  Eigen::Matrix4d mat = Eigen::Matrix4d::Zero();
  mat(0, 0) = 1. / (aspect_ * t);
  mat(1, 1) = 1. / t;
  mat(2, 2) = -(near_ + far_) / (far_ - near_);
  mat(2, 3) = -2. * near_ * far_ / (far_ - near_);
  mat(3, 2) = -1.;

  return mat;
}

Eigen::Matrix4d Camera::Ortho() const
{
  double d = (eye_ - center_).norm();

  Eigen::Matrix4d mat = Eigen::Matrix4d::Identity();
  mat(0, 0) = 1. / (aspect_ * d);
  mat(1, 1) = 1. / d;
  mat(2, 2) = -2. / (far_ - near_);
  mat(2, 3) = -(far_ + near_) / (far_ - near_);

  return mat;
}
}
