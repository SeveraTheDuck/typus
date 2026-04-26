include_guard(GLOBAL)

# Returns INTERFACE for INTERFACE_LIBRARY, PRIVATE otherwise.
function(get_target_logical_scope TARGET_NAME OUT_VAR)
  get_target_property(target_type ${TARGET_NAME} TYPE)
  if(target_type STREQUAL "INTERFACE_LIBRARY")
    set(${OUT_VAR}
        "INTERFACE"
        PARENT_SCOPE
    )
  else()
    set(${OUT_VAR}
        "PRIVATE"
        PARENT_SCOPE
    )
  endif()
endfunction()

# Globs *.cpp under BASE_DIR and adds them as PRIVATE sources. Skips INTERFACE targets (header-only
# has no translation units).
function(target_collect_sources TARGET_NAME BASE_DIR)
  set(multi_value_args EXCLUDE_BASENAMES)
  cmake_parse_arguments(ARG "" "" "${multi_value_args}" ${ARGN})

  get_target_property(target_type ${TARGET_NAME} TYPE)
  if(target_type STREQUAL "INTERFACE_LIBRARY")
    return()
  endif()

  file(GLOB_RECURSE cpp_files CONFIGURE_DEPENDS "${BASE_DIR}/*.cpp")
  foreach(ex_file IN LISTS ARG_EXCLUDE_BASENAMES)
    list(FILTER cpp_files EXCLUDE REGEX ".*${ex_file}$")
  endforeach()

  target_sources(${TARGET_NAME} PRIVATE ${cpp_files})
endfunction()

# Globs *.hpp under BASE_DIR and registers them as a FILE_SET HEADERS. BASE_DIRS is set to BASE_DIR
# itself, so the include path preserves the subdirectory structure: files in
# BASE_DIR/project_name/foo.hpp are included as #include <project_name/foo.hpp> when BASE_DIR =
# source/.
function(target_collect_headers TARGET_NAME BASE_DIR)
  get_target_property(target_type ${TARGET_NAME} TYPE)

  if(target_type STREQUAL "INTERFACE_LIBRARY")
    set(SCOPE "INTERFACE")
  elseif(target_type STREQUAL "EXECUTABLE")
    set(SCOPE "PRIVATE")
  else()
    set(SCOPE "PUBLIC")
  endif()

  file(GLOB_RECURSE hpp_files CONFIGURE_DEPENDS "${BASE_DIR}/*.hpp")
  if(NOT hpp_files)
    return()
  endif()

  target_sources(
    ${TARGET_NAME}
    ${SCOPE}
    FILE_SET
    HEADERS
    BASE_DIRS
    "${BASE_DIR}"
    FILES
    ${hpp_files}
  )
endfunction()

# Collects sources/headers from CMAKE_CURRENT_SOURCE_DIR and applies standard compile settings,
# sanitizers, and static analysis.
function(target_standard_setup TARGET_NAME)
  set(multi_value_args EXCLUDE_BASENAMES)
  cmake_parse_arguments(ARG "" "" "${multi_value_args}" ${ARGN})

  target_collect_sources(
    ${TARGET_NAME} "${CMAKE_CURRENT_SOURCE_DIR}" EXCLUDE_BASENAMES ${ARG_EXCLUDE_BASENAMES}
  )
  target_collect_headers(${TARGET_NAME} "${CMAKE_CURRENT_SOURCE_DIR}")

  apply_standard_settings(${TARGET_NAME})
  apply_sanitizers(${TARGET_NAME})
  apply_static_analysis(${TARGET_NAME})
endfunction()

# Creates a library target of the requested type (defaults to ${PROJECT_NAME}_LIBRARY_TYPE),
# registers a namespaced alias, and runs target_standard_setup.
function(add_project_library TARGET_NAME)
  set(one_value_args TYPE)
  set(multi_value_args EXCLUDE_BASENAMES)
  cmake_parse_arguments(ARG "" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT ARG_TYPE)
    set(ARG_TYPE "${${PROJECT_NAME}_LIBRARY_TYPE}")
  endif()

  if(ARG_TYPE STREQUAL "HEADER_ONLY")
    add_library(${TARGET_NAME} INTERFACE)
  elseif(ARG_TYPE MATCHES "^(STATIC|SHARED)$")
    add_library(${TARGET_NAME} ${ARG_TYPE})
    set_target_properties(
      ${TARGET_NAME}
      PROPERTIES POSITION_INDEPENDENT_CODE ON
                 CXX_VISIBILITY_PRESET hidden
                 VISIBILITY_INLINES_HIDDEN ON
    )
  else()
    message(FATAL_ERROR "add_project_library: unknown TYPE '${ARG_TYPE}'. "
                        "Expected STATIC, SHARED, or HEADER_ONLY."
    )
  endif()

  add_library(${PRJ_NAMESPACE}::${TARGET_NAME} ALIAS ${TARGET_NAME})

  target_standard_setup(${TARGET_NAME} EXCLUDE_BASENAMES ${ARG_EXCLUDE_BASENAMES})
endfunction()

# Installs TARGET_NAME under EXPORT_NAME, including the HEADERS file set.
function(target_install_package TARGET_NAME EXPORT_NAME)
  include(GNUInstallDirs)

  install(
    TARGETS ${TARGET_NAME}
    EXPORT ${EXPORT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            FILE_SET HEADERS
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endfunction()
