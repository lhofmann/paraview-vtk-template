paraview_add_plugin(ExampleFilterPlugin
  VERSION "1.0"
  MODULES ExampleFilter
  SERVER_MANAGER_XML ExampleFilter.xml
)
 
install(CODE "execute_process( \
    COMMAND ${CMAKE_COMMAND} -E create_symlink \
    ExampleFilterPlugin/$<TARGET_FILE_NAME:ExampleFilterPlugin> \
    ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/plugins/$<TARGET_FILE_NAME:ExampleFilterPlugin> \
    )"
)
