#ifndef PLANNER_ROBOT_SCREW_ROBOT_MODEL_H_
#define PLANNER_ROBOT_SCREW_ROBOT_MODEL_H_

#include <vector>

#include "planner/robot/screw_motion.h"

namespace planner
{
class ScrewRobotModel
{
public:
  struct Link
  {
    struct Visual
    {
      std::string filename;
      Eigen::Affine3d transform;
    };

    struct Collision
    {
      std::string filename;
      Eigen::Affine3d transform;
    };

    std::vector<Visual> visuals;
    std::vector<Collision> collisions;
  };

public:
  ScrewRobotModel() = delete;

  ScrewRobotModel(int num_joints);

  auto GetNumJoints() const
  {
    return num_joints_;
  }

  void AddVisual(int link_index, const std::string& filename, const Eigen::Affine3d& transform);
  void AddCollision(int link_index, const std::string& filename, const Eigen::Affine3d& transform);

  void SetScrewMotion(int joint_index, const ScrewMotion& screw_motion);

  void SetEndeffectorTransform(const Eigen::Affine3d& transform)
  {
    endeffector_transform_ = transform;
  }

  const auto& GetScrewMotion(int joint_index) const
  {
    return screw_motions_[joint_index];
  }

  const auto& GetEndeffectorTransform() const
  {
    return endeffector_transform_;
  }

  const auto& GetVisuals(int link_index) const
  {
    return links_[link_index].visuals;
  }

  const auto& GetCollisions(int link_index) const
  {
    return links_[link_index].visuals;
  }

private:
  int num_joints_;
  std::vector<Link> links_;
  std::vector<ScrewMotion> screw_motions_;
  Eigen::Affine3d endeffector_transform_;
};
}

#endif // PLANNER_ROBOT_SCREW_ROBOT_MODEL_H_
