target_sources(
        ${PROJECT_NAME}
        INTERFACE FILE_SET HEADERS BASE_DIRS ${${PROJECT_NAME}_INCLUDE_ROOT}
        FILES
        empty.hpp
        from.hpp
        iota.hpp
        pair.hpp
        repeat.hpp
        singleton.hpp
)
