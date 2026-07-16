add_subdirectory(base)
add_subdirectory(combine)
add_subdirectory(make)
add_subdirectory(model)
add_subdirectory(terminate)

add_subdirectory(details)

target_sources(
        ${PROJECT_NAME}
        INTERFACE FILE_SET HEADERS BASE_DIRS ${${PROJECT_NAME}_INCLUDE_ROOT}
        FILES typus.hpp
)
