# VTK/ParaView Template

This project contains examples and CMake infrastructure for using VTK or ParaView in the following ways:

*  Building a VTK module containing an VTK algorithm (`MyFilters/`),
*  Building a standalone that runs the VTK algorithm (`standalone/`),
*  Generating Python bindings for the VTK module (module `my_vtk_module`),
*  Building a ParaView plugin using the VTK module (`MyPlugin/`),
*  Implementing a VTK algorithm in pure Python (`python/my_python_module/`),
*  Loading the VTK Python algorithm as ParaView plugin (`python/plugin/load_python_plugins.py`),
*  Creating a VTK pipeline using the VTK algorithm and VTK Python algorithm (`python/standalone/example_pipeline.py`),
*  Using the two algorithms in ParaView (`python/standalone/example_pvpython.py`).

## Build

You need to build ParaView &ge; 5.8 or VTK &ge; 9.0 first. Follow instructions for [building ParaView](https://gitlab.kitware.com/paraview/paraview/-/blob/v5.9.0/Documentation/dev/build.md) or [building VTK](https://gitlab.kitware.com/vtk/vtk/-/blob/v9.0.1/Documentation/dev/build.md). Make sure, that you build with Python support enabled.

In the directory, where you cloned this repository, use CMake to build:
```bash
mkdir build
cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DParaView_DIR=[directory containing paraview-config.cmake]
ninja
```
You may also use VTK by setting `-DENABLE_PARAVIEW=OFF` and `-DVTK_DIR=[directory containing vtk-config.cmake]`.

## Running the Examples

After a successful build, the binaries are located in the `bin` and `lib` subfolders of your build directory.
To simplify usage, an `activate` script is generated during CMake configure. This script, similarly to a Python virtualenv, adds the required paths to your environment. Additionally to the build paths, the VTK/ParaView paths are also included.
```bash
source build/activate
```
Afterwards, e.g., `paraview` launches the ParaView binary that was used for building your plugin and automatically loads your plugins. You may also import the Python modules from `python` or `pvpython`.
The original environment can be restored by running `deactivate_vtk_env`.

The following steps can be skipped when using the activate script.

### Using the Python modules

To use the VTK Python module, add the `build/lib/python-x.y/site-packages` subdirectory to your `PYTHONPATH`. You will also need to add the `python/` subdirectory of the source to your `PYTHONPATH` to be able to use the pure Python module.

### Using the ParaView Plugin

The C++ ParaView plugin can be loaded from `build/lib/plugins/MyPlugin/MyPlugin.so`. The Python plugin can be loaded from `python/plugin/load_python_plugins.py`. Loading this file in ParaView also adds the `python/` subdirectory to ParaView's PYTHONPATH.

## Install

This project also supports installing all binaries and Python sources to locations, where they are automatically found. The main use case for this is installing VTK or ParaView in your home directory, by supplying, e.g., `-DCMAKE_INSTALL_PREFIX=$HOME/paraview`, and running `ninja install`.
Using the same path for this project will allow your local ParaView installation to use your code without additionally setting environment variables.

## Superbuild

Using the docker image [lhofmann/paraview-superbuild-docker](https://github.com/lhofmann/paraview-superbuild-docker), distributable binaries, which are compatible with the Linux binaries from [paraview.org](https://www.paraview.org/), can be built. This only requires docker to be installed on your system, and your user to be allowed to run docker commands  (i.e., member of the docker group).

Running `./superbuild.sh` automatically builds the project in the docker container, and outputs binaries in the `package/` subdirectory.
Note, that rerunning the script will perform an incremental build. If you want to use the same script in multiple different directories, change the `container` variable in the script (it must be a unique name).