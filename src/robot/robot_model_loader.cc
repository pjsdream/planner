#include "robot/robot_model_loader.h"

#include <tinyxml2/tinyxml2.h>

#include "tinyxml.h"

namespace robot
{
namespace
{
static Eigen::Affine3d ReadOrigin(const tinyxml2::XMLElement* element)
{
  Eigen::Affine3d origin = Eigen::Affine3d::Identity();

  const auto* origin_element = element->FirstChildElement("origin");
  if (origin_element)
  {
    double values[6] = {0., };

    if (origin_element->FindAttribute("xyz"))
      sscanf(origin_element->Attribute("xyz"), "%lf%lf%lf", &values[0], &values[1], &values[2]);

    if (origin_element->FindAttribute("rpy"))
      sscanf(origin_element->Attribute("rpy"), "%lf%lf%lf", &values[3], &values[4], &values[5]);

    origin.translate(Eigen::Vector3d(values[0], values[1], values[2]));
    origin.rotate(Eigen::AngleAxisd(values[3], Eigen::Vector3d(0., 0., 1.)));
    origin.rotate(Eigen::AngleAxisd(values[4], Eigen::Vector3d(0., 1., 0.)));
    origin.rotate(Eigen::AngleAxisd(values[5], Eigen::Vector3d(1., 0., 0.)));
  }

  return origin;
}

static RobotJoint::JointType GetJointTypeFromString(const std::string& type_string)
{
  if (type_string == "revolute")
    return RobotJoint::JointType::REVOLUTE;
  else if (type_string == "prismatic")
    return RobotJoint::JointType::PRISMATIC;
  else if (type_string == "continuous")
    return RobotJoint::JointType::CONTINUOUS;
  else if (type_string == "fixed")
    return RobotJoint::JointType::FIXED;
  else // TODO: add exception handling for unknown type string
    return RobotJoint::JointType::FIXED;
}
}

std::shared_ptr<RobotModel> RobotModelLoader::LoadFromUrdfFile(const std::string& filename)
{
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
    return nullptr;

  const auto* robot_xml = doc.FirstChildElement();
  auto robot_name = robot_xml->Attribute("name");

  auto it = robot_xml->FirstChildElement();
  while (it != 0)
  {
    if (std::string("link") == it->Value())
    {
      std::string link_name = it->Attribute("name");
      auto link = std::make_shared<RobotLink>(link_name);

      if (auto inertial = it->FirstChildElement("inertial"))
      {
        auto origin = ReadOrigin(inertial);

        double mass = 0.;
        if (auto mass_element = inertial->FirstChildElement("mass"))
          sscanf(mass_element->Attribute("value"), "%lf", &mass);

        Eigen::Matrix3d inertia = Eigen::Matrix3d::Zero();
        if (auto inertia_element = inertial->FirstChildElement("inertia"))
        {
          double ixx, ixy, ixz, iyy, iyz, izz;
          sscanf(inertia_element->Attribute("ixx"), "%lf", &ixx);
          sscanf(inertia_element->Attribute("ixy"), "%lf", &ixy);
          sscanf(inertia_element->Attribute("ixz"), "%lf", &ixz);
          sscanf(inertia_element->Attribute("iyy"), "%lf", &iyy);
          sscanf(inertia_element->Attribute("iyz"), "%lf", &iyz);
          sscanf(inertia_element->Attribute("izz"), "%lf", &izz);

          inertia(0, 0) = ixx;
          inertia(0, 1) = inertia(1, 0) = ixy;
          inertia(0, 2) = inertia(2, 0) = ixz;
          inertia(1, 1) = iyy;
          inertia(1, 2) = inertia(2, 1) = iyz;
          inertia(2, 2) = izz;
        }

        link->SetInertial(origin, mass, inertia);
      }

      if (auto visual = it->FirstChildElement("visual"))
      {
        while (visual != 0)
        {
          auto origin = ReadOrigin(visual);

          std::string mesh_filename;
          if (auto geometry = visual->FirstChildElement("geometry"))
          {
            if (auto mesh = geometry->FirstChildElement("mesh"))
              mesh_filename = SubstitutePackageDirectory(mesh->Attribute("filename"));
          }

          bool has_color = false;
          Eigen::Vector4f color;
          if (auto material = visual->FirstChildElement("material"))
          {
            if (auto color_element = material->FirstChildElement("color"))
            {
              has_color = true;
              double r, g, b, a;
              sscanf(color_element->Attribute("rgba"), "%lf%lf%lf%lf", &r, &g, &b, &a);
              color = Eigen::Vector4f(r, g, b, a);
            }
          }

          if (has_color)
            link->AddVisual(origin, mesh_filename, color);
          else
            link->AddVisual(origin, mesh_filename);

          // Iterate to the next visual element
          visual = visual->NextSiblingElement("visual");
        }
      }

      if (auto collision = it->FirstChildElement("collision"))
      {
        while (collision != 0)
        {
          auto origin = ReadOrigin(collision);

          std::string mesh_filename;
          if (auto geometry = collision->FirstChildElement("geometry"))
          {
            if (auto mesh = geometry->FirstChildElement("mesh"))
              mesh_filename = SubstitutePackageDirectory(mesh->Attribute("filename"));
          }

          // add link collision
          link->AddCollision(origin, mesh_filename);

          // iterate to the next collision element
          collision = collision->NextSiblingElement("collision");
        }
      }

      links_[link_name] = link;
    }

    else if (std::string("joint") == it->Value())
    {
      std::string joint_name = it->Attribute("name");
      auto joint = std::make_shared<RobotJoint>(joint_name, GetJointTypeFromString(it->Attribute("type")));

      auto origin = ReadOrigin(it);
      joint->SetOrigin(origin);

      link_child_joint_names_[it->FirstChildElement("parent")->Attribute("link")].push_back(joint_name);
      joint_child_link_name_[joint_name] = it->FirstChildElement("child")->Attribute("link");

      Eigen::Vector3d axis(1., 0., 0.);
      if (auto axis_element = it->FirstChildElement("axis"))
        sscanf(axis_element->Attribute("xyz"), "%lf%lf%lf", &axis(0), &axis(1), &axis(2));
      joint->SetAxis(axis);

      double effort = 0.;
      double velocity = 0.;
      double lower = 0.;
      double upper = 0.;
      if (auto limit = it->FirstChildElement("limit"))
      {
        sscanf(limit->Attribute("effort"), "%lf", &effort);
        sscanf(limit->Attribute("velocity"), "%lf", &velocity);

        if (limit->FindAttribute("lower"))
          sscanf(limit->Attribute("lower"), "%lf", &lower);

        if (limit->FindAttribute("upper"))
          sscanf(limit->Attribute("upper"), "%lf", &upper);
      }
      joint->SetLimit(lower, upper);

      joints_[joint_name] = joint;
    }

    it = it->NextSiblingElement();
  }

  // Find base link
  std::unordered_map<std::string, int> link_indegrees;
  for (const auto& link : links_)
    link_indegrees[link.second->GetName()] = 0;
  for (const auto& child_link : joint_child_link_name_)
    link_indegrees[child_link.second]++;

  std::shared_ptr<RobotLink> base_link;
  for (const auto& link : links_)
  {
    if (link_indegrees[link.second->GetName()] == 0)
    {
      base_link = link.second;
      break;
    }
  }

  // Make connectivity
  ConstructTree(base_link);

  // Clean up resources for the next uses
  links_.clear();
  joints_.clear();
  link_child_joint_names_.clear();
  joint_child_link_name_.clear();

  return std::make_shared<RobotModel>(robot_name, base_link);
}

void RobotModelLoader::ConstructTree(const std::shared_ptr<RobotLink>& link)
{
  for (const auto& child_joint_name : link_child_joint_names_[link->GetName()])
  {
    const auto& child_joint = joints_[child_joint_name];
    const auto& child_link_name = joint_child_link_name_[child_joint_name];
    const auto& child_link = links_[child_link_name];

    link->AddChildJoint(child_joint);
    child_joint->SetParentLink(link);

    child_joint->SetChildLink(child_link);
    child_link->SetParentJoint(child_joint);

    ConstructTree(child_link);
  }
}

std::string RobotModelLoader::SubstitutePackageDirectory(const std::string& filename)
{
  if (filename.substr(0, 10) == "package://")
  {
    // TODO: check whether the file exists
    return package_directories_[0] + "/" + filename.substr(10);
  }
  else
    return filename;
}
}
