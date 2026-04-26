include_guard(GLOBAL)

set(${PROJECT_NAME}_LIBRARY_TYPE
    "STATIC"
    CACHE STRING "Library type: STATIC, SHARED, HEADER_ONLY"
)
set_property(CACHE ${PROJECT_NAME}_LIBRARY_TYPE PROPERTY STRINGS STATIC SHARED HEADER_ONLY)

set(${PROJECT_NAME}_CXX_STANDARD
    "23"
    CACHE STRING "C++ standard"
)

option(${PROJECT_NAME}_ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(${PROJECT_NAME}_ENABLE_UBSAN "Enable Undefined Behavior Sanitizer" OFF)
option(${PROJECT_NAME}_ENABLE_TSAN "Enable Thread Sanitizer" OFF)

option(${PROJECT_NAME}_ENABLE_LTO "Enable Link Time Optimization" OFF)

option(${PROJECT_NAME}_ENABLE_CCACHE "Use ccache" ON)
option(${PROJECT_NAME}_ENABLE_CLANG_TIDY "Run clang-tidy" OFF)

option(${PROJECT_NAME}_ENABLE_WORKLOADS "Build profiling workloads" OFF)
