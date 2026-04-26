include_guard(GLOBAL)

function(check_compiler_versions)
  set(MIN_CLANG "17.0")
  set(MIN_GCC "14.0")

  message(
    STATUS "[${PROJECT_NAME}] Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
  )

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS MIN_CLANG)
      message(FATAL_ERROR "Clang ${MIN_CLANG}+ required, got ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS MIN_GCC)
      message(FATAL_ERROR "GCC ${MIN_GCC}+ required, got ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
  else()
    message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}. Use GCC or Clang.")
  endif()
endfunction()
