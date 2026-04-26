include_guard(GLOBAL)
include(FetchContent)

function(setup_gtest)
  if(TARGET GTest::gtest_main)
    return()
  endif()

  find_package(GTest CONFIG QUIET)
  if(GTest_FOUND)
    message(STATUS "[${PROJECT_NAME}] Using system GTest")
    return()
  endif()

  message(STATUS "[${PROJECT_NAME}] Fetching GTest via FetchContent...")
  fetchcontent_declare(
    googletest URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
                   EXCLUDE_FROM_ALL SYSTEM
  )
  set(gtest_force_shared_crt
      ON
      CACHE BOOL "" FORCE
  )
  fetchcontent_makeavailable(googletest)
endfunction()
