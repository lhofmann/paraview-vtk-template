# install the python module
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/my_python_module" DESTINATION "${python_site_packages_suffix}"
        FILES_MATCHING PATTERN "*.py")

if (ENABLE_PARAVIEW)
	# install plugin loader script
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/plugin/load_python_plugins.py" DESTINATION "${CMAKE_INSTALL_BINDIR}/plugins")
endif ()
