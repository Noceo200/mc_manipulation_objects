#include "manipulation_object.h"
#include "config.h"

#include <RBDyn/parsers/urdf.h>
#include <mc_rtc/logging.h>

#include <algorithm>
#include <cctype>

namespace mc_robots
{

namespace
{

std::string lowerCopy(const std::string & value)
{
  std::string out = value;
  std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return out;
}

std::string descriptionRoot()
{
  return mc_rtc::OBJECTS_DESCRIPTION_PATH;
}

std::string variantRoot(const std::string & variant)
{
  return descriptionRoot() + "/" + lowerCopy(variant);
}

std::string urdfPath(const std::string & variant)
{
  return variantRoot(variant) + "/urdf/" + lowerCopy(variant) + ".urdf";
}

} // namespace

ManipulationObjectRobotModule::ManipulationObjectRobotModule(const std::string & variant)
: RobotModule(descriptionRoot(), variant, urdfPath(variant))
{
  rsdf_dir = variantRoot(variant) + "/rsdf";

  bool fixed = true;
  init(rbd::parsers::from_urdf_file(urdf_path, fixed));

  _default_attitude = {{1., 0., 0., 0., 0., 0., 0.}};
}

} // namespace mc_robots

extern "C"
{

ROBOT_MODULE_API void MC_RTC_ROBOT_MODULE(std::vector<std::string> & names)
{
  names = {"manip/Box", "manip/Cylinder", "manip/Sphere", "manip/wrench"};
}

ROBOT_MODULE_API void destroy(mc_rbdyn::RobotModule * ptr)
{
  delete ptr;
}

ROBOT_MODULE_API mc_rbdyn::RobotModule * create(const std::string & n)
{
  ROBOT_MODULE_CHECK_VERSION("mc_manipulation_objects")

  if(n == "manip/Box")
  {
    return new mc_robots::ManipulationObjectRobotModule("box");
  }
  else if(n == "manip/Cylinder")
  {
    return new mc_robots::ManipulationObjectRobotModule("cylinder");
  }
  else if(n == "manip/Sphere")
  {
    return new mc_robots::ManipulationObjectRobotModule("sphere");
  }
  else if(n == "manip/wrench")
  {
    return new mc_robots::ManipulationObjectRobotModule("wrench");
  }

  mc_rtc::log::error("mc_manipulation_objects module cannot create an object of type {}", n);
  return nullptr;
}

}