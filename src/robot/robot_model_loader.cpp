#include "robot/robot_model_loader.h"

#include <tinyxml2/tinyxml2.h>
#include <Eigen/Dense>

#include "robot/robot_link.h"
#include "robot/robot_joint.h"

namespace simplan
{
namespace
{
void ParseOriginElement(tinyxml2::XMLElement* element, Eigen::Vector3d& xyz, Eigen::Vector3d& rpy)
{
  xyz.setZero();
  rpy.setZero();

  auto origin_element = element->FirstChildElement("origin");
  if (origin_element == nullptr)
    return;

  sscanf(origin_element->Attribute("xyz", "0 0 0"), "%lf%lf%lf", &xyz(0), &xyz(1), &xyz(2));
  sscanf(origin_element->Attribute("rpy", "0 0 0"), "%lf%lf%lf", &rpy(0), &rpy(1), &rpy(2));
}

auto ParseGeometryElement(tinyxml2::XMLElement* element)
{
  RobotLink::Geometry geometry;

  auto geometry_element = element->FirstChildElement("geometry");
  auto shape_element = geometry_element->FirstChildElement();
  if (strcmp(shape_element->Name(), "box") == 0)
  {
    Eigen::Vector3d size;
    sscanf(shape_element->Attribute("size"), "%lf%lf%lf", &size(0), &size(1), &size(2));

    geometry.type = RobotLink::Geometry::GeometryType::BOX;
    geometry.box_size[0] = size(0);
    geometry.box_size[1] = size(1);
    geometry.box_size[2] = size(2);
  }
  else if (strcmp(shape_element->Name(), "cylinder") == 0)
  {
    double radius = shape_element->DoubleAttribute("radius");
    double length = shape_element->DoubleAttribute("length");

    geometry.type = RobotLink::Geometry::GeometryType::CYLINDER;
    geometry.cylinder_radius = radius;
    geometry.cylinder_length = length;
  }
  else if (strcmp(shape_element->Name(), "sphere") == 0)
  {
    double radius = shape_element->DoubleAttribute("radius");

    geometry.type = RobotLink::Geometry::GeometryType::SPHERE;
    geometry.sphere_radius = radius;
  }
  else if (strcmp(shape_element->Name(), "mesh") == 0)
  {
    auto filename = shape_element->Attribute("filename");
    // TODO: scale

    geometry.type = RobotLink::Geometry::GeometryType::MESH;
    geometry.mesh_filename = filename;
  }

  return geometry;
}

auto ConvertXyzrpyToAffine(const Eigen::Vector3d& xyz, const Eigen::Vector3d& rpy)
{
  Eigen::Affine3d affine;

  affine.rotate(Eigen::AngleAxisd(rpy(2), Eigen::Vector3d(0., 0., 1.)));
  affine.rotate(Eigen::AngleAxisd(rpy(1), Eigen::Vector3d(0., 1., 0.)));
  affine.rotate(Eigen::AngleAxisd(rpy(0), Eigen::Vector3d(1., 0., 0.)));
  affine.translate(xyz);

  return affine;
}
}

RobotModelLoader::RobotModelLoader()
{
}

std::shared_ptr<RobotModel> RobotModelLoader::LoadFromFile(const std::string& filename)
{
  tinyxml2::XMLDocument doc;
  doc.LoadFile(filename.c_str());

  auto robot_element = doc.FirstChildElement("robot");
  auto robot_name = robot_element->Attribute("name");

  auto robot = std::make_shared<RobotModel>();
  robot->SetName(robot_name);

  // Links
  auto link_element = robot_element->FirstChildElement("link");
  while (link_element != nullptr)
  {
    RobotLink link;

    auto link_name = link_element->Attribute("name");
    link.SetName(link_name);

    auto inertial_element = link_element->FirstChildElement("inertial");
    if (inertial_element != nullptr)
    {
      Eigen::Vector3d xyz, rpy;
      ParseOriginElement(inertial_element, xyz, rpy);

      double mass = inertial_element->FirstChildElement("mass")->DoubleAttribute("value");

      double ixx, ixy, ixz, iyy, iyz, izz;
      auto inertia_element = inertial_element->FirstChildElement("inertia");
      ixx = inertia_element->DoubleAttribute("ixx");
      ixy = inertia_element->DoubleAttribute("ixy");
      ixz = inertia_element->DoubleAttribute("ixz");
      iyy = inertia_element->DoubleAttribute("iyy");
      iyz = inertia_element->DoubleAttribute("iyz");
      izz = inertia_element->DoubleAttribute("izz");

      Eigen::Matrix3d inertia;
      inertia << ixx, ixy, ixz, ixy, iyy, iyz, ixz, iyz, izz;
      link.SetInertial(ConvertXyzrpyToAffine(xyz, rpy), mass, inertia);
    }

    auto visual_element = link_element->FirstChildElement("visual");
    while (visual_element != nullptr)
    {
      RobotLink::Visual visual;

      auto visual_name = visual_element->Attribute("name");

      Eigen::Vector3d xyz, rpy;
      ParseOriginElement(visual_element, xyz, rpy);

      auto geometry = ParseGeometryElement(visual_element);

      auto material_element = visual_element->FirstChildElement("material");
      if (material_element != nullptr)
      {
        auto material_name = material_element->Attribute("name", "");

        if (material_element->Attribute("color"))
        {
          Eigen::Vector4d color;
          sscanf(material_element->Attribute("color"), "%lf%lf%lf%lf", &color(0), &color(1), &color(2), &color(3));

          visual.has_color = true;
          visual.color = color;
        }
        else
          visual.has_color = false;

        auto texture_element = material_element->FirstChildElement("texture");
        if (texture_element != nullptr)
        {
          auto filename = texture_element->Attribute("filename");

          visual.texture_filename = filename;
        }
      }

      visual.origin = ConvertXyzrpyToAffine(xyz, rpy);
      visual.geometry = geometry;

      link.AddVisual(visual);

      visual_element = visual_element->NextSiblingElement("visual");
    }

    auto collision_element = link_element->FirstChildElement("collision");
    if (collision_element != nullptr)
    {
      RobotLink::Collision collision;

      auto collision_name = collision_element->Attribute("name");

      Eigen::Vector3d xyz, rpy;
      ParseOriginElement(collision_element, xyz, rpy);

      auto geometry = ParseGeometryElement(collision_element);

      collision.origin = ConvertXyzrpyToAffine(xyz, rpy);
      collision.geometry = geometry;

      link.AddCollision(collision);

      collision_element = collision_element->NextSiblingElement("collision");
    }

    link_element = link_element->NextSiblingElement("link");
  }

  // Joints
  auto joint_element = robot_element->FirstChildElement("joint");
  while (joint_element != nullptr)
  {
    RobotJoint joint;

    auto joint_name = joint_element->Attribute("name");
    auto joint_type = joint_element->Attribute("type");

    Eigen::Vector3d xyz, rpy;
    ParseOriginElement(joint_element, xyz, rpy);

    auto parent_link = joint_element->FirstChildElement("parent")->Attribute("link");
    auto child_link = joint_element->FirstChildElement("child")->Attribute("link");

    Eigen::Vector3d axis(1., 0., 0.);
    auto axis_element = joint_element->FirstChildElement("axis");
    if (axis_element != nullptr)
      sscanf(axis_element->Attribute("xyz"), "%lf%lf%lf", &axis(0), &axis(1), &axis(2));

    // TODO: calibration

    // TODO: dynamics

    if (strcmp(joint_type, "revolute") == 0 || strcmp(joint_type, "prismatic") == 0)
    {
      auto limit_element = joint_element->FirstChildElement("limit");
      double lower = limit_element->DoubleAttribute("lower", 0.);
      double upper = limit_element->DoubleAttribute("upper", 0.);
      double effort = limit_element->DoubleAttribute("effort");
      double velocity = limit_element->DoubleAttribute("velocity");

      joint.SetLimit(lower, upper, effort, velocity);
    }

    // TODO: mimic

    // TODO: safety controller

    joint.SetName(joint_name);

    if (strcmp(joint_type, "revolute") == 0)
      joint.SetType(RobotJoint::JointType::REVOLUTE);
    else if (strcmp(joint_type, "continuous") == 0)
      joint.SetType(RobotJoint::JointType::CONTINUOUS);
    else if (strcmp(joint_type, "prismatic") == 0)
      joint.SetType(RobotJoint::JointType::PRISMATIC);
    else if (strcmp(joint_type, "fixed") == 0)
      joint.SetType(RobotJoint::JointType::FIXED);

    joint.SetParentLink(parent_link);
    joint.SetChildLink(child_link);

    joint_element = joint_element->NextSiblingElement("joint");
  }

  return robot;
}
}
