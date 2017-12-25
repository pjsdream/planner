#ifndef PLANNER_PLANNER_MOTION_H_
#define PLANNER_PLANNER_MOTION_H_

#include <vector>

#include <Eigen/Dense>

namespace planner
{
class RobotMotion
{
public:
  RobotMotion() = default;

  void SetNavigationJoints(const std::vector<std::string>& navigation_joints)
  {
    navigation_joints_ = navigation_joints;
  }

  const auto& GetNavigationJoints() const
  {
    return navigation_joints_;
  }

  void SetBodyJoints(const std::vector<std::string>& body_joints)
  {
    body_joints_ = body_joints;
  }

  const auto& GetBodyJoints() const
  {
    return body_joints_;
  }

  void SetGripper(const std::vector<std::string>& gripper_joints,
                  const std::string& gripping_position_link,
                  const Eigen::Vector3d& gripping_position_offset,
                  double gripper_width)
  {
    gripper_joints_ = gripper_joints;
    gripping_position_link_ = gripping_position_link;
    gripping_position_offset_ = gripping_position_offset;
    gripper_width_ = gripper_width;
  }

  const auto& GetGrippperJoints() const
  {
    return gripper_joints_;
  }

  const auto& GetGripingPositionLink() const
  {
    return gripping_position_link_;
  }

  const auto& GetGrippingPositionOffset() const
  {
    return gripping_position_offset_;
  }

  auto GetGripperWidth() const
  {
    return gripper_width_;
  }

private:
  std::vector<std::string> navigation_joints_;
  std::vector<std::string> body_joints_;
  std::vector<std::string> gripper_joints_;
  std::string gripping_position_link_;
  Eigen::Vector3d gripping_position_offset_;
  double gripper_width_ = 0.;
};
}

#endif // PLANNER_PLANNER_MOTION_H_
