#include "planner/planner.h"
#include "planner/environment/environment_loader.h"
#include "robot/robot_model_loader.h"
#include "planner/robot/linear_robot_model_factory.h"

namespace planner
{
void Planner::SetConfig(const std::shared_ptr<Config>& config)
{
  EnvironmentLoader environment_loader;
  environment_ = environment_loader.LoadEnvironmentFromFile(config->GetEnvironmentFilename());

  // TODO: load robot model
  robot::RobotModelLoader robot_model_loader;
  robot_model_loader.AddPackageDirectory(config->GetRobotPackageDirectory());
  auto robot_model = robot_model_loader.LoadFromUrdfFile(config->GetRobotUrdf());
  auto body_joints = config->GetBodyJoints();

  LinearRobotModelFactory robot_model_factory(robot_model);
  for (auto body_joint : body_joints)
    robot_model_factory.AddActiveJoint(body_joint);
  for (auto pair : config->GetDefaultJointValues())
    robot_model_factory.SetJointValue(pair.first, pair.second);
  robot_model_factory.SetEndeffectorLink(config->GetGrippingPositionLink());
  robot_model_ = robot_model_factory.CreateRobotModel();
}

void Planner::SetInitialRobotState(const Eigen::VectorXd& state)
{
  path_->SetInitialPoint(state);
}

void Planner::SetInitialZeroRobotState()
{
  // TODO
  /*
  Eigen::VectorXd state(1);
  state.setZero();

  path_->SetInitialPoint(state);
  */
}
}
