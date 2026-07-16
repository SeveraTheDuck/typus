target_sources(
        ${PROJECT_NAME}
        INTERFACE FILE_SET HEADERS BASE_DIRS ${${PROJECT_NAME}_INCLUDE_ROOT}
        FILES
        all.hpp
        any.hpp
        contains.hpp
        get.hpp
        index_of.hpp
        materialize.hpp
        none.hpp
        product.hpp
        size.hpp
        sum.hpp
        tag.hpp
)
