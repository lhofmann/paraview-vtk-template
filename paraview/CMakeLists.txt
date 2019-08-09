paraview_add_plugin(ExampleFilterPlugin
  VERSION "1.0"
  MODULES ExampleFilter
  SERVER_MANAGER_XML ExampleFilter.xml
)

install(TARGETS ExampleFilterPlugin DESTINATION ${CMAKE_INSTALL_BINDIR}/plugins)
