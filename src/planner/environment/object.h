#ifndef PLANNER_PLANNER_ENVIRONMENT_OBJECT_H_
#define PLANNER_PLANNER_ENVIRONMENT_OBJECT_H_

#include <Eigen/Dense>

#include "shape/shape.h"

namespace planner
{
class Object
{
public:
  void SetName(const std::string& name)
  {
    name_ = name;
  }

  const auto& GetName() const
  {
    return name_;
  }

  void SetColor(const Eigen::Vector4f& color)
  {
    color_ = color;
  }

  const Eigen::Vector4f& GetColor() const
  {
    return color_;
  }

  auto GetColor()
  {
    return color_;
  }

  void SetShape(const std::shared_ptr<shape::Shape>& shape)
  {
    shape_ = shape;
  }

  auto GetShape() const
  {
    return shape_;
  }

  void SetTransform(const Eigen::Affine3d& transform)
  {
    transform_ = transform;
  }

  const auto& GetTransform() const
  {
    return transform_;
  }

private:
  std::string name_;
  Eigen::Vector4f color_;
  std::shared_ptr<shape::Shape> shape_;
  Eigen::Affine3d transform_;
};
}

#endif // PLANNER_PLANNER_ENVIRONMENT_OBJECT_H_
