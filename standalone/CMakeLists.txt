add_executable(myalgorithm myalgorithm_main.cpp)
target_link_libraries(myalgorithm PRIVATE Template::MyFilters VTK::IOXML)

# set relative RPATH and install
set(executables myalgorithm)
file(RELATIVE_PATH executable_rpath "/${CMAKE_INSTALL_BINDIR}" "/${CMAKE_INSTALL_LIBDIR}")
set(executable_rpath "$ORIGIN/${executable_rpath}")
foreach (executable ${executables})
  set_target_properties(${executable} PROPERTIES INSTALL_RPATH "${executable_rpath}")
  install(TARGETS ${executable}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endforeach ()
