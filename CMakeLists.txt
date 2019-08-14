cmake_minimum_required(VERSION 3.8)

project(Example)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(ENABLE_PARAVIEW "Use ParaView (instead of VTK)" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE CACHE BOOL "")

if (ENABLE_PARAVIEW)
  find_package(ParaView REQUIRED)
else ()
  find_package(VTK REQUIRED)
endif ()

# name of our Python module
set(_python_module_name "my_vtk")

include(CMakePrintHelpers)
include(GNUInstallDirs)

# detect paraview and python paths
if ((ENABLE_PARAVIEW AND ParaView_VERSION VERSION_LESS 5.7) OR (NOT ENABLE_PARAVIEW AND VTK_VERSION VERSION_LESS 8.90.0))
  set(_legacy_build_system TRUE)
  set(CMAKE_INSTALL_LIBDIR lib)

  include(GenerateExportHeader)
  if (ENABLE_PARAVIEW)
    include(${ParaView_USE_FILE})
  else ()
    include(${VTK_USE_FILE})
  endif ()

  if (VTK_INSTALL_PREFIX)
    set(_vtk_prefix ${VTK_INSTALL_PREFIX})
    if (PARAVIEW_VERSION VERSION_GREATER 5.4)
      set(_paraview_plugin_path ${CMAKE_INSTALL_LIBDIR}/plugins)
    else ()
      set(_paraview_plugin_path ${CMAKE_INSTALL_LIBDIR}/paraview-${PARAVIEW_VERSION}/plugins)
    endif ()
  else ()
    # in build tree, plugins are located in bin/plugins
    if (ENABLE_PARAVIEW)
      set(_vtk_prefix ${ParaView_DIR})
    else ()
      set(_vtk_prefix ${VTK_DIR})
    endif ()
    set(_paraview_plugin_path ${CMAKE_INSTALL_BINDIR}/plugins)
  endif ()
else ()
  set(_legacy_build_system FALSE)
  set(_vtk_prefix ${VTK_PREFIX_PATH})
  set(_paraview_plugin_path ${CMAKE_INSTALL_BINDIR}/plugins)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

# setup RPATHs
file(RELATIVE_PATH _paraview_plugin_rpath /${_paraview_plugin_path} /${CMAKE_INSTALL_LIBDIR})
set(_paraview_plugin_rpath "$ORIGIN/${_paraview_plugin_rpath}")
if (VTK_WRAP_PYTHON)
  file(RELATIVE_PATH _python_module_path ${_vtk_prefix} ${VTK_PYTHONPATH})
  file(RELATIVE_PATH _python_module_rpath /${_python_module_path}/${_python_module_name} /${CMAKE_INSTALL_LIBDIR})
  set(_python_module_rpath "$ORIGIN/${_python_module_rpath}")
endif ()

message(STATUS "_vtk_prefix: ${_vtk_prefix}")
message(STATUS "_paraview_plugin_path: ${_paraview_plugin_path}")
message(STATUS "_paraview_plugin_rpath: ${_paraview_plugin_rpath}")
message(STATUS "_python_module_path: ${_python_module_path}")
message(STATUS "_python_module_rpath: ${_python_module_rpath}")

if (_legacy_build_system)
  add_subdirectory(vtk)
  if (ENABLE_PARAVIEW)
    add_subdirectory(paraview)
  endif ()
else ()
  set(_vtk_module_log ALL)

  # find vtk.module files in directory vtk/
  vtk_module_find_modules(module_files vtk)
  vtk_module_scan(
    MODULE_FILES         ${module_files}
    PROVIDES_MODULES     modules
    REQUIRES_MODULES     required_modules
    UNRECOGNIZED_MODULES unrecognized_modules
    WANT_BY_DEFAULT      ON)

  # build the modules (this calls add_subdirectory)
  vtk_module_build(
    MODULES             ${modules}
    INSTALL_HEADERS     OFF)

  if (VTK_WRAP_PYTHON)
    # build python modules
    vtk_module_wrap_python(
      MODULES             ${modules}
      MODULE_DESTINATION  "${_python_module_path}"
      INSTALL_HEADERS     OFF
      PYTHON_PACKAGE      ${_python_module_name}
      TARGET              PythonModules)
    foreach (module ${modules})
      set_target_properties(${module}Python PROPERTIES INSTALL_RPATH "${_python_module_rpath}")
    endforeach ()
  endif ()

  if (ENABLE_PARAVIEW)
    # find paraview.plugin files in directory paraview/
    paraview_plugin_find_plugins(plugin_files paraview)
    paraview_plugin_scan(
      PLUGIN_FILES      ${plugin_files}
      PROVIDES_PLUGINS  plugins
      REQUIRES_MODULES  plugin_required_modules
      ENABLE_BY_DEFAULT ON)

    # build the plugins (this calls add_subdirectory)
    paraview_plugin_build(
      PLUGINS ${plugins}
      TARGET  ParaViewPlugins
      LIBRARY_DESTINATION "${CMAKE_INSTALL_BINDIR}"
      LIBRARY_SUBDIRECTORY "plugins")
  endif ()
endif ()
