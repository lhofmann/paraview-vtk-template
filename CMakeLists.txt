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

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

if ((ENABLE_PARAVIEW AND ParaView_VERSION VERSION_LESS 5.7) OR (NOT ENABLE_PARAVIEW AND VTK_VERSION VERSION_LESS 8.90.0))
  set(_legacy_build_system TRUE)
else ()
  set(_legacy_build_system FALSE)
endif ()

if (_legacy_build_system)
  include(GenerateExportHeader)
  if (ENABLE_PARAVIEW)
    include(${ParaView_USE_FILE})
  else ()
    include(${VTK_USE_FILE})
  endif ()

  add_subdirectory(vtk)
  if (ENABLE_PARAVIEW)
    add_subdirectory(paraview)
  endif ()

  if (VTK_WRAP_PYTHON)
    # TODO
  endif ()
else ()
  set(_vtk_module_log ALL)

  vtk_module_find_modules(module_files ${CMAKE_CURRENT_SOURCE_DIR}/vtk)
  vtk_module_scan(
    MODULE_FILES         ${module_files}
    PROVIDES_MODULES     modules
    REQUIRES_MODULES     required_modules
    UNRECOGNIZED_MODULES unrecognized_modules
    WANT_BY_DEFAULT      ON)

  vtk_module_build(
    MODULES             ${modules}
    INSTALL_HEADERS     OFF)

  if (VTK_WRAP_PYTHON)
    file(RELATIVE_PATH python_module_path ${VTK_PREFIX_PATH} ${VTK_PYTHONPATH})

    vtk_module_wrap_python(
      MODULES             ${modules}
      MODULE_DESTINATION  "${python_module_path}"
      INSTALL_HEADERS     OFF
      PYTHON_PACKAGE      "my_vtk"
      TARGET              PythonModules)
  endif ()

  if (ENABLE_PARAVIEW)
    paraview_plugin_find_plugins(plugin_files ${CMAKE_CURRENT_SOURCE_DIR}/paraview)

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
  endif ()
endif ()
