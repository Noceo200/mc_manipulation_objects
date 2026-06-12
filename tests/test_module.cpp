#include <mc_rbdyn/RobotLoader.h>
#include <mc_rtc/logging.h>

#include <array>

int main(int argc, char * argv[])
{
  if(argc < 2)
  {
    mc_rtc::log::critical("Usage: {} [MODULE_DIR]", argv[0]);
    return 1;
  }
  mc_rbdyn::RobotLoader::clear();
  mc_rbdyn::RobotLoader::update_robot_module_path({argv[1]});

  const std::array<std::string, 3> moduleNames = {{"manip/Box", "manip/Cylinder", "manip/Sphere"}};
  for(const auto & moduleName : moduleNames)
  {
    auto rm = mc_rbdyn::RobotLoader::get_robot_module(moduleName);
    if(!rm)
    {
      mc_rtc::log::error("Failed to load {}", moduleName);
      return 1;
    }
    mc_rtc::log::info("{} has {} dof", moduleName, rm->mb.nrDof());
    if(rm->mb.nrDof() != 0)
    {
      mc_rtc::log::error("{} unexpectedly has {} dof", moduleName, rm->mb.nrDof());
      return 1;
    }
  }

  return 0;
}
