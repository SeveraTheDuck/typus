target_sources(
        ${PROJECT_NAME}
        INTERFACE FILE_SET HEADERS BASE_DIRS ${${PROJECT_NAME}_INCLUDE_ROOT}
        FILES
        anchored.hpp
        combinator.hpp
        operation.hpp
        pipe_expr.hpp
        terminator.hpp
        thunk.hpp
)
