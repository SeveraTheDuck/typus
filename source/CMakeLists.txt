add_project_library(${PROJECT_NAME})

if(PROJECT_IS_TOP_LEVEL)
  target_install_package(${PROJECT_NAME} ${PROJECT_NAME}Targets)
endif()
