#include "planner/config/config_loader.h"

#include <Eigen/Dense>

#include "json/json_loader.h"

namespace planner
{
std::shared_ptr<Config> ConfigLoader::LoadConfigFromFile(const std::string& filename)
{
  JsonLoader json_loader;
  Json json{json_loader.loadJson(filename)};

  std::vector<std::string> navigation_joints;
  std::vector<std::string> body_joints;
  std::vector<std::string> gripper_joints;

  auto robot_urdf = json["robot_urdf"].toString();
  auto robot_package_directory = json["robot_package_directory"].toString();
  auto environment_filename = json["environment_filename"].toString();

  auto json_navigation = json["navigation"];
  auto& json_navigation_joints = json_navigation["joints"].toArray();
  for (auto joint : json_navigation_joints)
    navigation_joints.push_back(joint.toString());

  auto json_body = json["body"];
  auto& json_body_joints = json_body["joints"].toArray();
  for (auto joint : json_body_joints)
    body_joints.push_back(joint.toString());

  auto json_gripper = json["gripper"];
  auto& json_gripper_joints = json_gripper["joints"].toArray();
  for (auto joint : json_gripper_joints)
    gripper_joints.push_back(joint.toString());

  auto gripping_position = json_gripper["gripping_position"];
  auto gripping_position_link = gripping_position["link"].toString();
  auto gripping_position_width = gripping_position["width"].toDouble();

  auto config = std::make_shared<Config>();
  config->SetRobotUrdf(robot_urdf);
  config->SetRobotPackageDirectory(robot_package_directory);
  config->SetEnvironmentFilename(environment_filename);
  config->SetNavigationJoints(navigation_joints);
  config->SetBodyJoints(body_joints);
  config->SetGripper(gripper_joints, gripping_position_link, gripping_position_width);

  auto json_default_joint_values = json["default_joint_values"];
  for (auto pair : json_default_joint_values.toObject())
    config->AddDefaultJointValues(pair.first, pair.second->toDouble());
  return config;
}
}
