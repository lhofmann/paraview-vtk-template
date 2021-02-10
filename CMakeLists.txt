cmake_minimum_required(VERSION 3.8)
project(Template)

# use C++11
set(CMAKE_CXX_STANDARD 11)

# build shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# use either VTK or ParaView (with its own VTK)
option(ENABLE_PARAVIEW "Use ParaView (instead of VTK)" ON)
if (ENABLE_PARAVIEW)
  find_package(ParaView REQUIRED)
else ()
  find_package(VTK REQUIRED)
endif ()

# setup installation directories
include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
# use relative RPATHs in build tree
set(CMAKE_BUILD_RPATH_USE_ORIGIN 1)

# uncomment to enable VTK debug messages
# set(_vtk_module_log ALL)

vtk_module_find_modules(module_files "${CMAKE_CURRENT_LIST_DIR}")
vtk_module_scan(
  MODULE_FILES         ${module_files}
  PROVIDES_MODULES     modules
  REQUIRES_MODULES     required_modules
  UNRECOGNIZED_MODULES unrecognized_modules
  WANT_BY_DEFAULT      ON)
vtk_module_build(
  MODULES             ${modules}
  INSTALL_HEADERS     OFF)

add_subdirectory(standalone)

if (VTK_WRAP_PYTHON)
  # name of the VTK Python module
  set(python_module_name "my_vtk_module")

  # determine path to Python site-packages
  if (IS_ABSOLUTE ${VTK_PYTHONPATH})
    file(RELATIVE_PATH python_site_packages_suffix ${VTK_PREFIX_PATH} ${VTK_PYTHONPATH})
  else()
    set(python_site_packages_suffix ${VTK_PYTHONPATH})
  endif()
  file(RELATIVE_PATH python_module_rpath "/${python_site_packages_suffix}/${python_module_name}" "/${CMAKE_INSTALL_LIBDIR}")
  set(python_module_rpath "$ORIGIN/${python_module_rpath}")


  vtk_module_wrap_python(
    MODULES             ${modules}
    MODULE_DESTINATION  "${python_site_packages_suffix}"
    INSTALL_HEADERS     OFF
    PYTHON_PACKAGE      "${python_module_name}"
    TARGET              PythonModules)

  # add relative RPATH to Python modules
  foreach (module_name ${modules})
    _vtk_module_get_module_property("${module_name}" PROPERTY "library_name" VARIABLE module)    
    set_target_properties(${module}Python PROPERTIES INSTALL_RPATH "${python_module_rpath}")
  endforeach ()

  # generate __init__.py
  set(init_py "from __future__ import absolute_import\n")
  foreach (module_name ${modules})
    _vtk_module_get_module_property("${module_name}" PROPERTY "library_name" VARIABLE module)
    if (TARGET ${module}Python)
      set(init_py "${init_py}from .${module} import *\n")
    endif ()
  endforeach ()
  file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/${python_site_packages_suffix}/${python_module_name}/__init__.py"
    CONTENT "${init_py}"
  )
  install(
    FILES "${CMAKE_BINARY_DIR}/${python_site_packages_suffix}/${python_module_name}/__init__.py"
    DESTINATION "${python_site_packages_suffix}/${python_module_name}"
  )

  add_subdirectory(python)
endif ()

if (ENABLE_PARAVIEW)
  # determine relative path from plugin destination to libraries
  file(RELATIVE_PATH plugin_rpath "/${CMAKE_INSTALL_BINDIR}/plugins/NAME" "/${CMAKE_INSTALL_LIBDIR}")
  set(plugin_rpath "$ORIGIN/${plugin_rpath}")

  paraview_plugin_find_plugins(plugin_files "${CMAKE_CURRENT_LIST_DIR}")
  paraview_plugin_scan(
    PLUGIN_FILES      ${plugin_files}
    PROVIDES_PLUGINS  plugins
    REQUIRES_MODULES  plugin_required_modules
    ENABLE_BY_DEFAULT ON)
  paraview_plugin_build(
    PLUGINS ${plugins}
    TARGET  ParaViewPlugins
    LIBRARY_DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY_SUBDIRECTORY "plugins")

  # add relative RPATH to Plugins
  foreach (plugin ${plugins})
    set_target_properties(${plugin} PROPERTIES INSTALL_RPATH "${plugin_rpath}")
  endforeach ()
endif ()

# generate activate script
set(_env_root "${CMAKE_BINARY_DIR}")
set(_env_bin "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(_env_lib "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(_env_bin "${_env_bin}:${VTK_PREFIX_PATH}/${CMAKE_INSTALL_BINDIR}")
set(_env_lib "${_env_lib}:${VTK_PREFIX_PATH}/${CMAKE_INSTALL_LIBDIR}")
set(_env_python)
if (VTK_WRAP_PYTHON)
  set(_env_python "${CMAKE_BINARY_DIR}/${python_site_packages_suffix}")
  set(_env_python "${_env_python}:${CMAKE_CURRENT_LIST_DIR}/python")
  set(_env_python "${_env_python}:${VTK_PREFIX_PATH}/${python_site_packages_suffix}")
endif ()
set(_env_paraview)
if (ENABLE_PARAVIEW)
  # add plugin directories to PV_PLUGIN_PATH
  foreach (plugin ${plugins})
    set(_env_paraview "${_env_paraview}:$<TARGET_FILE_DIR:${plugin}>")
  endforeach ()
endif ()
configure_file("activate.in" "${CMAKE_BINARY_DIR}/activate.in" @ONLY)
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/activate" INPUT "${CMAKE_BINARY_DIR}/activate.in")
