mc_manipulation_objects
=======

This package provides a self-contained RobotModule implementation for simple manipulation objects.

Variants
--------

- `manip/Box`
- `manip/Cylinder`
- `manip/Sphere`

Dependencies
------------

This package requires:
- [mc_rtc](https://github.com/jrl-umi3218/mc_rtc)

How to use
------------

Put the following in `mc_rtc.yaml`.
```yaml
MainRobot: manip/Box
Timestep: 0.002
```

The corresponding URDF, MJCF XML, RSDF, and STL assets live under `descriptions/<variant>/`.

Adding a new object manually
----------------------------

To add a new object variant, start from one of the existing ones and rename it consistently across the module.

0. You can get free STL meshes [here](https://cults3d.com/) and edit origin and scale using Blender [Blender](https://www.blender.org/)
1. Duplicate one of the existing variant folders under `descriptions/`, for example copy `descriptions/box/` to `descriptions/cube/`.
2. Keep the same internal layout inside the new folder: `urdf/`, `pdgains/`, `xml/`, `rsdf/`, and `meshes/`.
3. Rename the files inside that folder so they match the new variant name, for example `cube.urdf`, `cube.xml`, and `cube.rsdf`.
4. Edit the URDF link name, MJCF XML model name, and RSDF surface names so they all refer to the same object.
5. Adjust the primitive geometry or mesh reference to match the new shape.
6. Update `src/manipulation_object.cpp` to register the new variant name in `MC_RTC_ROBOT_MODULE` and in the `create()` switch. The public name should follow the `manip/<Variant>` pattern, while the description directory stays `descriptions/<variant>/`.
7. Add your new model name in the set of supported model in `CMakeLists.txt`
8. Rebuild and load the module with `MainRobot: manip/<YourNewVariant>` in `mc_rtc.yaml`.
