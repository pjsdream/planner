#ifndef SIMPLAN_ROBOT_ROBOT_JOINT_H_
#define SIMPLAN_ROBOT_ROBOT_JOINT_H_

#include <string>

#include <Eigen/Dense>

namespace simplan
{
class RobotJoint
{
public:
  enum class JointType
  {
    UNDEFINED,
    REVOLUTE,
    CONTINUOUS,
    PRISMATIC,
    FIXED,
    // TODO: floating, planar
  };

public:
  RobotJoint();

  void SetType(JointType type)
  {
    type_ = type;
  }

  void SetName(const std::string& name)
  {
    name_ = name;
  }

  const auto& GetName() const
  {
    return name_;
  }

  void SetChildLink(const std::string& link)
  {
    child_link_ = link;
  }

  const auto& GetChildLink()
  {
    return child_link_;
  }

  void SetParentLink(const std::string& link)
  {
    parent_link_ = link;
  }

  const auto& GetParentLink()
  {
    return parent_link_;
  }

  void SetLimit(double lower, double upper, double effort, double velocity)
  {
    lower_ = lower;
    upper_ = upper;
    effort_ = effort;
    velocity_ = velocity;
  }

  const auto& lower() const
  {
    return lower_;
  }

  const auto& upper() const
  {
    return upper_;
  }

  const auto& effort() const
  {
    return effort_;
  }

  const auto& velocity() const
  {
    return velocity_;
  }

private:
  JointType type_;
  std::string name_;
  std::string child_link_;
  std::string parent_link_;

  Eigen::Vector3d axis_;

  double lower_;
  double upper_;
  double effort_;
  double velocity_;
};
}

#endif // SIMPLAN_ROBOT_ROBOT_JOINT_H_
