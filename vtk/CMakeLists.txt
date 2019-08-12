if (_legacy_build_system)  
  add_library(ExampleFilter ExampleFilter.cpp)
  target_link_libraries(ExampleFilter PRIVATE ${VTK_LIBRARIES})
  set_target_properties(ExampleFilter PROPERTIES HEADERS ExampleFilter.h)
  generate_export_header(ExampleFilter EXPORT_MACRO_NAME "EXAMPLEFILTER_EXPORT" EXPORT_FILE_NAME "ExampleFilterModule.h")
  target_include_directories(ExampleFilter PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  install(TARGETS ExampleFilter DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
else ()
  vtk_module_add_module(ExampleFilter
    SOURCES ExampleFilter.cpp
    HEADERS ExampleFilter.h
  )
endif ()
