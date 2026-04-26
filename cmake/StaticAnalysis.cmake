include_guard(GLOBAL)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

function(apply_static_analysis TARGET_NAME)
  if(NOT ${PROJECT_NAME}_ENABLE_CLANG_TIDY)
    return()
  endif()

  if(CLANG_TIDY_EXE)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  else()
    message(WARNING "clang-tidy requested but not found")
  endif()
endfunction()
