cmake_minimum_required(VERSION 3.22)
project(mc_manipulation_objects VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

# Supported models to install, if SRC_MODE is ON, all src files will be available anyway
  set(models
      box
      cylinder
      sphere
      wrench
      dice
  )

option(INSTALL_MUJOCO_FILES "Whether to install mc_mujoco configuration files" ON)
option(HONOR_INSTALL_PREFIX "Whether to install in CMAKE_INSTALL_PREFIX (for nix)" OFF)
option(SRC_MODE "Use files in the repository instead of installed files" OFF)
find_package(mc_rtc REQUIRED)

if(HONOR_INSTALL_PREFIX)
  set(MC_RTC_SHARE_DESTINATION "${CMAKE_INSTALL_PREFIX}")
else()
  set(MC_RTC_SHARE_DESTINATION "${MC_RTC_INSTALL_PREFIX}/share/mc_rtc/${PROJECT_NAME}")
endif()

set(MANIP_DESCRIPTION_PATH "${MC_RTC_SHARE_DESTINATION}/descriptions")
# Install description files
foreach(MODEL IN LISTS models)
      install(DIRECTORY descriptions/${MODEL}/ DESTINATION ${MC_RTC_SHARE_DESTINATION}/descriptions/${MODEL} FILES_MATCHING PATTERN "*.STL" PATTERN "*.stl" PATTERN "*.urdf" PATTERN "*.rsdf")
      set(MESH_DIR "${MANIP_DESCRIPTION_PATH}/${MODEL}/meshes")
      configure_file(descriptions/${MODEL}/urdf/${MODEL}.urdf.in ${MC_RTC_SHARE_DESTINATION}/descriptions/${MODEL}/urdf/${MODEL}.urdf)
endforeach()
configure_file(src/config.h.in "${CMAKE_CURRENT_BINARY_DIR}/include/config.h")


# Install Module
macro(add_module NAME)
  add_robot(${NAME} src/${NAME}.cpp src/${NAME}.h)
  target_include_directories(${NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
endmacro()

add_module(manipulation_object)

option(DISABLE_TESTS "Disable unit tests" OFF)
if(NOT ${DISABLE_TESTS})
  enable_testing()
  add_subdirectory(tests)
endif()

# test install without mc_mujoco to see if it install only specified models + works in mc_visualization

# mc_mujoco Installation
if(INSTALL_MUJOCO_FILES)
  if(HONOR_INSTALL_PREFIX)
    set(MC_MUJOCO_SHARE_DESTINATION "${CMAKE_INSTALL_PREFIX}/mc_mujoco")
  elseif(NOT TARGET mc_mujoco)
    find_package(mc_mujoco REQUIRED)
  endif()
  foreach(MODEL IN LISTS models)
      set(MODEL_FILE ${MODEL})
      if(SRC_MODE)
          set(MANIP_XML_MODEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/descriptions/${MODEL_FILE}/xml/${MODEL_FILE}.xml")
          set(MANIP_PDGAINS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/descriptions/${MODEL_FILE}/pdgains/PDgains_sim.dat")
      else()
          set(MANIP_XML_MODEL_PATH "${MC_MUJOCO_SHARE_DESTINATION}/manipulation_objects/${MODEL_FILE}/xml/${MODEL_FILE}.xml")
          set(MANIP_PDGAINS_PATH "${MC_MUJOCO_SHARE_DESTINATION}/manipulation_objects/${MODEL_FILE}/pdgains/PDgains_sim.dat")
      endif()
          
      configure_file(mc_mujoco/manipulation_objects.in.yaml "${CMAKE_CURRENT_BINARY_DIR}/mc_mujoco/${MODEL}.yaml")
      install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mc_mujoco/${MODEL}.yaml" DESTINATION "${MC_MUJOCO_SHARE_DESTINATION}")
      install(DIRECTORY descriptions/${MODEL_FILE} DESTINATION "${MC_MUJOCO_SHARE_DESTINATION}/manipulation_objects" FILES_MATCHING PATTERN "*.STL" PATTERN "*.stl" PATTERN "*.dat" PATTERN "*.xml")
  endforeach()

endif()