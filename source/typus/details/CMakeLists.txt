target_sources(
        ${PROJECT_NAME}
        INTERFACE FILE_SET HEADERS BASE_DIRS ${${PROJECT_NAME}_INCLUDE_ROOT}
        FILES to_array.hpp from_array.hpp normalize_naming.hpp finalize.hpp chain/chain.hpp chain/model.hpp chain/tag.hpp
)
