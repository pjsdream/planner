#include "planner/motion/robot_motion_loader.h"

#include <Eigen/Dense>

#include "json/json_loader.h"

namespace planner
{
std::shared_ptr<RobotMotion> RobotMotionLoader::LoadRobotMotionFromFile(const std::string& filename)
{
  JsonLoader json_loader;
  Json json{json_loader.loadJson(filename)};

  std::vector<std::string> navigation_joints;
  std::vector<std::string> body_joints;
  std::vector<std::string> gripper_joints;

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
  auto gripping_position_offset = Eigen::Vector3d(gripping_position["xyz"][0].toDouble(),
                                                  gripping_position["xyz"][1].toDouble(),
                                                  gripping_position["xyz"][2].toDouble());
  auto gripping_position_width = gripping_position["width"].toDouble();

  auto motion = std::make_shared<RobotMotion>();
  motion->SetNavigationJoints(navigation_joints);
  motion->SetBodyJoints(body_joints);
  motion->SetGripper(gripper_joints, gripping_position_link, gripping_position_offset, gripping_position_width);
  return motion;
}
}
