include_guard(GLOBAL)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

function(generate_project_package_config EXPORT_NAME)
  set(one_value_args NAMESPACE)
  set(multi_value_args DEPENDENCIES)
  cmake_parse_arguments(ARG "" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT ARG_NAMESPACE)
    message(FATAL_ERROR "generate_project_package_config: NAMESPACE is required")
  endif()

  set(PACKAGE_DEPENDENCIES "")
  foreach(dep IN LISTS ARG_DEPENDENCIES)
    string(APPEND PACKAGE_DEPENDENCIES "find_dependency(${dep})\n")
  endforeach()

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  configure_package_config_file(
    "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ProjectConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

  install(
    EXPORT ${EXPORT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${ARG_NAMESPACE}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )
endfunction()
