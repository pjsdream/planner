#ifndef PLANNER_ROBOT_SCREW_MOTION_H_
#define PLANNER_ROBOT_SCREW_MOTION_H_

#include <Eigen/Dense>

namespace planner
{
class ScrewMotion
{
private:
  static Eigen::Matrix3d Outer(Eigen::Vector3d w)
  {
    Eigen::Matrix3d m;
    m << 0., -w(2), w(1),
      w(2), 0., -w(0),
      -w(1), w(0), 0.;
    return m;
  }

  static Eigen::Vector3d Log(Eigen::Matrix3d R)
  {
    constexpr double eps = 1e-5;

    if (R(0, 0) > -1. + eps)
      return 1. / (std::sqrt(2. * (1. + R(0, 0)))) * Eigen::Vector3d(1. + R(0, 0), R(1, 0), R(2, 0));
    else if (R(1, 1) > -1. + eps)
      return 1. / (std::sqrt(2. * (1. + R(1, 1)))) * Eigen::Vector3d(R(0, 1), 1. + R(1, 1), R(2, 1));
    else
      return 1. / (std::sqrt(2. * (1. + R(2, 2)))) * Eigen::Vector3d(R(0, 2), R(1, 2), 1. + R(2, 2));
  }

public:
  static Eigen::Matrix<double, 6, 6> AdjointMatrix(const Eigen::Matrix3d& R, const Eigen::Vector3d& p)
  {
    Eigen::Matrix<double, 6, 6> ad;
    ad << R, Eigen::Matrix3d::Zero(), Outer(p) * R, R;
    return ad;
  }

public:
  ScrewMotion()
  {
    s_.setZero();
  }

  ScrewMotion(const Eigen::Vector3d& w, const Eigen::Vector3d& v)
  {
    s_.block(0, 0, 3, 1) = w;
    s_.block(3, 0, 3, 1) = v;
  }

  ScrewMotion(const Eigen::Matrix<double, 6, 1>& s)
  {
    s_ = s;
  }

  ScrewMotion(const Eigen::Affine3d& transform)
  {
    constexpr double eps = 1e-5;

    const double trace = transform.linear().trace();
    if (trace >= 3. - eps)
    {
      s_.block(0, 0, 3, 1).setZero();
      s_.block(3, 0, 3, 1) = transform.translation();
    }
    else
    {
      Eigen::Vector3d w, v;
      double theta;
      if (trace <= -1. + eps)
      {
        theta = std::acos(-1.);
        w = Log(transform.linear());
      }
      else
      {
        theta = std::acos((trace - 1.) / 2.);
        Eigen::Matrix3d w_bracket = 1 / (2. * std::sin(theta)) * (transform.linear() - transform.linear().transpose());
        w(0) = w_bracket(2, 1);
        w(1) = w_bracket(0, 2);
        w(2) = w_bracket(1, 0);
      }

      v = ((1. / theta) * Eigen::Matrix3d::Identity() + 0.5 * Outer(w) + (1. / theta - 0.5 * (1. / std::tan(theta / 2.))) * Outer(w) * Outer(w)) * transform.translation();
      
      s_.block(0, 0, 3, 1) = w;
      s_.block(3, 0, 3, 1) = v;
    }
  }

  Eigen::Vector3d W() const
  {
    return s_.block(0, 0, 3, 1);
  }

  Eigen::Vector3d V() const
  {
    return s_.block(3, 0, 3, 1);
  }

  Eigen::Matrix4d Bracket() const
  {
    Eigen::Matrix4d m;
    m << Outer(W()), V(),
      0., 0., 0., 0.;
    return m;
  }

  ScrewMotion Adjoint(const Eigen::Matrix3d& R, const Eigen::Vector3d& p) const
  {
    return ScrewMotion(AdjointMatrix(R, p) * s_);
  }

  ScrewMotion Adjoint(const Eigen::Affine3d& transform) const
  {
    return ScrewMotion(AdjointMatrix(transform.linear(), transform.translation()) * s_);
  }

  Eigen::Matrix4d Exponential(double theta) const
  {
    Eigen::Matrix4d m;
    const double sin = std::sin(theta);
    const double cos = std::cos(theta);
    const auto w_outer = Outer(W());
    const auto w_outer_squared = w_outer * w_outer;

    m.block(0, 0, 3, 3) = Eigen::Matrix3d::Identity() + sin * w_outer + (1. - cos) * w_outer_squared;
    m.block(0, 3, 3, 1) = (Eigen::Matrix3d::Identity() * theta + (1. - cos) * w_outer + (theta - sin) * w_outer_squared) * V();
    m.block(3, 0, 1, 3).setZero();
    m(3, 3) = 1.;

    return m;
  }

  const auto& Vector() const
  {
    return s_;
  }

private:
  Eigen::Matrix<double, 6, 1> s_;
};
}

#endif // PLANNER_ROBOT_SCREW_MOTION_H_