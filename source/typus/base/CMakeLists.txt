target_sources(
        ${PROJECT_NAME}
        INTERFACE FILE_SET HEADERS BASE_DIRS ${${PROJECT_NAME}_INCLUDE_ROOT}
        FILES
        empty.hpp
        pair.hpp
        pipe.hpp
        singleton.hpp
        thunk.hpp
        type_tuple.hpp
)
