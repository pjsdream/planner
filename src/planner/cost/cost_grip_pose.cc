#include "planner/cost/cost_grip_pose.h"

#include <iostream>

namespace planner
{
namespace
{
double M_PI = 3.1415926535897932384626433832795;
}

double CostGripPose::GetCost() const
{
  auto state = GetRobotState();
  auto ee_pose = state->GetEndeffectorLinkPose();
  // Rotate endeffector pose
  ee_pose.rotate(Eigen::AngleAxisd(-M_PI / 2., Eigen::Vector3d(0., 1., 0.)));

  Eigen::Vector3d offset = ee_pose.translation() - pose_.translation();
  auto offset_squared_norm = offset.squaredNorm();

  auto ee_orientation = ee_pose.linear();
  auto pose_orientation = pose_.linear();
  auto orientation = ee_orientation.transpose() * pose_orientation;
  Eigen::Vector2d x = orientation.block(0, 0, 2, 1);

  double angular_difference_z = std::acos(orientation(2, 2));
  double angular_difference_x = std::atan2(x(1), x(0));

  double angular_difference = 0.;
  double angular_difference_plane = 0.;
  switch (type_)
  {
  case GripType::CUBE:
  {
    angular_difference_plane = std::fmod(angular_difference_x, M_PI / 2.);
    if (angular_difference_plane < 0.)
      angular_difference_plane += M_PI / 2.;
    angular_difference_plane = std::min(angular_difference_plane, M_PI / 2. - angular_difference_plane);
  }
  break;

  case GripType::BOOK:
  {
    angular_difference_plane = std::fmod(angular_difference_x, M_PI);
    if (angular_difference_plane < 0.)
      angular_difference_plane += M_PI;
    angular_difference_plane = std::min(angular_difference_plane, M_PI - angular_difference_plane);
  }
  break;

  case GripType::CYLINDER:
    angular_difference_plane = 0.;
    break;
  }

  angular_difference = angular_difference_z * angular_difference_z + angular_difference_plane * angular_difference_plane;

  return GetWeight() * (offset_squared_norm + angular_weight_ * angular_difference);
}
}
