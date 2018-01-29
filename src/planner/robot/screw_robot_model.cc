#include "planner/robot/screw_robot_model.h"

namespace planner
{
ScrewRobotModel::ScrewRobotModel(int num_joints)
  : num_joints_(num_joints), links_(num_joints + 1), screw_motions_(num_joints_)
{
}

void ScrewRobotModel::AddVisual(int link_index, const std::string& filename, const Eigen::Affine3d& transform)
{
  Link::Visual visual;
  visual.filename = filename;
  visual.transform = transform;
  links_[link_index].visuals.push_back(visual);
}

void ScrewRobotModel::AddCollision(int link_index, const std::string& filename, const Eigen::Affine3d& transform)
{
  Link::Collision collision;
  collision.filename = filename;
  collision.transform = transform;
  links_[link_index].collisions.push_back(collision);
}

void ScrewRobotModel::SetScrewMotion(int joint_index, const ScrewMotion& screw_motion)
{
  screw_motions_[joint_index] = screw_motion;
}
}
