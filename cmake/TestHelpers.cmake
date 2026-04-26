include_guard(GLOBAL)
include(GoogleTest)

function(add_project_test TARGET_NAME)
  set(options)
  set(one_value_args)
  set(multi_value_args SOURCES LIBRARIES)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT ARG_SOURCES)
    file(GLOB ARG_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
  endif()

  if(NOT ARG_SOURCES)
    message(FATAL_ERROR "add_project_test(${TARGET_NAME}): no sources found")
  endif()

  add_executable(${TARGET_NAME} ${ARG_SOURCES})

  target_link_libraries(
    ${TARGET_NAME} PRIVATE ${PRJ_NAMESPACE}::${PROJECT_NAME} GTest::gtest_main ${ARG_LIBRARIES}
  )

  apply_standard_settings(${TARGET_NAME})
  apply_sanitizers(${TARGET_NAME})

  gtest_discover_tests(${TARGET_NAME} DISCOVERY_MODE PRE_TEST PROPERTIES LABELS ${TARGET_NAME})
endfunction()
