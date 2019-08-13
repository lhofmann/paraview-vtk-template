set(plugin             ExamplePlugin)
set(plugin_version     "1.0")
set(modules            ExampleModule)
set(server_manager_xml ExampleFilter.xml)

if (_legacy_build_system)
  set(_headers)
  foreach (module ${modules})
    get_target_property(_source_dir ${module} SOURCE_DIR)
    get_target_property(_headers_relative ${module} HEADERS)
    foreach (header ${_headers_relative})
      if (IS_ABSOLUTE ${header})
        set(_headers ${_headers} "${header}")
      else ()
        set(_headers ${_headers} "${_source_dir}/${header}")
      endif ()
    endforeach ()
  endforeach ()

  add_paraview_plugin(${plugin} ${plugin_version}
      SERVER_MANAGER_XML ${server_manager_xml}
      SERVER_MANAGER_SOURCES ${_headers})
  target_link_libraries(${plugin} PRIVATE ${modules})
  set_target_properties(${plugin} PROPERTIES PREFIX "")
  set_target_properties(${plugin} PROPERTIES INSTALL_RPATH "$ORIGIN/..:$ORIGIN/")
  install(TARGETS ${plugin} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/plugins)
else ()
  paraview_add_plugin(${plugin}
    VERSION ${plugin_version}
    MODULES ${modules}
    SERVER_MANAGER_XML ${server_manager_xml}
  )
  set_target_properties(${plugin} PROPERTIES INSTALL_RPATH "$ORIGIN/../../${CMAKE_INSTALL_LIBDIR}:$ORIGIN/")
  set_target_properties(${plugin} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins)

  install(CODE "execute_process( \
      COMMAND ${CMAKE_COMMAND} -E create_symlink \
      ${plugin}/${plugin}.so \
      ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/plugins/${plugin}.so \
      )"
  )
endif ()
