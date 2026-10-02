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
Timestep: 0.001
```

The corresponding URDF, MJCF XML, RSDF, and STL assets live under `descriptions/<variant>/`.

Isaac Sim (mc_isaac)
--------------------

The package also installs mc_isaac (Isaac Sim simulation of mc_rtc controllers) descriptions (option
`INSTALL_ISAAC_FILES`, ON by default):

- `<prefix>/share/mc_isaac/<variant>.yaml`: one description per object (key = mc_rtc module name, e.g. `dice_red`),
  in the default mc_isaac description folder when installed alongside mc_rtc
  (`<CMAKE_INSTALL_PREFIX>/mc_isaac` with `HONOR_INSTALL_PREFIX`: add it to `description_paths` in the IsaacSim plugin
  configuration)
- `<prefix>/share/mc_isaac/manipulation_objects/<variant>/`: the USD of each object (from `descriptions/<variant>/usd/`)

| Module | USD | Mass [kg] | Collision | Source |
|---|---|---|---|---|
| `manip/Box` (`box`) | `box.usda` | 1.0 | convex hull | generated from `box.STL` |
| `manip/Cylinder` (`cylinder`) | `cylinder.usda` | 1.0 | convex hull | generated from `cylinder.STL` |
| `manip/Sphere` (`sphere`) | `sphere.usda` | 1.0 | convex hull | generated from `sphere.STL` |
| `manip/dice` (`dice`) | `dice.usda` | 0.018 | convex hull | generated from `dice.STL` |
| `manip/dice_red` (`dice_red`) | `dice_red.usd` + `textures/RedDiceTexture.png` | 0.018 | convex hull | IsaacLab asset `dice30_red.usd` (RL training) |
| `manip/dice_yellow` (`dice_yellow`) | `dice_yellow.usd` + `textures/YellowDiceTexture.png` | 0.018 | convex hull | IsaacLab asset `dice30_yellow.usd` (RL training) |
| `manip/wrench` (`wrench`) | `wrench.usd` | 0.745 | convex decomposition | IsaacLab asset `wrench.usd` (RL training) |
| `manip/wrench_holder` (`wrench_holder`) | `wrench_holder.usda` | 5.0 | triangle mesh (fixed, kinematic) | generated from `wrench_holder.STL` |

Masses match the MJCF models. Objects are simulated as single rigid bodies; fixed modules (`wrench_holder`) are
kinematic. mc_isaac feeds their pose/velocity to the `FloatingBase` body sensor (use a `BodySensor` observer).

The `.usda` files were generated with the `mc_isaac_mesh_to_usd` tool of mc_isaac, e.g.:

```bash
mc_isaac_mesh_to_usd --out descriptions/box/usd/box.usda --mass 1.0 --color 0.85 0.55 0.20 \
  --visual descriptions/box/meshes/box.STL
mc_isaac_mesh_to_usd --out descriptions/wrench_holder/usd/wrench_holder.usda --mass 5.0 --approximation none \
  --visual descriptions/wrench_holder/meshes/wrench_holder.STL
```

Files referenced by a USD (textures) must be in `descriptions/<variant>/usd/textures/`: they are listed in the
description (`extra_files`) and uploaded with the USD to the Isaac server.

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
9. For mc_isaac, add `descriptions/<variant>/usd/<variant>.usda` (e.g. generated with `mc_isaac_mesh_to_usd`) and
   its textures in `usd/textures/` if any.
