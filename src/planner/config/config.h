#ifndef PLANNER_PLANNER_CONFIG_CONFIG_H_
#define PLANNER_PLANNER_CONFIG_CONFIG_H_

#include <vector>
#include <unordered_map>

#include <Eigen/Dense>

namespace planner
{
class Config
{
public:
  Config() = default;

  void SetRobotUrdf(const std::string& urdf)
  {
    robot_urdf_ = urdf;
  }

  const auto& GetRobotUrdf()
  {
    return robot_urdf_;
  }

  void SetRobotPackageDirectory(const std::string& directory)
  {
    robot_package_directory_ = directory;
  }

  const auto& GetRobotPackageDirectory()
  {
    return robot_package_directory_;
  }

  void SetEnvironmentFilename(const std::string& filename)
  {
    environment_filename_ = filename;
  }

  const auto& GetEnvironmentFilename()
  {
    return environment_filename_;
  }

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
                  double gripper_width)
  {
    gripper_joints_ = gripper_joints;
    gripping_position_link_ = gripping_position_link;
    gripper_width_ = gripper_width;
  }

  const auto& GetGripperJoints() const
  {
    return gripper_joints_;
  }

  const auto& GetGrippingPositionLink() const
  {
    return gripping_position_link_;
  }

  auto GetGripperWidth() const
  {
    return gripper_width_;
  }

  void AddDefaultJointValues(const std::string& joint_name, double joint_value)
  {
    default_joint_values_[joint_name] = joint_value;
  }

  const auto& GetDefaultJointValues() const
  {
    return default_joint_values_;
  }

private:
  std::string robot_urdf_;
  std::string robot_package_directory_;
  std::string environment_filename_;
  std::vector<std::string> navigation_joints_;
  std::vector<std::string> body_joints_;
  std::vector<std::string> gripper_joints_;
  std::unordered_map<std::string, double> default_joint_values_;
  std::string gripping_position_link_;
  double gripper_width_ = 0.;
};
}

#endif // PLANNER_PLANNER_CONFIG_CONFIG_H_
