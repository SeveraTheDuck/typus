cmake_minimum_required(VERSION 3.28...3.30 FATAL_ERROR)

set(PRJ_NAMESPACE "severa")

project(
  project_name
  VERSION 0.1.0
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Options)

set(CMAKE_CXX_STANDARD ${${PROJECT_NAME}_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CompilerCheck)
include(TargetHelpers)
include(StandardFlags)
include(Sanitizers)
include(StaticAnalysis)
include(Dependencies)
include(PackageHelpers)
include(Ccache)
include(CTest)

check_compiler_versions()
setup_ccache()

add_subdirectory(source)

if(BUILD_TESTING)
  setup_gtest()
  add_subdirectory(tests)
endif()

if(${PROJECT_NAME}_ENABLE_WORKLOADS)
  add_subdirectory(workloads)
endif()

if(PROJECT_IS_TOP_LEVEL)
  generate_project_package_config(${PROJECT_NAME}Targets NAMESPACE ${PRJ_NAMESPACE}::)
endif()

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
       "${CMAKE_SOURCE_DIR}/compile_commands.json" SYMBOLIC
  )
endif()
