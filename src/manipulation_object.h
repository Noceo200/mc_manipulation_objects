#pragma once

#include <mc_rbdyn/RobotModule.h>
#include <mc_rbdyn/RobotModuleMacros.h>
#include <mc_robots/api.h>

namespace mc_robots
{

struct MC_ROBOTS_DLLAPI ManipulationObjectRobotModule : public mc_rbdyn::RobotModule
{
  explicit ManipulationObjectRobotModule(const std::string & variant);
};

} // namespace mc_robots