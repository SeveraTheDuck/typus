include_guard(GLOBAL)
include(CheckIPOSupported)

function(apply_standard_settings TARGET_NAME)
  get_target_logical_scope(${TARGET_NAME} SCOPE)

  target_compile_features(${TARGET_NAME} ${SCOPE} "cxx_std_${${PROJECT_NAME}_CXX_STANDARD}")

  target_compile_options(
    ${TARGET_NAME}
    ${SCOPE}
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
    -fstack-protector-strong
    $<$<NOT:$<CONFIG:Release>>:-fno-omit-frame-pointer>
    >
  )

  target_compile_definitions(
    ${TARGET_NAME} ${SCOPE} $<$<AND:$<CXX_COMPILER_ID:Clang,AppleClang,GNU>,$<CONFIG:Release>>:
                   _FORTIFY_SOURCE=2 >
  )

  if(${PROJECT_NAME}_ENABLE_LTO)
    check_ipo_supported(RESULT ipo_supported)
    if(ipo_supported)
      set_target_properties(
        ${TARGET_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
                                  INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON
      )
    endif()
  endif()
endfunction()
