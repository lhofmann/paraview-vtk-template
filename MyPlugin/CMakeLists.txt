paraview_add_plugin(MyPlugin
  VERSION "1.0"
  MODULES Template::MyFilters
  SERVER_MANAGER_XML MyAlgorithm.xml
)