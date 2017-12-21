#ifndef PLANNER_SHAPE_CUBE_H_
#define PLANNER_SHAPE_CUBE_H_

#include "shape/shape.h"

namespace shape
{
class Cube : public Shape
{
public:
  Cube()
      : Shape(), size_{1., 1., 1.}, transform_(Eigen::Affine3d::Identity())
  {
  }

  Cube(const Eigen::Vector3d& size)
      : Shape(), size_(size), transform_(Eigen::Affine3d::Identity())
  {
  }

  Cube(const Eigen::Vector3d& size, const Eigen::Affine3d& transform)
      : Shape(), size_(size), transform_(transform)
  {
  }

  ~Cube() override = default;

  bool IsCube() const override
  {
    return true;
  }

  const auto& GetSize() const
  {
    return size_;
  }

  const auto& GetTransform() const
  {
    return transform_;
  }

private:
  Eigen::Vector3d size_;
  Eigen::Affine3d transform_;
};
}

#endif // PLANNER_SHAPE_CUBE_H_
