include_guard(GLOBAL)

set(${PROJECT_NAME}_CXX_STANDARD
    "26"
    CACHE STRING "C++ standard"
)

# Compiler version floors -- a guard for builds outside the Nix dev shell. Default to the pinned
# toolchain; lower here to support older compilers.
set(${PROJECT_NAME}_MIN_GCC
    "16"
    CACHE STRING "Minimum supported GCC version"
)
set(${PROJECT_NAME}_MIN_CLANG
    "22"
    CACHE STRING "Minimum supported Clang version"
)

option(${PROJECT_NAME}_ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(${PROJECT_NAME}_ENABLE_UBSAN "Enable Undefined Behavior Sanitizer" OFF)
option(${PROJECT_NAME}_ENABLE_TSAN "Enable Thread Sanitizer" OFF)
option(${PROJECT_NAME}_ENABLE_LTO "Enable Link Time Optimization" OFF)
option(${PROJECT_NAME}_ENABLE_CCACHE "Use ccache" ON)
option(${PROJECT_NAME}_ENABLE_INSTALL "Generate install / export rules" OFF)
option(${PROJECT_NAME}_ENABLE_COVERAGE "Enable coverage instrumentation" OFF)
