#ifndef PLANNER_CAMERA_CAMERA_H_
#define PLANNER_CAMERA_CAMERA_H_

#include <Eigen/Dense>

namespace renderer
{
class Camera
{
public:
  enum class ProjectionType : unsigned char
  {
    ORTHO,
    PERSPECTIVE,
  };

public:
  Camera();

  auto EyePosition() const noexcept
  {
    return eye_;
  }

  void SetAspect(double aspect) noexcept
  {
    aspect_ = aspect;
  }

  void SetFovy(double fovy) noexcept
  {
    fovy_ = fovy;
  }

  void SetPerspective() noexcept
  {
    projection_type_ = ProjectionType::PERSPECTIVE;
  }

  void SetOrtho() noexcept
  {
    projection_type_ = ProjectionType::ORTHO;
  }

  void SetSensitivityTranslation(double s)
  {
    sensitivity_translation_ = s;
  }

  void SetSensitivityRotation(double s)
  {
    sensitivity_rotation_ = s;
  }

  void SetSensitivityZoom(double s)
  {
    sensitivity_zoom_ = s;
  }

  void SetNearFar(double near, double far)
  {
    near_ = near;
    far_ = far;
  }

  void LookAt(const Eigen::Vector3d& eye, const Eigen::Vector3d& center)
  {
    eye_ = eye;
    center_ = center;
  }

  void LookAt(const Eigen::Vector3d& eye, const Eigen::Vector3d& center, const Eigen::Vector3d& up)
  {
    eye_ = eye;
    center_ = center;
    up_ = up;
  }

  void TranslatePixel(int dx, int dy);
  void RotatePixel(int dx, int dy);
  void ZoomPixel(int dx, int dy);

  Eigen::Matrix4d ProjectionMatrix() const;
  Eigen::Matrix4d ViewMatrix() const;

private:
  Eigen::Vector3d eye_;
  Eigen::Vector3d up_;
  Eigen::Vector3d center_;
  double fovy_;
  double aspect_;
  double near_;
  double far_;
  ProjectionType projection_type_;

  double sensitivity_translation_;
  double sensitivity_rotation_;
  double sensitivity_zoom_;

  Eigen::Matrix4d Perspective() const;
  Eigen::Matrix4d Ortho() const;
};
}

#endif // PLANNER_CAMERA_CAMERA_H_
