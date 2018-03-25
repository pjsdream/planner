#ifndef SIMPLAN_RENDERING_CAMERA_H_
#define SIMPLAN_RENDERING_CAMERA_H_

#include <Eigen/Dense>

namespace simplan
{
class Camera
{
private:
  enum class ProjectionType
  {
    ORTHO,
    PERSPECTIVE,
  };

public:
  Camera();

  void SetAspect(double aspect)
  {
    aspect_ = aspect;
  }

  void SetFovy(double fovy)
  {
    fovy_ = fovy;
  }

  void SetPerspective()
  {
    projection_type_ = ProjectionType::PERSPECTIVE;
  }

  void SetOrtho()
  {
    projection_type_ = ProjectionType::ORTHO;
  }

  void SetNear(double Near)
  {
    near_ = Near;
  }

  void SetFar(double Near)
  {
    near_ = Near;
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

  void TranslateByPixel(int dx, int dy);
  void RotateByPixel(int dx, int dy);
  void ZoomByPixel(int dx, int dy);

  Eigen::Matrix4d ProjectionMatrix() const;
  Eigen::Matrix4d ViewMatrix() const;

private:
  Eigen::Matrix4d Ortho() const;
  Eigen::Matrix4d Perspective() const;

  Eigen::Vector3d eye_;
  Eigen::Vector3d up_;
  Eigen::Vector3d center_;

  double translation_speed_;
  double rotation_speed_;
  double zoom_speed_;

  double fovy_;
  double aspect_;
  double near_;
  double far_;

  ProjectionType projection_type_;
};
}

#endif // SIMPLAN_RENDERING_CAMERA_H_
