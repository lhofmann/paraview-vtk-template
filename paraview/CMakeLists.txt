if (_legacy_build_system)
  get_target_property(_source_dir ExampleFilter SOURCE_DIR)
  get_target_property(_headers_relative ExampleFilter HEADERS)
  set(_headers)
  foreach (header ${_headers_relative})
    if (IS_ABSOLUTE ${header})
      set(_headers ${_headers} "${header}")
    else ()
      set(_headers ${_headers} "${_source_dir}/${header}")
    endif ()
  endforeach ()

  add_paraview_plugin(ExampleFilterPlugin "1.0"
      SERVER_MANAGER_XML ExampleFilter.xml
      SERVER_MANAGER_SOURCES ${_headers})
  target_link_libraries(ExampleFilterPlugin PRIVATE ExampleFilter)
  set_target_properties(ExampleFilterPlugin PROPERTIES PREFIX "")
  set_target_properties(ExampleFilterPlugin PROPERTIES INSTALL_RPATH "$ORIGIN/..:$ORIGIN/")
  install(TARGETS ExampleFilterPlugin DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/plugins)
else ()
  paraview_add_plugin(ExampleFilterPlugin
    VERSION "1.0"
    MODULES ExampleFilter
    SERVER_MANAGER_XML ExampleFilter.xml
  )
  set_target_properties(ExampleFilterPlugin PROPERTIES INSTALL_RPATH "$ORIGIN/../../${CMAKE_INSTALL_LIBDIR}:$ORIGIN/")
  
  add_custom_target(ExampleFilterPlugin_symlink ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink 
      ExampleFilterPlugin/ExampleFilterPlugin.so 
      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/plugins/ExampleFilterPlugin.so)
  install(CODE "execute_process( \
      COMMAND ${CMAKE_COMMAND} -E create_symlink \
      ExampleFilterPlugin/ExampleFilterPlugin.so \
      ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/plugins/ExampleFilterPlugin.so \
      )"
  )
endif ()
