#ifndef PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_LINK_H_
#define PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_LINK_H_

#include <Eigen/Dense>

namespace planner
{
class LinearRobotLink
{
public:
  struct Visual
  {
    std::string filename;
    Eigen::Affine3d origin;
    Eigen::Vector4f color;
  };

public:
  LinearRobotLink() = default;

  void AddVisual(const std::string& filename, const Eigen::Affine3d& origin)
  {
    Visual visual;
    visual.filename = filename;
    visual.origin = origin;
    visual.color = Eigen::Vector4f(0.8f, 0.8f, 0.8f, 1.f);
    visuals_.push_back(visual);
  }

  void AddVisual(const std::string& filename, const Eigen::Affine3d& origin, const Eigen::Vector4f& color)
  {
    Visual visual;
    visual.filename = filename;
    visual.origin = origin;
    visual.color = color;
    visuals_.push_back(visual);
  }

  const auto& GetVisuals() const
  {
    return visuals_;
  }

private:
  std::vector<Visual> visuals_;
};
}

#endif // PLANNER_PLANNER_ROBOT_LINEAR_ROBOT_LINK_H_
