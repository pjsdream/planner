#ifndef SIMPLAN_ROBOT_ROBOT_LINK_H_
#define SIMPLAN_ROBOT_ROBOT_LINK_H_

#include <string>
#include <vector>

#include <Eigen/Dense>

namespace simplan
{
class RobotLink
{
public:
  struct Inertial
  {
    Eigen::Affine3d origin;
    double mass;
    Eigen::Matrix3d inertia;
  };

  struct Geometry
  {
    enum class GeometryType
    {
      BOX,
      CYLINDER,
      SPHERE,
      MESH,
    };
    GeometryType type = GeometryType::MESH;

    union
    {
      double box_size[3];
      struct
      {
        double cylinder_radius;
        double cylinder_length;
      };
      struct
      {
        double sphere_radius;
      };
    };

    std::string mesh_filename;
  };

  struct Visual
  {
    Eigen::Affine3d origin;
    Geometry geometry;
    bool has_color;
    Eigen::Vector4d color;
    std::string texture_filename;
  };

  struct Collision
  {
    Eigen::Affine3d origin;
    Geometry geometry;
  };

public:
  RobotLink();

  void SetName(const std::string& name)
  {
    name_ = name;
  }

  const auto& GetName() const
  {
    return name_;
  }

  void SetInertial(const Eigen::Affine3d& origin, double mass, const Eigen::Matrix3d& inertia)
  {
    inertial_.origin = origin;
    inertial_.mass = mass;
    inertial_.inertia = inertia;
  }

  const auto& GetInertial() const
  {
    return inertial_;
  }

  void AddVisual(const Visual& visual)
  {
    visuals_.push_back(visual);
  }

  void AddCollision(const Collision& collision)
  {
    collisions_.push_back(collision);
  }

private:
  std::string name_;

  Inertial inertial_;
  std::vector<Visual> visuals_;
  std::vector<Collision> collisions_;
};
}

#endif // SIMPLAN_ROBOT_ROBOT_LINK_H_
