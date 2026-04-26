include_guard(GLOBAL)

function(add_project_workload TARGET_NAME)
  set(multi_value_args SOURCES LIBRARIES)
  cmake_parse_arguments(ARG "" "" "${multi_value_args}" ${ARGN})

  if(NOT ARG_SOURCES)
    file(GLOB ARG_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
  endif()

  if(NOT ARG_SOURCES)
    message(
      FATAL_ERROR
        "add_project_workload(${TARGET_NAME}): no sources found in ${CMAKE_CURRENT_SOURCE_DIR}"
    )
  endif()

  add_executable(${TARGET_NAME} ${ARG_SOURCES})

  target_link_libraries(${TARGET_NAME} PRIVATE ${PRJ_NAMESPACE}::${PROJECT_NAME} ${ARG_LIBRARIES})

  apply_standard_settings(${TARGET_NAME})

  target_compile_options(${TARGET_NAME} PRIVATE -fno-omit-frame-pointer)
endfunction()
