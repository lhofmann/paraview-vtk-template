set(module  ExampleModule)
set(sources ExampleFilter.cpp)
set(headers ExampleFilter.h)

if (_legacy_build_system)  
  add_library(${module} ${sources})
  target_link_libraries(${module} PRIVATE ${VTK_LIBRARIES})
  set_target_properties(${module} PROPERTIES HEADERS ${headers})
  generate_export_header(${module} EXPORT_FILE_NAME "${module}Module.h")
  target_include_directories(${module} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  install(TARGETS ${module} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
else ()
  vtk_module_add_module(${module}
    SOURCES ${sources}
    HEADERS ${headers}
  )
endif ()
